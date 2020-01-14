ALL_CLEAN   += adc_clean
ALL_TARGETS +=
ALL_TEST    += adc_test

adc: adc/ADCDriverStub.o
	$(CXX) adc/ADCDriverStub.o $(EES_LDFLAGS) $(DEV_OBJS) -L{$MYLIBS} $(DEV_LIBS)

adc_clean:
	-rm -f adc/*.o
	-rm -f adc/tests

adc_test: adc/ADCDriverStub.o adc/ADCDriverStubTest.o adc/AllTests.o adc/IADCDriver.o
	$(CXX) adc/IADCDriver.o adc/ADCDriverStub.o adc/ADCDriverStubTest.o adc/AllTests.o -o adc/tests $(EES_LDFLAGS) $(DEV_OBJS) -L{$MYLIBS} $(DEV_LIBS) $(TEST_FLAGS)
	./adc/tests
