// di_test_impl2.h -- second test driver header

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
