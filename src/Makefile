# Un-comment to display shell commands
# EES_OUT = 

# Configure ELSD build tools
EES_PREFIX	         = $(prefix)
EES_PROJECT	         = mu2eer
EES_TARGET_ARCH          = arm
EES_TARGET_ARCH2         = -gnueabihf
EES_TARGET_BOARD         = achilles
EES_TARGET_BOARD_VARIANT = mu2eer
EES_TARGET_OS            = linux
EES_TARGET_OS_VERSION    = 4.1.22-ltsi
EES_BR_BASE              = /usr/local/products/buildroot/achilles_mu2eer
EES_CDEV_VERSION         = 1.5

# Include ELSD build tools
include /usr/local/products/elsd/include/elsd-latest.mk

EES_CPPFLAGS	= -I$(INC_DIR) \
	-I$(EES_CDEV_INC) \
	-I$(EES_ERL_LIBS)/acnet-2.1/include \
	$(EES_PREF_CPPFLAGS) \
	$(EES_PREF_LXRT_CPPFLAGS) \
	-fno-strict-aliasing \
	-std=c++0x \
	-I$(EES_INC) \
	-I$(EES_ERL_LIBS)/acnet-2.1/include
EES_LDFLAGS	= $(EES_PREF_LDFLAGS) $(EES_PREF_LXRT_LDFLAGS)
ERLANG_INT	= erl_interface-3.10.4
MYLIBS		= $(EES_ERLANG_LIBDIR)/lib/$(ERLANG_INT)/lib

# Include CppUTest unit testing libraries
# -fprofile-arcs and -ftest-coverage enable gcov test coverage analysis
TEST_FLAGS      = -lCppUTest -lCppUTestExt -fprofile-arcs -ftest-coverage 

# Flags passed when the tests are ran
TEST_RUN_FLAGS  ?= -c

# Library locations
HOST_LIB_DIR	= /usr/local/products/lib

# -lrt needed for POSIX shared memory
DEV_LIBS        = -lconfig++ -lrt -lmcastrecv
TARGET_LIBS     = -lerl_interface -lei -lconfig++ -lrt -lmcastrecv

EES_HOST_CPPFLAGS = -I/usr/local/products/include \
	-I$(INC_DIR) \
	-fno-strict-aliasing \
	-std=c++0x \
	$(DEV_LIBS) \
	$(TEST_FLAGS) 

# List of all C++ source files
ALL_SOURCES     := $(wildcard *.C) 
ALL_HEADERS     := $(wildcard *.H) 

ALL_TARGETS     := $(BIN_DIR)
ALL_TEST        :=
ALL_CLEAN       :=
ALL_OUT         :=
ALL_COVERAGE    :=

BIN_DIR         := ../bin
TARGET_BIN_DIR  := $(BIN_DIR)/target
HOST_BIN_DIR    := $(BIN_DIR)/host
DOC_DIR         := ../docs
INC_DIR         := ../include
ETC_DIR         := ../etc
DB_DIR		:= ../db

# Some more host utilities
SCP             = scp -q
SSH             = ssh -K
HOST_GCOV       = gcov -r

TARGET_TEST_HOSTNAME = srsd

# Include module makefiles
include ACNET/ACNET.mk
include adc/adc.mk
include api/api.mk
include config/config.mk
include mu2eercli/mu2eercli.mk
include mu2eerd/mu2eerd.mk
include ssm/ssm.mk
include tclk/tclk.mk

.PHONY: all all_buildroot test clean docs deploy_test install_buildroot_target

# Compile all C++ modules
%.o: %.C Makefile $(BIN_DIR)
	@ echo "-m-> Compiling C++ file $< (target) ..."
	$(EES_OUT) $(CXX) -c -o $(TARGET_BIN_DIR)/$*.o $(EES_CPPFLAGS) $<
	@ echo "-m-> Compiling C++ file $< (host) ..."
	$(EES_OUT) $(HOST_CXX) -c -o $(HOST_BIN_DIR)/$*.o $(EES_HOST_CPPFLAGS) $<
	@ touch $*.o

# Make everything
all: $(ALL_TARGETS) docs

# Make everything from inside an ees-buildroot build
all_buildroot: $(ALL_TARGETS)

# Make and run unit test suite
tests: $(ALL_TEST)
	@echo "-m-> Coverage report for $(ALL_COVERAGE)..."
	$(EES_OUT) $(HOST_GCOV) $(ALL_COVERAGE)

# Clean output/ directory
clean: $(ALL_CLEAN)
	@ echo "-m-> Cleaning..."
	$(EES_OUT) -rm -rf *.gcov
	$(EES_OUT) -rm -rf *.pkginfo
	$(EES_OUT) -rm -rf $(BIN_DIR)
	$(EES_OUT) -rm -rf $(DOC_DIR)

# Generate API documentation
docs: $(DOC_DIR)/html/index.html

# Main Doxygen target
$(DOC_DIR)/html/index.html: $(DOC_DIR)/ Doxyfile $(ALL_SOURCES) $(ALL_HEADERS)
	@ echo "-m-> Generating documentation..."
	$(EES_OUT) doxygen

# Creates the doc directory structure
$(DOC_DIR)/:
	$(EES_OUT) mkdir -p $(DOC_DIR)/html

# Creates the binary directory structure
$(BIN_DIR): $(ALL_OUT)
	$(EES_OUT) mkdir -p $(BIN_DIR)
	$(EES_OUT) mkdir -p $(TARGET_BIN_DIR)
	$(EES_OUT) mkdir -p $(HOST_BIN_DIR)

# Install target used by ees-buildroot
install_buildroot_target: install_buildroot_pkginfo
	install -d $(EES_BIN)
	install -D -m 0755 $(@D)/../etc/init.d/* $(EES_PREFIX)/etc/init.d/
	mkdir -p $(EES_PREFIX)/etc/mu2eer.d
	install -D -m 0744 $(@D)/../etc/mu2eer.d/* $(EES_PREFIX)/etc/mu2eer.d/
	install -D -m 0755 $(@D)/$(ACNET_TARGET_OUT)/acnet_adc $(EES_PREFIX)$(EES_BASE)/acsysfe/lib/cdev-$(EES_CDEV_VER)/priv
	install -D -m 0755 $(@D)/$(ACNET_TARGET_OUT)/acnet_mu2eerd $(EES_PREFIX)$(EES_BASE)/acsysfe/lib/cdev-$(EES_CDEV_VER)/priv
	install -D -m 0755 $(@D)/$(ACNET_TARGET_OUT)/acnet_ssm $(EES_PREFIX)$(EES_BASE)/acsysfe/lib/cdev-$(EES_CDEV_VER)/priv
	install -D -m 0755 $(@D)/$(ACNET_TARGET_OUT)/acnet_tclk $(EES_PREFIX)$(EES_BASE)/acsysfe/lib/cdev-$(EES_CDEV_VER)/priv
	install -D -m 0755 $(@D)/ACNET/startacsys $(EES_PREFIX)/usr/local/products/acsysfe/bin/startacsys
	install -D -m 0755 $(@D)/$(MU2EERD_TARGET_OUT)/mu2eerd $(EES_PREFIX)/usr/local/products/bin/mu2eerd
	install -D -m 0755 $(@D)/$(MU2EERCLI_TARGET_OUT)/mu2eercli $(EES_PREFIX)/usr/local/products/bin/mu2eercli
