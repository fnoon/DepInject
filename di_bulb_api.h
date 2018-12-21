// di_bulb_api.h -- DepInject test driver header declaring IBulb interface

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

#ifndef NOON_DI_BULB_API_H
#define NOON_DI_BULB_API_H

//---------------------------------------------------------------------
// Note: The IBulb interface is all that the lamp classes know about.
//       The lamp classes don't use the di_bulbs.h header file.
//       All bulb classes are implemented in di_bulbs.cc.
//---------------------------------------------------------------------

//
// IBulb class: An interface class describing all bulb types.
//
class IBulb {
public:

  // Allow deletion through an interface pointer.
  virtual ~IBulb();

  // Forbid copying (slicing) via an interface pointer.
  IBulb(IBulb const&) = delete;
  IBulb& operator=(IBulb const&) = delete;

  // Non-virtual public interface.
  void electrified(bool receiving_current);

  bool is_lit() const;

protected:
  // Forbid instantiation of a bare interface class object.
  IBulb();

private:
  // Virtual hooks for derived class implementations.
  virtual void do_electrified(bool receiving_current) = 0;
  virtual bool do_is_lit() const = 0;
};

#endif // NOON_DI_BULB_API_H
