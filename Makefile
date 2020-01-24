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
EES_CPPSOURCES_B       = $(wildcard *.C) $(wildcard adc/*.C) $(wildcard ACNET/*.C) $(wildcard tclk/*.C)
EES_HEADERS            = $(wildcard *.H) $(wildcard adc/*.H) $(wildcard ACNET/*.H) $(wildcard tclk/*.H)

# List of modules (.o files)
EES_OBJS               = $(EES_CPPSOURCES_A:.cpp=.o) $(EES_CPPSOURCES_B:.C=.o) $(CSOURCES:.c=.o)

ALL_TARGETS = output
ALL_TEST =
ALL_CLEAN =
ALL_OUT =

include ACNET/ACNET.mk
include adc/adc.mk
include tclk/tclk.mk

HOST_CXX = g++
SCP      = scp -q
SSH      = ssh -K
TARGET_TEST_HOSTNAME = srsd
TAR_FILENAME = mu2eer.tar

#
# Compile all C++ modules
#
.cpp.o: output $(EES_CPPSOURCES_A) %.d Makefile
	@ echo "-m-> Compiling C++ file $< (target) ..."
	$(EES_OUT) $(CXX) -c -o $*.o $(EES_CPPFLAGS) $<
	touch $*.o

.C.o: output $(EES_CPPSOURCES_B) %.d Makefile
	@ echo "-m-> Compiling C++ file $< (target) ..."
	$(EES_OUT) $(CXX) -c -o output/target/$*.o $(EES_CPPFLAGS) $<
	@ echo "-m-> Compiling C++ file $< (host) ..."
	$(EES_OUT) $(HOST_CXX) -c -o output/host/$*.o $(EES_HOST_CPPFLAGS) $<
	@ touch $*.o

.PHONY: all all_buildroot test clean docs deploy_test install_buildroot_target

# Make everything
all: $(ALL_TARGETS) docs

all_buildroot: $(ALL_TARGETS)

# Make and run unit test suite
tests: $(ALL_TEST)

# Clean output/ directory
clean: $(ALL_CLEAN)
	@ echo "-m-> Cleaning..."
	$(EES_OUT) -rm -rf output/

# Run Doxygen to generate API documentation
docs: output/docs/html/index.html

# Main Doxygen target
output/docs/html/index.html: output $(EES_CPPSOURCES_B) $(EES_HEADERS) Doxyfile
	@ echo "-m-> Generating documentation..."
	$(EES_OUT) doxygen

# Creates the output/ directory structure
output: $(ALL_OUT)
	$(EES_OUT) mkdir -p output
	$(EES_OUT) mkdir -p output/target
	$(EES_OUT) mkdir -p output/host
	$(EES_OUT) mkdir -p output/docs/html

# Deploys directly to TARGET_TEST_HOSTNAME via scp/ssh
deploy_test: 
	@ echo "-m-> Deploying to $(TARGET_TEST_HOSTNAME)..."
	@ echo "-m-> (please make sure $(TARGET_TEST_HOSTNAME) is in your ~/.ssh/config file)"
	$(EES_OUT) $(SCP) $(ACNET_TARGET_OUT)/acnet_adc $(TARGET_TEST_HOSTNAME):/tmp
	$(EES_OUT) $(SSH) -t $(TARGET_TEST_HOSTNAME) "ksu -e /bin/mkdir -p /usr/local/products/bin"
	$(EES_OUT) $(SSH) -t $(TARGET_TEST_HOSTNAME) "ksu -e /bin/chmod 755 /usr/local/products/bin"
	$(EES_OUT) $(SSH) -t $(TARGET_TEST_HOSTNAME) "ksu -e /bin/busybox mv /tmp/acnet_adc /usr/local/products/bin"

install_buildroot_target:
	install -d $(EES_BIN)
	install -D -m 0755 $(@D)/$(ACNET_TARGET_OUT)/acnet_adc $(EES_PREFIX)/usr/local/products/acsysfe/lib/cdev-1.2/priv/acnet_adc
