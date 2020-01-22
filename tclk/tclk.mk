TCLK_HOST_OUT   = output/host/tclk
TCLK_TARGET_OUT = output/target/tclk

ALL_CLEAN   += tclk_clean
ALL_TARGETS +=
ALL_TEST    += tclk_tests
ALL_OUT     += $(TCLK_HOST_OUT) $(TCLK_TARGET_OUT)

$(TCLK_TARGET_OUT):
	$(EES_OUT) mkdir -p $(TCLK_TARGET_OUT)

$(TCLK_HOST_OUT): 
	$(EES_OUT) mkdir -p $(TCLK_HOST_OUT)

tclk_clean:
	$(EES_OUT) rm -f tclk/*.o

tclk_tests: output tclk/ITCLKDecoderDriver.o tclk/TCLKDecoderDriverMock.o tclk/AllTests.o tclk/TCLKDecoderDriverMockTest.o
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) $(TCLK_HOST_OUT)/ITCLKDecoderDriver.o $(TCLK_HOST_OUT)/TCLKDecoderDriverMock.o $(TCLK_HOST_OUT)/AllTests.o $(TCLK_HOST_OUT)/TCLKDecoderDriverMockTest.o -o $(TCLK_HOST_OUT)/tclk_tests $(DEV_OBJS) $(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(TCLK_HOST_OUT)/tclk_tests

