MU2EERD_HOST_OUT   = $(HOST_BIN_DIR)/mu2eerd
MU2EERD_TARGET_OUT = $(TARGET_BIN_DIR)/mu2eerd

MU2EERD_LIBS        = ssm.a config.a api.a tclk.a
MU2EERD_LIBS_HOST   = $(addprefix $(HOST_BIN_DIR)/,$(MU2EERD_LIBS))
MU2EERD_LIBS_TARGET = $(addprefix $(TARGET_BIN_DIR)/,$(MU2EERD_LIBS))

MU2EERD_OBJS        = main.o Controller.o
MU2EERD_OBJS_PREFIX = $(addprefix mu2eerd/,$(MU2EERD_OBJS))
MU2EERD_OBJS_HOST   = $(addprefix $(MU2EERD_HOST_OUT)/,$(MU2EERD_OBJS))
MU2EERD_OBJS_TARGET = $(addprefix $(MU2EERD_TARGET_OUT)/,$(MU2EERD_OBJS))

MU2EERD_TEST_OBJS        = AllTests.o ControllerTests.o Controller.o
MU2EERD_TEST_OBJS_PREFIX = $(addprefix mu2eerd/,$(MU2EERD_TEST_OBJS))
MU2EERD_TEST_OBJS_HOST   = $(addprefix $(MU2EERD_HOST_OUT)/,$(MU2EERD_TEST_OBJS))

ALL_CLEAN       += mu2eerd_clean
ALL_TARGETS     += mu2eerd
ALL_TEST        += mu2eerd_tests
ALL_OUT         += $(MU2EERD_HOST_OUT) $(MU2EERD_TARGET_OUT)
ALL_SOURCES     += $(wildcard mu2eerd/*.C)
ALL_HEADERS     += $(wildcard mu2eerd/*.H)
ALL_COVERAGE    += $(MU2EERD_OBJS_HOST)

$(MU2EERD_TARGET_OUT):
	$(EES_OUT) mkdir -p $(MU2EERD_TARGET_OUT)

$(MU2EERD_HOST_OUT): 
	$(EES_OUT) mkdir -p $(MU2EERD_HOST_OUT)

mu2eerd_clean:
	$(EES_OUT) rm -f mu2eerd/*.o

# mu2eerd daemon
mu2eerd: $(MU2EERD_OBJS_PREFIX) $(MU2EERD_LIBS_HOST) $(MU2EERD_LIBS_TARGET)
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) -o $(MU2EERD_TARGET_OUT)/mu2eerd \
		$(MU2EERD_OBJS_TARGET) \
		$(MU2EERD_LIBS_TARGET) \
		$(EES_LDFLAGS) -L$(MYLIBS) $(DEV_LIBS)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(MU2EERD_HOST_OUT)/mu2eerd \
		$(MU2EERD_OBJS_HOST) \
		$(MU2EERD_LIBS_HOST) \
		$(DEV_LIBS) $(TEST_FLAGS) \
		-L$(HOST_LIB_DIR)

# Unit test suite
mu2eerd_tests: $(MU2EERD_LIBS_HOST) $(MU2EERD_TEST_OBJS_PREFIX)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(MU2EERD_HOST_OUT)/mu2eerd_tests \
		$(MU2EERD_TEST_OBJS_HOST) \
		$(MU2EERD_LIBS_HOST) \
		$(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	$(EES_OUT)./$(MU2EERD_HOST_OUT)/mu2eerd_tests $(TEST_RUN_FLAGS)
