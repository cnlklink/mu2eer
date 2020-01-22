ADC_HOST_OUT   = output/host/adc
ADC_TARGET_OUT = output/target/adc

ALL_CLEAN   += adc_clean
ALL_TARGETS += adc
ALL_TEST    += adc_tests
ALL_OUT     += $(ADC_HOST_OUT) $(ADC_TARGET_OUT)

$(ADC_TARGET_OUT):
	$(EES_OUT) mkdir -p $(ADC_TARGET_OUT)

$(ADC_HOST_OUT):
	$(EES_OUT) mkdir -p $(ADC_HOST_OUT)

adc: adc/ADCDriverStub.o

adc_clean:
	$(EES_OUT) -rm -f adc/*.o

adc_tests: output adc/ADCDriverStub.o adc/ADCDriverStubTest.o adc/AllTests.o adc/IADCDriver.o
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) output/host/adc/IADCDriver.o output/host/adc/ADCDriverStub.o output/host/adc/ADCDriverStubTest.o output/host/adc/AllTests.o -o output/host/adc/adc_tests $(DEV_OBJS) -L$(MYLIBS) $(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(ADC_HOST_OUT)/adc_tests
