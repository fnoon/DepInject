# DepInject makefile
#
# Copyright © 2018 Frederick Noon.  All rights reserved.
#
# This file is part of DepInject.
#
# DepInject is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# DepInject is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with DepInject.  If not, see
# <https://www.gnu.org/licenses/>.

CXX       = c++
CXXFLAGS += -std=c++14
LD        = c++

USE_GOOGLETEST =
USE_DOCTEST    = true

ifdef USE_GOOGLETEST
# Googletest requirements.
GTEST        = ./gtest
CXXFLAGS    += -pthread -isystem $(GTEST_DIR)/googletest/include
#LDFLAGS     += -L$(GTEST)
#LDLIBS      += -lgtest_main
GTEST_LIB    = $(GTEST)/libgtest_main.a
endif

ifdef USE_DOCTEST
DOCTEST_ROOT = $(HOME)/src/third/doctest
CPPFLAGS    += -I$(DOCTEST_ROOT)/doctest
endif


.PHONY: all clean

all: di_test

di_test: di_main.o di_bulbs.o di_lamps.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -rf *.o di_test

ifdef USE_GOOGLETEST
di_test: $(GTEST_LIB)

$(GTEST_LIB):
	$(MAKE) -C $(GTEST)

.PHONY: clean_gtest
clean_gtest:
	$(MAKE) -C $(GTEST) clean

clean: clean_gtest
endif
