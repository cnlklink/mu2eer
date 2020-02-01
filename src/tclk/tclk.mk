TCLK_HOST_OUT   = $(HOST_BIN_DIR)/tclk
TCLK_TARGET_OUT = $(TARGET_BIN_DIR)/tclk

ALL_CLEAN       += tclk_clean
ALL_TARGETS     +=
ALL_TEST        += tclk_tests
ALL_OUT         += $(TCLK_HOST_OUT) $(TCLK_TARGET_OUT)
ALL_SOURCES     += $(wildcard tclk/*.C)
ALL_HEADERS     += $(wildcard tclk/*.H)

TCLK_OBJS        = ITCLKDecoderDriver.o TCLKDecoderDriverMock.o 
TCLK_OBJS_PREFIX = $(addprefix tclk/,$(TCLK_OBJS))
TCLK_OBJS_HOST   = $(addprefix $(TCLK_HOST_OUT)/,$(TCLK_OBJS))
TCLK_OBJS_TARGET = $(addprefix $(TCLK_TARGET_OUT)/,$(TCLK_OBJS))

TCLK_TEST_OBJS   = AllTests.o TCLKDecoderDriverMockTest.o
TCLK_TEST_OBJS_PREFIX = $(addprefix tclk/,$(TCLK_TEST_OBJS))
TCLK_TEST_OBJS_HOST = $(addprefix $(TCLK_HOST_OUT)/,$(TCLK_TEST_OBJS))

$(TCLK_TARGET_OUT):
	$(EES_OUT) mkdir -p $(TCLK_TARGET_OUT)

$(TCLK_HOST_OUT): 
	$(EES_OUT) mkdir -p $(TCLK_HOST_OUT)

tclk_clean:
	$(EES_OUT) rm -f tclk/*.o

tclk_tests: $(TCLK_OBJS_PREFIX) $(TCLK_TEST_OBJS_PREFIX)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(TCLK_HOST_OUT)/tclk_tests \
		$(TCLK_OBJS_HOST) \
		$(TCLK_TEST_OBJS_HOST) \
		$(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(TCLK_HOST_OUT)/tclk_tests
	@echo "-m-> Coverage report for $(TCLK_OBJS)..."
	$(EES_OUT) $(HOST_GCOV) $(TCLK_OBJS_HOST)
