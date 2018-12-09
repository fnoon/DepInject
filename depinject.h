// depinject/depinject.h
//
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
//

#ifndef NOON_DEPINJECT_H

#include <memory>
#include <stdexcept>
#include <string>

namespace DepInject
{

  template <typename Dep>
  class Builder {
  public:
    using BuildFunc = Dep* (*)();

    void declare(BuildFunc bldr, bool uniq) {
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
        std::string diag {builder ? "BuildFunc failed" : "no BuildFunc declared"};
        throw std::logic_error("DepInject::Builder::get: "
                               "cannot build object (" + diag + ")");
      }
      return dep;
    }

  private:
    BuildFunc            builder {nullptr};
    std::unique_ptr<Dep> common_instance;
    bool                 unique  {false};
  };


  template <typename Dep>
  class Factory {
  public:
    static void declare(typename Builder<Dep>::BuildFunc bldr, bool uniq = false) {
      auto builder = instance();
      builder->declare(bldr, uniq);
    }

    static Dep* get() {
      auto builder = instance();
      return builder->get();
    }

  private:
    static Builder<Dep>* instance() {
      static Builder<Dep> builder;
      return &builder;
    }
  };

} // DepInject

#endif  // NOON_DEPINJECT_H

// Emacs:
// Local Variables:
// mode: c++
// fill-column: 90
// comment-column: 0
// End:
