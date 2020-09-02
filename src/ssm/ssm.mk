SSM_HOST_OUT   = $(HOST_BIN_DIR)/ssm
SSM_TARGET_OUT = $(TARGET_BIN_DIR)/ssm

ALL_CLEAN       += ssm_clean
ALL_TARGETS     += $(HOST_BIN_DIR)/ssm.a $(TARGET_BIN_DIR)/ssm.a
ALL_TEST        += ssm_tests
ALL_OUT         += $(SSM_HOST_OUT) $(SSM_TARGET_OUT)
ALL_SOURCES     += $(wildcard ssm/*.C)
ALL_HEADERS     += $(wildcard ssm/*.H)

SSM_LIBS        = ssm.a config.a api.a
SSM_LIBS_HOST   = $(addprefix $(HOST_BIN_DIR)/,$(SSM_LIBS))
SSM_LIBS_TARGET = $(addprefix $(TARGET_BIN_DIR)/,$(SSM_LIBS))

SSM_OBJS        = ISSMDeviceDriver.o SSMDeviceDriverMock.o SpillStateMachine.o
SSM_OBJS_PREFIX = $(addprefix ssm/,$(SSM_OBJS))
SSM_OBJS_HOST   = $(addprefix $(SSM_HOST_OUT)/,$(SSM_OBJS))
SSM_OBJS_TARGET = $(addprefix $(SSM_TARGET_OUT)/,$(SSM_OBJS))

SSM_TEST_OBJS   = AllTests.o SSMDeviceDriverMockTest.o SpillStateMachineTest.o ISSMDeviceDriverTest.o MockLinearDataTest.o
SSM_TEST_OBJS_PREFIX = $(addprefix ssm/,$(SSM_TEST_OBJS))
SSM_TEST_OBJS_HOST = $(addprefix $(SSM_HOST_OUT)/,$(SSM_TEST_OBJS))

ALL_COVERAGE    += $(SSM_OBJS_HOST)

$(SSM_TARGET_OUT):
	$(EES_OUT) mkdir -p $(SSM_TARGET_OUT)

$(SSM_HOST_OUT):
	$(EES_OUT) mkdir -p $(SSM_HOST_OUT)

ssm_clean:
	$(EES_OUT) rm -f ssm/*.o

# Host ssm library
$(HOST_BIN_DIR)/ssm.a: $(SSM_OBJS_PREFIX)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_AR) ru $@ $(SSM_OBJS_HOST)
	$(EES_OUT) $(HOST_RANLIB) $@

# Target ssm library
$(TARGET_BIN_DIR)/ssm.a: $(SSM_OBJS_PREFIX)
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(AR) ru $@ $(SSM_OBJS_TARGET)
	$(EES_OUT) $(RANLIB) $@

# Unit test suite
ssm_tests: $(SSM_LIBS_HOST) $(SSM_TEST_OBJS_PREFIX)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(SSM_HOST_OUT)/ssm_tests \
		$(SSM_TEST_OBJS_HOST) \
		$(SSM_LIBS_HOST) \
		$(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(SSM_HOST_OUT)/ssm_tests $(TEST_RUN_FLAGS)
