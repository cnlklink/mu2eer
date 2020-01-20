ALL_CLEAN   += adc_clean
ALL_TARGETS += adc
ALL_TEST    += adc_tests

adc: adc/ADCDriverStub.o

adc_clean:
	$(EES_OUT) -rm -f adc/*.o
	$(EES_OUT) -rm -f output/host/adc/*.o
	$(EES_OUT) -rm -f output/target/adc/*.o

adc_tests: adc/ADCDriverStub.o adc/ADCDriverStubTest.o adc/AllTests.o adc/IADCDriver.o
	$(HOST_CXX) output/host/adc/IADCDriver.o output/host/adc/ADCDriverStub.o output/host/adc/ADCDriverStubTest.o output/host/adc/AllTests.o -o output/host/adc/adc_tests $(EES_LDFLAGS) $(DEV_OBJS) -L$(MYLIBS) $(DEV_LIBS) $(TEST_FLAGS)
	./output/host/adc/adc_tests
