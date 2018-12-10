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


.PHONY: all clean

all: di_test

di_test: di_test_main.o di_test_impl.o di_test_impl2.o
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
