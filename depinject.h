// depinject.h -- DepInject implementation file

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
//================================================================================


// Notes on interface base classes:
//
//     * For dependency injection to make sense, the dependent classes known to depinject
//       are assumed to be abstract base classes of those classes actually constructed by
//       the configuration code.  (Thus it never new()'s dependent objects into
//       existance.)
//
//     * As depinject, by default, will destroy the dependent classes in its destructor,
//       dependent classes should generally sport a public virtual destructor.
//
//     * A dependency registration may optionally be flagged as "unique," meaning that
//       depinject will not maintain a common pointer to the new object for other classes
//       to use.  In this case, the get() caller is responsible for the ultimate
//       destruction of the returned object.

#ifndef NOON_DEPINJECT_H

#include <memory>
#include <stdexcept>
#include <string>

namespace DepInject
{
  namespace Internals
  {
    //
    //  A Builder object can build dependencies.
    //
    template <typename Dep>
    class Builder {
    public:
      using BuildFunc = Dep* (*)();

      void declare (BuildFunc bldr, bool uniq) {
        if (builder)
          throw std::logic_error("DepInject: declare: redeclaration for same type");
        if (!bldr)
          throw std::logic_error("DepInject: declare: no allocation function provided");
        builder = bldr;
        unique  = uniq;
      }

      Dep* get (bool uniq) {
        if (!builder) {
          throw std::logic_error("DepInject: get: object type not declared");
        }
        if (uniq != unique) {
          std::string qualif {unique ? "non-" : ""};
          throw std::logic_error("DepInject: get: "
                                 "request for " + qualif + " unique instance doesn't match declaration");
        }

        Dep* dep = nullptr;
        if (unique)
          dep = builder();
        else {
          if (!common_instance)
            common_instance.reset(builder());
          dep = common_instance.get();
        }

        if (dep)
          return dep;
        else
          throw std::runtime_error("DepInject: get: object allocation failed");
      }

      void testing_reset ( ) {
        // This function is intended to reinitialize the Builder singleton for testing
        // DepInject itself.  Not for general use.
        builder = nullptr;
        common_instance.reset();
        unique = false;
      }

    private:
      BuildFunc            builder {nullptr};
      std::unique_ptr<Dep> common_instance;
      bool                 unique  {false};
    };

  } // Internals


  //
  //  Builders work in (singleton) Factories.
  //  DepInject users only call Factory<Dep> methods.
  //
  struct DefaultTag { };

  template <typename Dep, typename Tag = DefaultTag>
  class Factory {
    using Builder = Internals::Builder<Dep>;
  public:
    Factory() = default;
    Factory(Factory const&) = delete;
    Factory& operator=(Factory const&) = delete;

    static void declare (typename Builder::BuildFunc bldr) {
      auto builder = instance();
      builder->declare(bldr, false);
    }

    static void declare_unique (typename Builder::BuildFunc bldr) {
      auto builder = instance();
      builder->declare(bldr, true);
    }

    static Dep* get ( ) {
      auto builder = instance();
      return builder->get(false);
    }

    static Dep* get_unique ( ) {
      auto builder = instance();
      return builder->get(true);
    }

    static void testing_reset ( ) {
      // This function is intended to reinitialize the Builder singleton for testing
      // DepInject itself.  Not for general use.
      auto builder = instance();
      builder->testing_reset();
    }

  private:
    static Builder* instance ( ) {
      static Builder builder;
      return &builder;
    }
  };


  // A helper function, for the simplest cases.
  template <typename Dep, typename Concrete, typename Tag = DefaultTag>
  void basic_declaration() {
    Factory<Dep, Tag>::declare([]() -> Dep* {return new Concrete;});
  }

} // DepInject

#endif  // NOON_DEPINJECT_H
