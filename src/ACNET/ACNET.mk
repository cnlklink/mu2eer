ACNET_HOST_OUT   = $(HOST_BIN_DIR)/ACNET
ACNET_TARGET_OUT = $(TARGET_BIN_DIR)/ACNET

ALL_CLEAN        += acnet_clean
ALL_TARGETS      += acnet
ALL_TEST         += acnet_tests
ALL_OUT          += $(ACNET_HOST_OUT) $(ACNET_TARGET_OUT)
ALL_SOURCES      += $(wildcard ACNET/*.C)
ALL_HEADERS      += $(wildcard ACNET/*.H)

$(ACNET_TARGET_OUT):
	$(EES_OUT) mkdir -p $(ACNET_TARGET_OUT)

$(ACNET_HOST_OUT):
	$(EES_OUT) mkdir -p $(ACNET_HOST_OUT)

acnet_clean:
	$(EES_OUT) -rm -f ACNET/*.o

acnet: acnet_adc

acnet_adc: ACNET/ADCDevice.o adc/IADCDriver.o adc/ADCDriverStub.o ACNET/adc_fef_init.o
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) -o $(ACNET_TARGET_OUT)/acnet_adc \
		$(EES_ERL_LIBS)/cdev-1.2/priv/fef_driver_lib.o \
		$(ADC_TARGET_OUT)/ADCDriverStub.o \
		$(ACNET_TARGET_OUT)/adc_fef_init.o \
		$(ACNET_TARGET_OUT)/ADCDevice.o \
		$(ADC_TARGET_OUT)/IADCDriver.o \
		$(EES_LDFLAGS) -L$(MYLIBS) $(DEV_LIBS) -lerl_interface -lei

acnet_tests: $(BIN_DIR) ACNET/ADCDevice.o ACNET/ADCDeviceTest.o ACNET/AllTests.o adc/IADCDriver.o adc/ADCDriverStub.o
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(ACNET_HOST_OUT)/acnet_tests \
		$(ACNET_HOST_OUT)/ADCDevice.o \
		$(ACNET_HOST_OUT)/ADCDeviceTest.o \
		$(ACNET_HOST_OUT)/AllTests.o \
		$(ADC_HOST_OUT)/IADCDriver.o \
		$(ADC_HOST_OUT)/ADCDriverStub.o \
		-L{$MYLIBS} $(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(ACNET_HOST_OUT)/acnet_tests
