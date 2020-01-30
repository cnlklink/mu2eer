SSM_HOST_OUT   = $(HOST_BIN_DIR)/ssm
SSM_TARGET_OUT = $(TARGET_BIN_DIR)/ssm

ALL_CLEAN       += ssm_clean
ALL_TARGETS     +=
ALL_TEST        += ssm_tests
ALL_OUT         += $(SSM_HOST_OUT) $(SSM_TARGET_OUT)
ALL_SOURCES     += $(wildcard ssm/*.C)
ALL_HEADERS     += $(wildcard ssm/*.H)

$(SSM_TARGET_OUT):
	$(EES_OUT) mkdir -p $(SSM_TARGET_OUT)

$(SSM_HOST_OUT): 
	$(EES_OUT) mkdir -p $(SSM_HOST_OUT)

ssm_clean:
	$(EES_OUT) rm -f ssm/*.o

ssm_tests: $(BIN_DIR) ssm/ISSMDeviceDriver.o ssm/AllTests.o ssm/SSMDeviceDriverMockTest.o
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(SSM_HOST_OUT)/ssm_tests \
		$(SSM_HOST_OUT)/ISSMDeviceDriver.o \
		$(SSM_HOST_OUT)/AllTests.o \
		$(SSM_HOST_OUT)/SSMDeviceDriverMockTest.o \
		$(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(SSM_HOST_OUT)/ssm_tests
