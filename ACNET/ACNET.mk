ACNET_HOST_OUT   = output/host/ACNET
ACNET_TARGET_OUT = output/target/ACNET

ALL_CLEAN += acnet_clean
ALL_TARGETS += acnet
ALL_TEST += acnet_tests
ALL_OUT += $(ACNET_HOST_OUT) $(ACNET_TARGET_OUT)

$(ACNET_TARGET_OUT):
	$(EES_OUT) mkdir -p $(ACNET_TARGET_OUT)

$(ACNET_HOST_OUT):
	$(EES_OUT) mkdir -p $(ACNET_HOST_OUT)

acnet: acnet_adc

acnet_adc: ACNET/ADCDevice.o adc/IADCDriver.o adc/ADCDriverStub.o ACNET/adc_fef_init.o
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) $(EES_ERL_LIBS)/cdev-1.2/priv/fef_driver_lib.o output/target/adc/ADCDriverStub.o output/target/ACNET/adc_fef_init.o output/target/ACNET/ADCDevice.o output/target/adc/IADCDriver.o -o output/target/ACNET/acnet_adc $(EES_LDFLAGS) $(DEV_OBJS) -L$(MYLIBS) $(DEV_LIBS) -lerl_interface -lei

acnet_clean:
	$(EES_OUT) -rm -f ACNET/*.o

acnet_tests: output ACNET/ADCDevice.o ACNET/ADCDeviceTest.o ACNET/AllTests.o adc/IADCDriver.o adc/ADCDriverStub.o
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) output/host/ACNET/ADCDevice.o output/host/ACNET/ADCDeviceTest.o output/host/ACNET/AllTests.o output/host/adc/IADCDriver.o output/host/adc/ADCDriverStub.o -o output/host/ACNET/acnet_tests $(DEV_OBJS) -L{$MYLIBS} $(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(ACNET_HOST_OUT)/acnet_tests
