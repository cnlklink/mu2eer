ALL_CLEAN   += adc_clean
ALL_TARGETS +=
ALL_TEST    += adc_test

adc: adc/ADCDriverStub.o

adc_clean:
	-rm -f adc/*.o
	-rm -f output/host/adc/*.o
	-rm -f output/target/adc/*.o

adc_test: adc/ADCDriverStub.o adc/ADCDriverStubTest.o adc/AllTests.o adc/IADCDriver.o
	$(HOST_CXX) output/host/adc/IADCDriver.o output/host/adc/ADCDriverStub.o output/host/adc/ADCDriverStubTest.o output/host/adc/AllTests.o -o output/host/adc/tests $(EES_LDFLAGS) $(DEV_OBJS) -L$(MYLIBS) $(DEV_LIBS) $(TEST_FLAGS)
	./output/host/adc/tests
