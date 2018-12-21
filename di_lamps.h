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

#ifndef NOON_DI_LAMPS_H
#define NOON_DI_LAMPS_H

#include "di_bulb_api.h"
#include <memory>

//-------------------------------------------------------------------------
// Note: These lamp classes provide the same "concept" API, however they
//       are not derived from a common class interface.  This demonstrates
//       that independent classes can use the same dependency objects
//       retrieved from the DepInject factories.
//-------------------------------------------------------------------------

//
//  Lamp class: A class using a shared IBulb-based class.
//
class Lamp {
public:
  Lamp();

  void toggle_switch();

  bool is_lit() const;

private:
  IBulb& m_bulb;
  bool   m_current_flowing {false};
};


//
//  LampWithUniqueBulb class: A class using a unique IBulb-based instance.
//
struct UniqueTag { };

class LampWithUniqueBulb {
public:
  LampWithUniqueBulb();

  void toggle_switch();

  bool is_lit() const;

private:
  std::unique_ptr<IBulb> m_bulb;
  bool                   m_current_flowing {false};
};


//
//  GaudyLamp class: A class using a shared IBulb object instance it retiries using the GaudyTag tag.
//
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

#endif // NOON_DI_LAMPS_H
