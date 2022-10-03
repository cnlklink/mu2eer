ADC_HOST_OUT         = $(HOST_BIN_DIR)/adc
ADC_TARGET_OUT       = $(TARGET_BIN_DIR)/adc

ALL_CLEAN            += adc_clean
ALL_TARGETS          += $(HOST_BIN_DIR)/adc.a $(TARGET_BIN_DIR)/adc.a
ALL_TEST             += adc_tests
ALL_OUT              += $(ADC_HOST_OUT) $(ADC_TARGET_OUT)
ALL_SOURCES          += $(wildcard adc/*.C) 
ALL_HEADERS          += $(wildcard adc/*.H) 

ADC_OBJS             = ADCDriverStub.o IADCDriver.o
ADC_OBJS_PREFIX      = $(addprefix adc/,$(ADC_OBJS))
ADC_OBJS_HOST        = $(addprefix $(ADC_HOST_OUT)/,$(ADC_OBJS))
ADC_OBJS_TARGET      = $(addprefix $(ADC_TARGET_OUT)/,$(ADC_OBJS))

ADC_TEST_OBJS        = AllTests.o ADCDriverStubTests.o
ADC_TEST_OBJS_PREFIX = $(addprefix adc/,$(ADC_TEST_OBJS))
ADC_TEST_OBJS_HOST   = $(addprefix $(ADC_HOST_OUT)/,$(ADC_TEST_OBJS))

ALL_COVERAGE         += $(ADC_OBJS_HOST)

$(ADC_TARGET_OUT):
	$(EES_OUT) mkdir -p $(ADC_TARGET_OUT)

$(ADC_HOST_OUT):
	$(EES_OUT) mkdir -p $(ADC_HOST_OUT)

adc_clean:
	$(EES_OUT) -rm -f adc/*.o

# Host adc library
$(HOST_BIN_DIR)/adc.a: $(ADC_OBJS_PREFIX)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_AR) ru $@ $(ADC_OBJS_HOST)
	$(EES_OUT) $(HOST_RANLIB) $@

# Target adc library
$(TARGET_BIN_DIR)/adc.a: $(ADC_OBJS_PREFIX)
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(AR) ru $@ $(ADC_OBJS_TARGET)
	$(EES_OUT) $(RANLIB) $@

# Unit test suite
adc_tests: $(BIN_DIR) $(HOST_BIN_DIR)/adc.a $(ADC_TEST_OBJS_PREFIX)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(ADC_HOST_OUT)/adc_tests \
		$(ADC_TEST_OBJS_HOST) \
		$(HOST_BIN_DIR)/adc.a \
		$(DEV_LIBS) $(TEST_FLAGS) \
		-L$(HOST_LIB_DIR)
	@echo "-m-> Running $@..."
	@./$(ADC_HOST_OUT)/adc_tests $(TEST_RUN_FLAGS)
