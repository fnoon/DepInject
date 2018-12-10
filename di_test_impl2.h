// depinject/di_test_impl2.h

#ifndef NOON_DI_TEST_IMPL2_H
#define NOON_DI_TEST_IMPL2_H

#include "di_test_impl.h"


class GaudyBulb : public IBulb {
public:
  GaudyBulb();

  // Restore copy operations, overriding base class deletions.
  GaudyBulb(GaudyBulb const& b);
  GaudyBulb& operator=(GaudyBulb const& b);

private:
  virtual void do_electrified(bool receiving_current) override;
  virtual bool do_is_lit() const override;

  bool m_is_lit {false};
};


struct GaudyTag { };


class GaudyLamp {
public:
  GaudyLamp();

  void toggle_switch();

  bool is_lit() const;

private:
  IBulb& m_bulb;
  bool   m_current_flowing {false};
};


#endif // NOON_DI_TEST_IMPL2_H
