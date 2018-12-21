// di_main.cc -- DepInject test driver setup code

//================================================================================
//
// Copyright © 2018 Frederick Noon.  All rights reserved.
//
// This file is part of DepInject.
//
// DepInject is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// DepInject is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with DepInject.  If not, see
// <https://www.gnu.org/licenses/>.
//

#include "di_bulbs.h"
#include "di_lamps.h"
#include "depinject.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <type_traits>

using std::cout;
using std::endl;


// Create strings for doctest to print for the types used in our tests.
TYPE_TO_STRING(Lamp);
TYPE_TO_STRING(LampWithUniqueBulb);
TYPE_TO_STRING(GaudyLamp);

// A tag never used by any class to retrieve a dependency.
struct WrongTag {};


// reset_all_factories:
//   Clear entries from all (singleton) Factory<>'s to establish a known
//   global state before running the next unit test.  (This maneuver should
//   never be needed in any production code.)
void
reset_all_factories ( )
{
    DepInject::Factory<IBulb           >::testing_reset();
    DepInject::Factory<IBulb, UniqueTag>::testing_reset();
    DepInject::Factory<IBulb, GaudyTag >::testing_reset();
    DepInject::Factory<IBulb, WrongTag >::testing_reset();
}


// DepTraits:
//   Traits classes to help make more generic tests.
template <typename T>
struct DepTraits {
  using tag = DepInject::DefaultTag;
  static const bool declared_unique = false;
};

template <>
struct DepTraits <LampWithUniqueBulb> {
  using tag = UniqueTag;
  static const bool declared_unique = true;
};

template <>
struct DepTraits <GaudyLamp> {
  using tag = GaudyTag;
  static const bool declared_unique = false;
};


// tag_required_for_declaration:
//   Determine whether the given Lamp-like type uses a (non-default) tag when retrieving
//   an IBulb from a factory.  If not, then the 'tag' parameter may be omitted on declare()
//   and get() calls.
template <typename Lamp_T>
bool tag_required_for_declaration ( )
{
  return !std::is_same<typename DepTraits<Lamp_T>::tag, DepInject::DefaultTag>::value;
}


// call_standard_declaration:
//   A helper function for the unit tests, to let them call the combinations of tagged /
//   non-tagged and unique / non-unique declarations in a common manner.  (Separate cases
//   omitting a tag when the default tag is used weren't strictly necessary, but I wanted
//   to "exercise" the the template's default parameter.)
template <typename Requestor, typename Dependency>
void call_standard_declaration (Dependency* (*builder)())
{
  if (tag_required_for_declaration<Requestor>()) {
    if (DepTraits<Requestor>::declared_unique)
      DepInject::Factory<Dependency, typename DepTraits<Requestor>::tag>::declare_unique(builder);
    else
      DepInject::Factory<Dependency, typename DepTraits<Requestor>::tag>::declare(builder);
  }
  else {
    if (DepTraits<Requestor>::declared_unique)
      DepInject::Factory<Dependency>::declare_unique(builder);
    else
      DepInject::Factory<Dependency>::declare(builder);
  }
}


TEST_SUITE("Test exception generation")
{
  SCENARIO_TEMPLATE("Common exception tests", Lamp_T, Lamp, LampWithUniqueBulb, GaudyLamp) {
    reset_all_factories();

    GIVEN("A Lamp-like class") {
      // Here: we use the Lamp_T alias.

      WHEN("get() is called before declare()") {
        THEN("an exception should be thrown") {
          CHECK_THROWS_WITH(Lamp_T lamp,
                        "DepInject: get: object type+tag not declared");
        }
      }
      AND_WHEN("The dependency allocator function fails") {
        // Register a BuildFunc which always fails.
        call_standard_declaration<Lamp_T, IBulb>([]() -> IBulb* {return nullptr;});

        THEN("The attempted allocation should generate an exception") {
          CHECK_THROWS_WITH(Lamp_T lamp, "DepInject: get: object allocation failed");
        }
      }
      AND_WHEN("The dependency allocator function succeeds") {
        // Register a BuildFunc which "should" always succeed.
        call_standard_declaration<Lamp_T, IBulb>([]() -> IBulb* {return new Bulb;});

        THEN("The object should be allocated and returned") {
          CHECK_NOTHROW(Lamp_T lamp);
        }
      }
      AND_WHEN("The dependency allocator function is called with the wrong tag") {
        DepInject::Factory<IBulb, WrongTag>::declare([]() -> IBulb* {return new Bulb;});

        THEN("The attempted allocation should throw") {
          CHECK_THROWS_WITH(Lamp_T lamp,
                            "DepInject: get: object type+tag not declared");
        }
      }
    }
  }

  SCENARIO("Exceptions for non-unique classes") {
    reset_all_factories();

    GIVEN("A class with a non-unique dependency") {
      using Lamp_T = Lamp;

      WHEN("The dependency allocator function is registered as \"unique\"") {
        DepInject::Factory<IBulb>::declare_unique([]() -> IBulb* {return new Bulb;});

        THEN("The attempted allocation should throw") {
          CHECK_THROWS_WITH(Lamp_T lamp,
                            "DepInject: get: request for "
                            "non-unique instance doesn't match declaration");
        }
      }
    }
  }

  SCENARIO("Exceptions for unique classes") {
    reset_all_factories();

    GIVEN("A class with a unique dependency") {
      using Lamp_T = LampWithUniqueBulb;

      WHEN("The dependency allocator function is NOT registered as \"unique\"") {
        DepInject::Factory<IBulb, UniqueTag>::declare([]() -> IBulb* {return new Bulb;});

        THEN("The attempted allocation should throw") {
          CHECK_THROWS_WITH(Lamp_T lamp,
                            "DepInject: get: request for "
                            "unique instance doesn't match declaration");
        }
      }
    }
  }
}


template <typename Lamp_T>
void exercise_lamp_wiring ( )
{
  Lamp_T lamp;
  REQUIRE(!lamp.is_lit());
  lamp.toggle_switch();
  REQUIRE(lamp.is_lit());
  lamp.toggle_switch();
  REQUIRE(!lamp.is_lit());
}


TEST_CASE("Test basic factory functionality")
{
  reset_all_factories();

  // We "hand-craft" our Bulb builders, then call exercise_lamp_wiring() to
  // exercise them.

  // For the simple case, use the helper function equivalent to:
  // DepInject::Factory<IBulb>::declare([]() -> IBulb* {return new Bulb;});
  DepInject::basic_declaration<IBulb, Bulb>();

  // Register a "unique" Bulb builder (used by LampWithUniqueBulb).
  DepInject::Factory<IBulb, UniqueTag>::declare_unique([]() -> IBulb* {return new Bulb;});

  // Register a Gaudy Bulb builder (used by Gaudy Lamp constructor).
  DepInject::Factory<IBulb, GaudyTag>::declare([]() -> IBulb* {return new GaudyBulb;});

  exercise_lamp_wiring<Lamp>();
  exercise_lamp_wiring<LampWithUniqueBulb>();
  exercise_lamp_wiring<GaudyLamp>();
}
