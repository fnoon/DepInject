// di_test_impl.cc -- first test driver implementation file

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
#include "depinject.h"
#include <iostream>

using std::cout;
using std::endl;


//////////////////////////////////////////////////////////////////////////////////
//
//  (abstract) class IBulb implementation.
//
//////////////////////////////////////////////////////////////////////////////////

// Allow deletion through an interface pointer.
IBulb::~IBulb()
{ }


// Non-virtual public interface.
void
IBulb::electrified (bool receiving_current)
{
  do_electrified(receiving_current);
}


bool
IBulb::is_lit () const
{
  return do_is_lit();
}


// Protected: forbid instantiation of a bare interface class object.
IBulb::IBulb ()
{ }


//////////////////////////////////////////////////////////////////////////////////
//
//  (concrete) class Bulb implementation.
//
//////////////////////////////////////////////////////////////////////////////////

Bulb::Bulb ()
{
  cout << "bulb created\n";
}


// Restore copy operations, overriding base class deletions.
Bulb::Bulb (Bulb const& b)
  : m_is_lit(b.m_is_lit)
{ }


Bulb&
Bulb::operator= (Bulb const& b)
{
  m_is_lit = b.m_is_lit;
  return *this;
}


void
Bulb::do_electrified (bool receiving_current)
{
  m_is_lit = receiving_current;
}


bool
Bulb::do_is_lit() const
{
  return m_is_lit;
}


//////////////////////////////////////////////////////////////////////////////////
//
//  class Lamp implementation.
//
//////////////////////////////////////////////////////////////////////////////////

Lamp::Lamp()
  : m_bulb(*DepInject::Factory<IBulb>::get())
{
  cout << "lamp created\n";
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


//////////////////////////////////////////////////////////////////////////////////
//
//  class LampWithUniqueBulb implementation.
//
//////////////////////////////////////////////////////////////////////////////////

LampWithUniqueBulb::LampWithUniqueBulb()
  : m_bulb(DepInject::Factory<IBulb, UniqueTag>::get_unique())
{
  cout << "lamp with unique bulb created\n";
}


void
LampWithUniqueBulb::toggle_switch ()
{
  m_current_flowing = !m_current_flowing;
  m_bulb->electrified(m_current_flowing);
  cout << "lamp turned " << (m_bulb->is_lit() ? "on" : "off") << endl;
}


bool
LampWithUniqueBulb::is_lit () const
{
  return m_bulb->is_lit();
}
