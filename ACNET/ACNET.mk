ALL_CLEAN += acnet_clean
ALL_TARGETS +=
ALL_TEST += acnet_test

acnet: acnet_adc

acnet_adc: ACNET/ADCDevice.o adc/IADCDriver.o
	$(CXX) ACNET/ADCDevice.o adc/IADCDriver.o -o ACNET/acnet_adc $(EES_LDFLAGS) $(DEV_OBJS) -L{$MYLIBS} $(DEV_LIBS) $(TEST_FLAGS)

acnet_clean:
	-rm -f ACNET/adc_acsys
	-rm -f ACNET/tests

acnet_test: ACNET/ADCDevice.o ACNET/ADCDeviceTest.o ACNET/AllTests.o adc/IADCDriver.o adc/ADCDriverStub.o
	$(CXX) ACNET/ADCDevice.o ACNET/ADCDeviceTest.o ACNET/AllTests.o adc/IADCDriver.o adc/ADCDriverStub.o -o ACNET/tests $(EES_LDFLAGS) $(DEV_OBJS) -L{$MYLIBS} $(DEV_LIBS) $(TEST_FLAGS)
	./ACNET/tests
