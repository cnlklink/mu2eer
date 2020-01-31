ACNET_HOST_OUT   = $(HOST_BIN_DIR)/ACNET
ACNET_TARGET_OUT = $(TARGET_BIN_DIR)/ACNET

ALL_CLEAN        += acnet_clean
ALL_TARGETS      += acnet
ALL_TEST         += acnet_tests
ALL_OUT          += $(ACNET_HOST_OUT) $(ACNET_TARGET_OUT)
ALL_SOURCES      += $(wildcard ACNET/*.C)
ALL_HEADERS      += $(wildcard ACNET/*.H)

ACNET_ADC_OBJS        = ADCDevice.o adc_fef_init.o
ACNET_ADC_OBJS_PREFIX = $(addprefix ACNET/,$(ACNET_ADC_OBJS))
ACNET_ADC_OBJS_HOST   = $(addprefix $(ACNET_HOST_OUT)/,$(ACNET_ADC_OBJS))
ACNET_ADC_OBJS_TARGET = $(addprefix $(ACNET_TARGET_OUT)/,$(ACNET_ADC_OBJS))

ACNET_TEST_OBJS        = ADCDevice.o ADCDeviceTest.o AllTests.o 
ACNET_TEST_OBJS_PREFIX = $(addprefix ACNET/,$(ACNET_TEST_OBJS))
ACNET_TEST_OBJS_HOST   = $(addprefix $(ACNET_HOST_OUT)/,$(ACNET_TEST_OBJS))

$(ACNET_TARGET_OUT):
	$(EES_OUT) mkdir -p $(ACNET_TARGET_OUT)

$(ACNET_HOST_OUT):
	$(EES_OUT) mkdir -p $(ACNET_HOST_OUT)

acnet_clean:
	$(EES_OUT) -rm -f ACNET/*.o

acnet: acnet_adc

acnet_adc: $(TARGET_BIN_DIR)/adc.a $(ACNET_ADC_OBJS_PREFIX)
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) -o $(ACNET_TARGET_OUT)/acnet_adc \
		$(EES_ERL_LIBS)/cdev-1.2/priv/fef_driver_lib.o \
		$(ACNET_ADC_OBJS_TARGET) \
		$(TARGET_BIN_DIR)/adc.a \
		$(EES_LDFLAGS) $(DEV_LIBS) -L$(MYLIBS) -lerl_interface -lei

acnet_tests: $(BIN_DIR) $(ACNET_TEST_OBJS_PREFIX) $(ACNET_ADC_OBJS_PREFIX) $(HOST_BIN_DIR)/adc.a
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(ACNET_HOST_OUT)/acnet_tests \
		$(ACNET_TEST_OBJS_HOST) \
		$(HOST_BIN_DIR)/adc.a \
		$(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(ACNET_HOST_OUT)/acnet_tests
