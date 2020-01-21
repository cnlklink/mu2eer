ALL_CLEAN += acnet_clean
ALL_TARGETS += acnet
ALL_TEST += acnet_tests

acnet: acnet_adc

acnet_adc: ACNET/ADCDevice.o adc/IADCDriver.o adc/ADCDriverStub.o ACNET/adc_fef_init.o
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) $(EES_ERL_LIBS)/cdev-1.2/priv/fef_driver_lib.o output/target/adc/ADCDriverStub.o output/target/ACNET/adc_fef_init.o output/target/ACNET/ADCDevice.o output/target/adc/IADCDriver.o -o output/target/ACNET/acnet_adc $(EES_LDFLAGS) $(DEV_OBJS) -L$(MYLIBS) $(DEV_LIBS) -lerl_interface -lei

acnet_clean:
	$(EES_OUT) -rm -f ACNET/*.o
	$(EES_OUT) -rm -f output/host/ACNET/*.o
	$(EES_OUT) -rm -f output/target/ACNET/*.o
	$(EES_OUT) -rm -f output/host/ACNET/adc_acsys
	$(EES_OUT) -rm -f output/target/ACNET/adc_acsys
	$(EES_OUT) -rm -f output/host/ACNET/tests
	$(EES_OUT) -rm -f output/target/ACNET/tests

acnet_tests: ACNET/ADCDevice.o ACNET/ADCDeviceTest.o ACNET/AllTests.o adc/IADCDriver.o adc/ADCDriverStub.o
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) output/host/ACNET/ADCDevice.o output/host/ACNET/ADCDeviceTest.o output/host/ACNET/AllTests.o output/host/adc/IADCDriver.o output/host/adc/ADCDriverStub.o -o output/host/ACNET/acnet_tests $(DEV_OBJS) -L{$MYLIBS} $(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./output/host/ACNET/acnet_tests
