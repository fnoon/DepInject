CXX       = c++
CXXFLAGS += -std=c++14
LD        = c++
LDFLAGS  += -stdlib=libc++

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


all: di_test

di_test:   di_test.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

di_test.o: di_test.cc

.PHONY: clean
clean:
	rm -rf *.o di_test

ifdef USE_GOOGLETEST
di_test : $(GTEST_LIB)

$(GTEST_LIB):
	$(MAKE) -C $(GTEST)

.PHONY: clean_gtest
clean_gtest:
	$(MAKE) -C $(GTEST) clean

clean: clean_gtest
endif
