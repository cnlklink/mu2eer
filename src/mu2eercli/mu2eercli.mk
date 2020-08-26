MU2EERCLI_HOST_OUT   = $(HOST_BIN_DIR)/mu2eercli
MU2EERCLI_TARGET_OUT = $(TARGET_BIN_DIR)/mu2eercli

MU2EERCLI_LIBS        = api.a adc.a ssm.a config.a
MU2EERCLI_LIBS_HOST   = $(addprefix $(HOST_BIN_DIR)/,$(MU2EERCLI_LIBS))
MU2EERCLI_LIBS_TARGET = $(addprefix $(TARGET_BIN_DIR)/,$(MU2EERCLI_LIBS))

MU2EERCLI_OBJS        = \
	main.o \
	CLI.o \
	Command.o \
	PIDCommand.o \
	ShutdownCommand.o \
	ShowCommand.o \
	StartCommand.o
MU2EERCLI_OBJS_PREFIX = $(addprefix mu2eercli/,$(MU2EERCLI_OBJS))
MU2EERCLI_OBJS_HOST   = $(addprefix $(MU2EERCLI_HOST_OUT)/,$(MU2EERCLI_OBJS))
MU2EERCLI_OBJS_TARGET = $(addprefix $(MU2EERCLI_TARGET_OUT)/,$(MU2EERCLI_OBJS))

MU2EERCLI_TEST_OBJS        = \
	AllTests.o \
	CLITests.o CLI.o \
	Command.o \
	PIDCommand.o \
	ShutdownCommand.o \
	ShowCommand.o \
	StartCommand.o
MU2EERCLI_TEST_OBJS_PREFIX = $(addprefix mu2eercli/,$(MU2EERCLI_TEST_OBJS))
MU2EERCLI_TEST_OBJS_HOST   = $(addprefix $(MU2EERCLI_HOST_OUT)/,$(MU2EERCLI_TEST_OBJS))

ALL_CLEAN       += mu2eercli_clean
ALL_TARGETS     += mu2eercli
ALL_TEST        += mu2eercli_tests
ALL_OUT         += $(MU2EERCLI_HOST_OUT) $(MU2EERCLI_TARGET_OUT)
ALL_SOURCES     += $(wildcard mu2eercli/*.C)
ALL_HEADERS     += $(wildcard mu2eercli/*.H)
ALL_COVERAGE    += $(MU2EERCLI_OBJS_HOST)

$(MU2EERCLI_TARGET_OUT):
	$(EES_OUT) mkdir -p $(MU2EERCLI_TARGET_OUT)

$(MU2EERCLI_HOST_OUT): 
	$(EES_OUT) mkdir -p $(MU2EERCLI_HOST_OUT)

mu2eercli_clean:
	$(EES_OUT) rm -f mu2eercli/*.o

# mu2eer command line interface application
mu2eercli: $(MU2EERCLI_OBJS_PREFIX) $(MU2EERCLI_LIBS_HOST) $(MU2EERCLI_LIBS_TARGET)
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) -o $(MU2EERCLI_TARGET_OUT)/mu2eercli \
		$(MU2EERCLI_OBJS_TARGET) \
		$(MU2EERCLI_LIBS_TARGET) \
		$(EES_LDFLAGS) -L$(MYLIBS) $(DEV_LIBS)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(MU2EERCLI_HOST_OUT)/mu2eercli \
		$(MU2EERCLI_OBJS_HOST) \
		$(MU2EERCLI_LIBS_HOST) \
		$(DEV_LIBS) $(TEST_FLAGS)

# Unit test suite
mu2eercli_tests: $(MU2EERCLI_LIBS_HOST) $(MU2EERCLI_TEST_OBJS_PREFIX) mu2eerd/Controller.o
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(MU2EERCLI_HOST_OUT)/mu2eercli_tests \
		$(MU2EERCLI_TEST_OBJS_HOST) \
		$(HOST_BIN_DIR)/mu2eerd/Controller.o \
		$(MU2EERCLI_LIBS_HOST) \
		$(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(MU2EERCLI_HOST_OUT)/mu2eercli_tests $(TEST_RUN_FLAGS)
