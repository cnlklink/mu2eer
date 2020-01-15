# EES_PREFIX	= $(prefix)
# EES_PROJECT	= mu2eer
# include $(EES_PREFIX)/usr/local/products/include/ees.mk

# EES_CFLAGS	= -I$(EES_ERL_LIBS)/cdev-1.2/include -I$(EES_ERL_LIBS)/acnet-2.1/include $(EES_PREF_CFLAGS)	$(EES_PREF_LXRT_CFLAGS)	-fno-strict-aliasing -D'BOARD_SUPPORT=$(BPMD_BOARD_ID)'
EES_CPPFLAGS	= -I$(EES_ERL_LIBS)/cdev-1.2/include -I$(EES_ERL_LIBS)/acnet-2.1/include $(EES_PREF_CPPFLAGS)	$(EES_PREF_LXRT_CPPFLAGS) -fno-strict-aliasing -std=c++0x -I$(EES_INC) -I$(EES_ERL_LIBS)/cdev-1.2/include -I$(EES_ERL_LIBS)/acnet-2.1/include -I$(EES_PREFIX)/usr/include/libnl3 -D'BOARD_SUPPORT=$(BPMD_BOARD_ID)'
# EES_LDFLAGS	= $(EES_PREF_LDFLAGS)	$(EES_PREF_LXRT_LDFLAGS)
# ERLANG_INT	= erl_interface-3.8.2
# MYLIBS		= $(EES_ERLANG_LIBDIR)/lib/$(ERLANG_INT)/lib

#CPPUTEST_HOME = /home/jdiamond/cpputest
#TEST_FLAGS = -I$(CPPUTEST_HOME)/include -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
TEST_FLAGS = -lCppUTest -lCppUTestExt

DEV_LIBS = -lpthread
TARGET_LIBS = -lpthread -lerl_interface -lei

DEV_OBJS =
TARGET_OBJS = $(EES_ERL_LIBS)/cdev-1.2/priv/fef_driver_lib.o

# List of all C++ source files
EES_CPPSOURCES_A       = $(wildcard *.cpp) $(wildcard ../adc/*.cpp)
EES_CPPSOURCES_B       = $(wildcard *.C) $(wildcard ../adc/*.C)

# List of modules (.o files)
EES_OBJS               = $(EES_CPPSOURCES_A:.cpp=.o) $(EES_CPPSOURCES_B:.C=.o) $(CSOURCES:.c=.o)

# Compile all C modules
.c.o: $(EES_CSOURCES)
	$(CC) -c -o $*.o $(EES_CFLAGS) $<

# Compile all C++ modules
.cpp.o: $(EES_CPPSOURCES_A)
	$(CXX) -c -o $*.o $(EES_CPPFLAGS) $(TEST_FLAGS) $<
.C.o: $(EES_CPPSOURCES_B)
	$(CXX) -c -o $*.o $(EES_CPPFLAGS) $(TEST_FLAGS) $<

ALL_CLEAN =
ALL_TARGETS =
ALL_TEST =

include ACNET/ACNET.mk
include adc/adc.mk
include tclk/tclk.mk

.PHONY: all test clean

all: $(ALL_TARGETS)

test: $(ALL_TEST)

clean: $(ALL_CLEAN)

