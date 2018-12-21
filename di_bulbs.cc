// di_bulbs.cc -- DepInject test driver bulb classes implementation file

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

#include "di_bulbs.h"
#include <iostream>

using std::cout;
using std::endl;


//////////////////////////////////////////////////////////////////////////////////
//
//  (abstract) class IBulb implementation.
//
//////////////////////////////////////////////////////////////////////////////////

// Allow deletion through an interface pointer.
IBulb::~IBulb ( )
{ }


// Non-virtual public interface.
void
IBulb::electrified (bool receiving_current)
{
  do_electrified(receiving_current);
}


bool
IBulb::is_lit ( ) const
{
  return do_is_lit();
}


// Protected: forbid instantiation of a bare interface class object.
IBulb::IBulb ( )
{ }


//////////////////////////////////////////////////////////////////////////////////
//
//  (concrete) class Bulb implementation.
//
//////////////////////////////////////////////////////////////////////////////////

Bulb::Bulb ( )
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
Bulb::do_is_lit ( ) const
{
  return m_is_lit;
}


//////////////////////////////////////////////////////////////////////////////////
//
//  (concrete) class GaudyBulb implementation.
//
//////////////////////////////////////////////////////////////////////////////////

GaudyBulb::GaudyBulb ( )
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
GaudyBulb::do_is_lit ( ) const
{
  return m_is_lit;
}
