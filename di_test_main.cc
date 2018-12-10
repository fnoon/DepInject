// di_test_main.cc -- test driver setup code

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

#include "di_test_impl.h"
#include "di_test_impl2.h"
#include "depinject.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>

using std::cout;
using std::endl;


TEST_CASE("Test builder exceptions")
{
  // Calling get() before registration: no builder function.
  REQUIRE_THROWS_WITH(Lamp lamp,
                      "DepInject::Builder::get: cannot build object (no BuildFunc declared)");
}


TEST_CASE("Test builder exceptions")
{
  // Register a BuildFunc which always fails.
  DepInject::Factory<IBulb>::declare([]() -> IBulb* {return nullptr;});
  REQUIRE_THROWS_WITH(Lamp lamp, "DepInject::Builder::get: cannot build object (BuildFunc failed)");
}


TEST_CASE("Test basic factory functionality")
{
  // Register a Bulb builder (used by Lamp constructor).
//  DepInject::Factory<IBulb>::declare([]() -> IBulb* {return new Bulb;});

  DepInject::basic_declaration<IBulb, Bulb>();

  Lamp lamp;
  REQUIRE(!lamp.is_lit());
  lamp.toggle_switch();
  REQUIRE(lamp.is_lit());
  lamp.toggle_switch();
  REQUIRE(!lamp.is_lit());

  Bulb b1;
  Bulb b2(b1);
  b1 = b2;
  b2 = std::move(b1);
  Bulb b3(std::move(b2));

  IBulb* ib1(new Bulb);
  IBulb* ib2(new Bulb);
  IBulb* ib3 = ib2;
  cout << ib1->is_lit() << endl;
  cout << ib2->is_lit() << endl;
  ib2->electrified(true);
  // *ib1 = *ib2;
  // cout << ib1->is_lit() << endl;
  // cout << ib2->is_lit() << endl;
}


TEST_CASE("Test tagged factory functionality")
{
  // Register a Gaudy Bulb builder (used by Gaudy Lamp constructor).
  DepInject::Factory<IBulb, GaudyTag>::declare([]() -> IBulb* {return new GaudyBulb;});

  GaudyLamp lamp;
  REQUIRE(!lamp.is_lit());
  lamp.toggle_switch();
  REQUIRE(lamp.is_lit());
  lamp.toggle_switch();
  REQUIRE(!lamp.is_lit());
}
