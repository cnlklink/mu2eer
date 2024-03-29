TCLK_HOST_OUT   = $(HOST_BIN_DIR)/tclk
TCLK_TARGET_OUT = $(TARGET_BIN_DIR)/tclk

ALL_CLEAN       += tclk_clean
ALL_TARGETS     += $(HOST_BIN_DIR)/tclk.a $(TARGET_BIN_DIR)/tclk.a
ALL_TEST        += tclk_tests
ALL_OUT         += $(TCLK_HOST_OUT) $(TCLK_TARGET_OUT)
ALL_SOURCES     += $(wildcard tclk/*.C)
ALL_HEADERS     += $(wildcard tclk/*.H)

TCLK_LIBS        = tclk.a config.a api.a
TCLK_LIBS_HOST   = $(addprefix $(HOST_BIN_DIR)/,$(TCLK_LIBS))
TCLK_LIBS_TARGET = $(addprefix $(TARGET_BIN_DIR)/,$(TCLK_LIBS))

TCLK_OBJS        = \
	TCLKDecoderFactory.o \
	ITCLKDecoderDriver.o \
	TCLKDecoderDriverMock.o \
	MulticastTCLKDecoderDriver.o \
	TCLKSubsystem.o
TCLK_OBJS_PREFIX = $(addprefix tclk/,$(TCLK_OBJS))
TCLK_OBJS_HOST   = $(addprefix $(TCLK_HOST_OUT)/,$(TCLK_OBJS))
TCLK_OBJS_TARGET = $(addprefix $(TCLK_TARGET_OUT)/,$(TCLK_OBJS))

TCLK_TEST_OBJS   = AllTests.o TCLKDecoderDriverMockTests.o TCLKSubsystemTests.o TCLKFactoryTests.o
TCLK_TEST_OBJS_PREFIX = $(addprefix tclk/,$(TCLK_TEST_OBJS))
TCLK_TEST_OBJS_HOST = $(addprefix $(TCLK_HOST_OUT)/,$(TCLK_TEST_OBJS))

ALL_COVERAGE     += $(TCLK_OBJS_HOST)

$(TCLK_TARGET_OUT):
	$(EES_OUT) mkdir -p $(TCLK_TARGET_OUT)

$(TCLK_HOST_OUT): 
	$(EES_OUT) mkdir -p $(TCLK_HOST_OUT)

tclk_clean:
	$(EES_OUT) rm -f tclk/*.o

# Host tclk library
$(HOST_BIN_DIR)/tclk.a: $(TCLK_OBJS_PREFIX)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_AR) ru $@ $(TCLK_OBJS_HOST)
	$(EES_OUT) $(HOST_RANLIB) $@

# Target tclk library
$(TARGET_BIN_DIR)/tclk.a: $(TCLK_OBJS_PREFIX)
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(AR) ru $@ $(TCLK_OBJS_TARGET)
	$(EES_OUT) $(RANLIB) $@

# Unit test suite
tclk_tests: $(TCLK_LIBS_HOST) $(TCLK_TEST_OBJS_PREFIX)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(TCLK_HOST_OUT)/tclk_tests \
		$(TCLK_TEST_OBJS_HOST) \
		$(TCLK_LIBS_HOST) \
		$(DEV_LIBS) $(TEST_FLAGS) \
		-L$(HOST_LIB_DIR)
	@echo "-m-> Running $@..."
	@./$(TCLK_HOST_OUT)/tclk_tests $(TEST_RUN_FLAGS)
