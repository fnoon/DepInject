// di_test_impl2.cc -- second test driver implementation file

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

#include "di_test_impl2.h"
#include "depinject.h"
#include <iostream>

using std::cout;
using std::endl;


//////////////////////////////////////////////////////////////////////////////////
//
//  (concrete) class GaudyBulb implementation.
//
//////////////////////////////////////////////////////////////////////////////////

GaudyBulb::GaudyBulb ()
{
  cout << "gaudy bulb created\n";
}


// Restore copy operations, overriding base class deletions.
GaudyBulb::GaudyBulb (GaudyBulb const& b)
  : m_is_lit(b.m_is_lit)
{ }


GaudyBulb&
GaudyBulb::operator= (GaudyBulb const& b)
{
  m_is_lit = b.m_is_lit;
  return *this;
}


void
GaudyBulb::do_electrified (bool receiving_current)
{
  m_is_lit = receiving_current;
}


bool
GaudyBulb::do_is_lit() const
{
  return m_is_lit;
}


//////////////////////////////////////////////////////////////////////////////////
//
//  class GaudyLamp implementation.
//
//////////////////////////////////////////////////////////////////////////////////

GaudyLamp::GaudyLamp()
  : m_bulb(*DepInject::Factory<IBulb, GaudyTag>::get())
{
  cout << "gaudy lamp created\n";
}


void
GaudyLamp::toggle_switch ()
{
  m_current_flowing = !m_current_flowing;
  m_bulb.electrified(m_current_flowing);
  cout << "gaudy lamp turned " << (m_bulb.is_lit() ? "on" : "off") << endl;
}


bool
GaudyLamp::is_lit () const
{
  return m_bulb.is_lit();
}
