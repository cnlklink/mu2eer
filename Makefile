# EES_OUT               = 
EES_PREFIX	      = $(prefix)
EES_PROJECT	      = mu2eer

EES_TARGET_ARCH       = arm
EES_TARGET_ARCH2      = -gnueabihf
EES_TARGET_BOARD      = achilles
EES_TARGET_OS         = linux
EES_TARGET_OS_VERSION = 4.1.22-ltsi

EES_BR_BASE           = /usr/local/products/buildroot/ees/ees_buildroot_achilles

include /usr/local/products/elsd/include/elsd-2.5.mk

EES_CFLAGS	= -I$(EES_ERL_LIBS)/cdev-1.2/include -I$(EES_ERL_LIBS)/acnet-2.1/include $(EES_PREF_CFLAGS)	$(EES_PREF_LXRT_CFLAGS)	-fno-strict-aliasing -D'BOARD_SUPPORT=$(BPMD_BOARD_ID)'
EES_CPPFLAGS	= -I$(EES_ERL_LIBS)/cdev-1.2/include -I$(EES_ERL_LIBS)/acnet-2.1/include $(EES_PREF_CPPFLAGS)	$(EES_PREF_LXRT_CPPFLAGS) -fno-strict-aliasing -std=c++0x -I$(EES_INC) -I$(EES_ERL_LIBS)/cdev-1.2/include -I$(EES_ERL_LIBS)/acnet-2.1/include -I$(EES_PREFIX)/usr/include/libnl3 -D'BOARD_SUPPORT=$(BPMD_BOARD_ID)'
EES_LDFLAGS	= $(EES_PREF_LDFLAGS)	$(EES_PREF_LXRT_LDFLAGS)
ERLANG_INT	= erl_interface-3.10.4
MYLIBS		= $(EES_ERLANG_LIBDIR)/lib/$(ERLANG_INT)/lib

TEST_FLAGS = -lCppUTest -lCppUTestExt

DEV_LIBS = -lpthread
TARGET_LIBS = -lpthread -lerl_interface -lei

DEV_OBJS =
TARGET_OBJS = $(EES_ERL_LIBS)/cdev-1.2/priv/fef_driver_lib.o

EES_HOST_CPPFLAGS = -fno-strict-aliasing -std=c++0x -D'BOARD_SUPPORT=$(BPMD_BOARD_ID)' $(DEV_LIBS) $(TEST_FLAGS) 

# List of all C++ source files
EES_CPPSOURCES_A       = $(wildcard *.cpp) $(wildcard adc/*.cpp)
EES_CPPSOURCES_B       = $(wildcard *.C) $(wildcard adc/*.C) $(wildcard ACNET/*.C)

# List of modules (.o files)
EES_OBJS               = $(EES_CPPSOURCES_A:.cpp=.o) $(EES_CPPSOURCES_B:.C=.o) $(CSOURCES:.c=.o)

ALL_CLEAN =
ALL_TARGETS =
ALL_TEST =

include ACNET/ACNET.mk
include adc/adc.mk
include tclk/tclk.mk

HOST_CXX = g++

#
# Compile all C++ modules
#
.cpp.o: $(EES_CPPSOURCES_A) %.d Makefile
	@ echo "-m-> Compiling C++ file $< (target) ..."
	$(EES_OUT) $(CXX) -c -o $*.o $(EES_CPPFLAGS) $<
	touch $*.o

.C.o: $(EES_CPPSOURCES_B) %.d Makefile
	@ echo "-m-> Compiling C++ file $< (target) ..."
	$(EES_OUT) $(CXX) -c -o output/target/$*.o $(EES_CPPFLAGS) $<
	@ echo "-m-> Compiling C++ file $< (host) ..."
	$(EES_OUT) $(HOST_CXX) -c -o output/host/$*.o $(EES_HOST_CPPFLAGS) $<
	@ touch $*.o

.PHONY: all test clean

all: $(ALL_TARGETS)

tests: $(ALL_TEST)

clean: $(ALL_CLEAN)
	-rm -rf output/docs

docs: $(EES_CPPSOURCES_B)
	@ echo "-m-> Generating documentation..."
	$(EES_OUT) doxygen

