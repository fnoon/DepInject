#include "depinject.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>

using std::cout;
using std::endl;


class IBulb {
public:

  // Allow deletion through an interface pointer.
  virtual ~IBulb() { }

  // Forbid copying (slicing) via an interface pointer.
  IBulb(IBulb const&) = delete;
  IBulb& operator=(IBulb const&) = delete;

  // Non-virtual public interface.
  void electrified(bool receiving_current) {
    do_electrified(receiving_current);
  }

  bool is_lit() const {
    return do_is_lit();
  }

protected:
  // Forbid instantiation of a bare interface class object.
  IBulb() { }

private:
  // Virtual hooks for derived class implementations.
  virtual void do_electrified(bool receiving_current) = 0;
  virtual bool do_is_lit() const = 0;
};


class Bulb : public IBulb {
public:
  Bulb() {
      cout << "bulb created\n";
  }

  // Restore copy operations, overriding base class deletions.
  Bulb(Bulb const& b)
    : m_is_lit(b.m_is_lit)
    { }

  Bulb& operator=(Bulb const& b) {
    m_is_lit = b.m_is_lit;
    return *this;
  }

private:
  virtual void do_electrified(bool receiving_current) override {
    m_is_lit = receiving_current;
  }

  virtual bool do_is_lit() const override {
    return m_is_lit;
  }

  bool m_is_lit {false};
};

//template void DepInject::Dependency<IBulb>::declare(Creator cr, bool unique = false);
//template DepInject::Dependency<IBulb>::DepSpec DepInject::Dependency<IBulb>::spec;

DepInject::DependencyBuilder<IBulb> ibulb_builder;
IBulb* build_our_ibulb() {
  return new Bulb;
}

class Lamp {
public:
  Lamp()
//  : m_bulb(*DepInject::DependencyBuilder<IBulb>::get()) {
    : m_bulb(*ibulb_builder.get()) {
      cout << "lamp created\n";
  }

  void toggle_switch() {
      m_current_flowing = !m_current_flowing;
      m_bulb.electrified(m_current_flowing);
      cout << "lamp turned " << (m_bulb.is_lit() ? "on" : "off") << endl;
  }

  bool is_lit() const {
    return m_bulb.is_lit();
  }

private:
  IBulb& m_bulb;
  bool   m_current_flowing {false};
};


TEST_CASE("Test basic factory functionality")
{

  ibulb_builder.declare(build_our_ibulb);

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
