// depinject/depinject.h
// -*- mode: c++ -*-
//
// Notes on interface base classes:
//
//     * For dependency injection to make sense, the dependent classes known to depinject
//       are assumed to be abstract base classes of those classes actually constructed
//       by the configuration code.  (Thus it never new()'s dependent objects into existance.)
//
//     * As depinject, by default, will destroy the dependent classes in its destructor,
//       dependent classes should generally sport a public virtual destructor.  (This
//       isn't required if the configuration code supplies its own destruction routine.)
//

#ifndef NOON_DEPINJECT_H

#include <memory>
#include <stdexcept>

namespace DepInject
{

  template <typename Dep>
  class DependencyBuilder {
  public:
    using Builder = Dep* (*)();

    void declare(Builder bldr, bool uniq = false) {
      builder = bldr;
      unique  = uniq;
      if (!unique)
        common_instance.reset();
    }

    Dep* get() {
      Dep* dep = nullptr;
      if (builder) {
        if (unique)
          dep = builder();
        else {
          if (!common_instance)
            common_instance.reset(builder());
          dep = common_instance.get();
        }
      }
      if (!dep) {
        throw std::logic_error("DepInject::DependencyBuilder::get: "
                               "cannot build object (no preceding declaration?)");
      }
      return dep;
    }

  private:
    Builder              builder {nullptr};
    std::unique_ptr<Dep> common_instance;
    bool                 unique  {false};
  };

} // DepInject

#endif  // NOON_DEPINJECT_H
