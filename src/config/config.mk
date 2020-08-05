CONFIG_HOST_OUT   = $(HOST_BIN_DIR)/config
CONFIG_TARGET_OUT = $(TARGET_BIN_DIR)/config

ALL_CLEAN       += config_clean
ALL_TARGETS     += $(HOST_BIN_DIR)/config.a $(TARGET_BIN_DIR)/config.a
ALL_TEST        += config_tests
ALL_OUT         += $(CONFIG_HOST_OUT) $(CONFIG_TARGET_OUT)
ALL_SOURCES     += $(wildcard config/*.C)
ALL_HEADERS     += $(wildcard config/*.H)

CONFIG_OBJS        = ConfigurationManager.o SSMConfig.o
CONFIG_OBJS_PREFIX = $(addprefix config/,$(CONFIG_OBJS))
CONFIG_OBJS_HOST   = $(addprefix $(CONFIG_HOST_OUT)/,$(CONFIG_OBJS))
CONFIG_OBJS_TARGET = $(addprefix $(CONFIG_TARGET_OUT)/,$(CONFIG_OBJS))

CONFIG_TEST_OBJS   = AllTests.o ConfigurationManagerTest.o
CONFIG_TEST_OBJS_PREFIX = $(addprefix config/,$(CONFIG_TEST_OBJS))
CONFIG_TEST_OBJS_HOST = $(addprefix $(CONFIG_HOST_OUT)/,$(CONFIG_TEST_OBJS))

ALL_COVERAGE    += $(CONFIG_OBJS_HOST)

$(CONFIG_TARGET_OUT):
	$(EES_OUT) mkdir -p $(CONFIG_TARGET_OUT)

$(CONFIG_HOST_OUT): 
	$(EES_OUT) mkdir -p $(CONFIG_HOST_OUT)

config_clean:
	$(EES_OUT) rm -f config/*.o

# Host config library
$(HOST_BIN_DIR)/config.a: $(CONFIG_OBJS_PREFIX)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_AR) ru $@ $(CONFIG_OBJS_HOST)
	$(EES_OUT) $(HOST_RANLIB) $@

# Target config library
$(TARGET_BIN_DIR)/config.a: $(CONFIG_OBJS_PREFIX)
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(AR) ru $@ $(CONFIG_OBJS_TARGET)
	$(EES_OUT) $(RANLIB) $@

# Unit test suite
config_tests: $(HOST_BIN_DIR)/config.a $(CONFIG_TEST_OBJS_PREFIX)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(CONFIG_HOST_OUT)/config_tests \
		$(CONFIG_TEST_OBJS_HOST) \
		$(HOST_BIN_DIR)/config.a \
		$(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(CONFIG_HOST_OUT)/config_tests
