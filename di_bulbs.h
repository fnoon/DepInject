// di_bulbs.h -- DepInject test driver header declaring bulb classes

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

#ifndef NOON_DI_BULBS_H
#define NOON_DI_BULBS_H

#include "di_bulb_api.h"

//
//  Bulb class: a concrete class implementing the IBulb interface.
//
class Bulb : public IBulb {
public:
  Bulb();

  // Restore copy operations, overriding base class deletions.
  Bulb(Bulb const& b);
  Bulb& operator=(Bulb const& b);

private:
  virtual void do_electrified(bool receiving_current) override;
  virtual bool do_is_lit() const override;

  bool m_is_lit {false};
};


//
//  GaudyBulb class: A second concrete class implementing the IBulb interface.
//
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


#endif // NOON_DI_BULBS_H
