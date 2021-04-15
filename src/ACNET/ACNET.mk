ACNET_HOST_OUT   = $(HOST_BIN_DIR)/ACNET
ACNET_TARGET_OUT = $(TARGET_BIN_DIR)/ACNET

ALL_CLEAN        += acnet_clean
ALL_TARGETS      += acnet
ALL_TEST         += acnet_tests
ALL_OUT          += $(ACNET_HOST_OUT) $(ACNET_TARGET_OUT)
ALL_SOURCES      += $(wildcard ACNET/*.C)
ALL_HEADERS      += $(wildcard ACNET/*.H)

ACNET_LIBS        = api.a adc.a ssm.a config.a tclk.a
ACNET_LIBS_HOST   = $(addprefix $(HOST_BIN_DIR)/,$(ACNET_LIBS))
ACNET_LIBS_TARGET = $(addprefix $(TARGET_BIN_DIR)/,$(ACNET_LIBS))

ACNET_ADC_OBJS        = ADCDevice.o adc_fef_init.o
ACNET_ADC_OBJS_PREFIX = $(addprefix ACNET/,$(ACNET_ADC_OBJS))
ACNET_ADC_OBJS_HOST   = $(addprefix $(ACNET_HOST_OUT)/,$(ACNET_ADC_OBJS))
ACNET_ADC_OBJS_TARGET = $(addprefix $(ACNET_TARGET_OUT)/,$(ACNET_ADC_OBJS))

ACNET_MU2EERD_OBJS        = Mu2eerdDevice.o \
	mu2eerd_fef_init.o \
	DaemonController.o \
	SystemController.o \
	TestSystemController.o
ACNET_MU2EERD_OBJS_PREFIX = $(addprefix ACNET/,$(ACNET_MU2EERD_OBJS))
ACNET_MU2EERD_OBJS_HOST   = $(addprefix $(ACNET_HOST_OUT)/,$(ACNET_MU2EERD_OBJS))
ACNET_MU2EERD_OBJS_TARGET = $(addprefix $(ACNET_TARGET_OUT)/,$(ACNET_MU2EERD_OBJS))

ACNET_SSM_OBJS        = SSMDevice.o ssm_fef_init.o
ACNET_SSM_OBJS_PREFIX = $(addprefix ACNET/,$(ACNET_SSM_OBJS))
ACNET_SSM_OBJS_HOST   = $(addprefix $(ACNET_HOST_OUT)/,$(ACNET_SSM_OBJS))
ACNET_SSM_OBJS_TARGET = $(addprefix $(ACNET_TARGET_OUT)/,$(ACNET_SSM_OBJS))

ACNET_TCLK_OBJS        = TCLKDevice.o tclk_fef_init.o
ACNET_TCLK_OBJS_PREFIX = $(addprefix ACNET/,$(ACNET_TCLK_OBJS))
ACNET_TCLK_OBJS_HOST   = $(addprefix $(ACNET_HOST_OUT)/,$(ACNET_TCLK_OBJS))
ACNET_TCLK_OBJS_TARGET = $(addprefix $(ACNET_TARGET_OUT)/,$(ACNET_TCLK_OBJS))

ACNET_TEST_OBJS        = ADCDevice.o \
	ADCDeviceTest.o \
	DaemonController.o \
	Mu2eerdDevice.o \
	Mu2eerdDeviceTest.o \
	SSMDevice.o \
	SSMDeviceTests.o \
	SystemController.o \
	TCLKDevice.o \
	TCLKDeviceTests.o \
	TestSystemController.o \
	AllTests.o 
ACNET_TEST_OBJS_PREFIX = $(addprefix ACNET/,$(ACNET_TEST_OBJS))
ACNET_TEST_OBJS_HOST   = $(addprefix $(ACNET_HOST_OUT)/,$(ACNET_TEST_OBJS))

ALL_COVERAGE += $(ACNET_ADC_OBJS_HOST) $(ACNET_MU2EERD_OBJS_HOST) $(ACNET_SSM_OBJS_HOST)

$(ACNET_TARGET_OUT):
	$(EES_OUT) mkdir -p $(ACNET_TARGET_OUT)

$(ACNET_HOST_OUT):
	$(EES_OUT) mkdir -p $(ACNET_HOST_OUT)

acnet_clean:
	$(EES_OUT) -rm -f ACNET/*.o

acnet: acnet_adc acnet_mu2eerd acnet_ssm acnet_tclk

acnet_adc: $(ACNET_LIBS_TARGET) $(ACNET_ADC_OBJS_PREFIX) 
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) -o $(ACNET_TARGET_OUT)/acnet_adc \
		$(EES_ERL_LIBS)/cdev-$(EES_CDEV_VERSION)/priv/fef_driver_lib.o \
		$(ACNET_ADC_OBJS_TARGET) \
		$(ACNET_LIBS_TARGET) \
		$(EES_LDFLAGS) $(DEV_LIBS) -L$(MYLIBS) -lerl_interface -lei

acnet_mu2eerd: $(ACNET_LIBS_TARGET) $(ACNET_MU2EERD_OBJS_PREFIX) 
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) -o $(ACNET_TARGET_OUT)/acnet_mu2eerd \
		$(EES_ERL_LIBS)/cdev-$(EES_CDEV_VERSION)/priv/fef_driver_lib.o \
		$(ACNET_MU2EERD_OBJS_TARGET) \
		$(ACNET_LIBS_TARGET) \
		$(EES_LDFLAGS) $(DEV_LIBS) -L$(MYLIBS) -lerl_interface -lei

acnet_ssm: $(ACNET_LIBS_TARGET) $(ACNET_SSM_OBJS_PREFIX) 
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) -o $(ACNET_TARGET_OUT)/acnet_ssm \
		$(EES_ERL_LIBS)/cdev-$(EES_CDEV_VERSION)/priv/fef_driver_lib.o \
		$(ACNET_SSM_OBJS_TARGET) \
		$(ACNET_LIBS_TARGET) \
		$(EES_LDFLAGS) $(DEV_LIBS) -L$(MYLIBS) -lerl_interface -lei

acnet_tclk: $(ACNET_LIBS_TARGET) $(ACNET_TCLK_OBJS_PREFIX) 
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) -o $(ACNET_TARGET_OUT)/acnet_tclk \
		$(EES_ERL_LIBS)/cdev-$(EES_CDEV_VERSION)/priv/fef_driver_lib.o \
		$(ACNET_TCLK_OBJS_TARGET) \
		$(ACNET_LIBS_TARGET) \
		$(EES_LDFLAGS) $(DEV_LIBS) -L$(MYLIBS) -lerl_interface -lei

acnet_tests: $(BIN_DIR) $(ACNET_TEST_OBJS_PREFIX) $(ACNET_ADC_OBJS_PREFIX) $(ACNET_LIBS_HOST) mu2eerd/Controller.o
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(ACNET_HOST_OUT)/acnet_tests \
		$(ACNET_TEST_OBJS_HOST) \
		$(HOST_BIN_DIR)/mu2eerd/Controller.o \
		$(ACNET_LIBS_HOST) \
		$(DEV_LIBS) $(TEST_FLAGS)
	@echo "-m-> Running $@..."
	@./$(ACNET_HOST_OUT)/acnet_tests $(TEST_RUN_FLAGS)

