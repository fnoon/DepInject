// di_lamps.cc -- DepInject test driver lamp classes implementation file

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

#include "di_lamps.h"
#include "depinject.h"
#include <atomic>
#include <iostream>

using std::cout;
using std::endl;


//////////////////////////////////////////////////////////////////////////////////
//
//  class Lamp implementation.
//
//////////////////////////////////////////////////////////////////////////////////

Lamp::Lamp ( )
  : m_bulb(*DepInject::Factory<IBulb>::get())
{
  cout << "Lamp #" << lampcount(true) << " created\n";
}


Lamp::~Lamp ( )
{
  cout << "Lamp #" << lampcount() << " destroyed\n";
}


void
Lamp::toggle_switch ()
{
  m_current_flowing = !m_current_flowing;
  m_bulb.electrified(m_current_flowing);
  cout << "lamp turned " << (m_bulb.is_lit() ? "on" : "off") << endl;
}


bool
Lamp::is_lit () const
{
  return m_bulb.is_lit();
}


unsigned
Lamp::lampcount (bool incr)
{
  static std::atomic_uint count {0};
  if (incr)
    return count.fetch_add(1) + 1;
  else
    return count.load();
}


//////////////////////////////////////////////////////////////////////////////////
//
//  class LampWithUniqueBulb implementation.
//
//////////////////////////////////////////////////////////////////////////////////

LampWithUniqueBulb::LampWithUniqueBulb ( )
  : m_bulb(DepInject::Factory<IBulb, UniqueTag>::get_unique())
{
  cout << "lamp with unique bulb #" << lampcount(true) << " created\n";
}


LampWithUniqueBulb::~LampWithUniqueBulb ( )
{
  cout << "lamp with unique bulb #" << lampcount() << " destroyed\n";
}


void
LampWithUniqueBulb::toggle_switch ( )
{
  m_current_flowing = !m_current_flowing;
  m_bulb->electrified(m_current_flowing);
  cout << "lamp turned " << (m_bulb->is_lit() ? "on" : "off") << endl;
}


bool
LampWithUniqueBulb::is_lit ( ) const
{
  return m_bulb->is_lit();
}


unsigned
LampWithUniqueBulb::lampcount (bool incr)
{
  static std::atomic_uint count {0};
  if (incr)
    return count.fetch_add(1) + 1;
  else
    return count.load();
}


//////////////////////////////////////////////////////////////////////////////////
//
//  class GaudyLamp implementation.
//
//////////////////////////////////////////////////////////////////////////////////

GaudyLamp::GaudyLamp ( )
  : m_bulb(*DepInject::Factory<IBulb, GaudyTag>::get())
{
  cout << "gaudy lamp #" << lampcount(true) << " created\n";
}


GaudyLamp::~GaudyLamp ( )
{
  cout << "gaudy lamp #" << lampcount() << " destroyed\n";
}


void
GaudyLamp::toggle_switch ( )
{
  m_current_flowing = !m_current_flowing;
  m_bulb.electrified(m_current_flowing);
  cout << "gaudy lamp turned " << (m_bulb.is_lit() ? "on" : "off") << endl;
}


bool
GaudyLamp::is_lit ( ) const
{
  return m_bulb.is_lit();
}


unsigned
GaudyLamp::lampcount (bool incr)
{
  static std::atomic_uint count {0};
  if (incr)
    return count.fetch_add(1) + 1;
  else
    return count.load();
}
