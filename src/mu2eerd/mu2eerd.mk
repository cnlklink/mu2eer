MU2EERD_HOST_OUT   = $(HOST_BIN_DIR)/mu2eerd
MU2EERD_TARGET_OUT = $(TARGET_BIN_DIR)/mu2eerd

ALL_CLEAN       += mu2eerd_clean
ALL_TARGETS     += mu2eerd
ALL_TEST        += 
ALL_OUT         += $(MU2EERD_HOST_OUT) $(MU2EERD_TARGET_OUT)
ALL_SOURCES     += $(wildcard mu2eerd/*.C)
ALL_HEADERS     += $(wildcard mu2eerd/*.H)

$(MU2EERD_TARGET_OUT):
	$(EES_OUT) mkdir -p $(MU2EERD_TARGET_OUT)

$(MU2EERD_HOST_OUT): 
	$(EES_OUT) mkdir -p $(MU2EERD_HOST_OUT)

mu2eerd_clean:
	$(EES_OUT) rm -f mu2eerd/*.o

mu2eerd: mu2eerd/main.o
	@echo "-m-> Linking $@ (target)..."
	$(EES_OUT) $(CXX) -o $(MU2EERD_TARGET_OUT)/mu2eerd \
		$(MU2EERD_TARGET_OUT)/main.o \
		$(EES_LDFLAGS) -L$(MYLIBS) $(DEV_LIBS)
	@echo "-m-> Linking $@ (host)..."
	$(EES_OUT) $(HOST_CXX) -o $(MU2EERD_HOST_OUT)/mu2eerd \
		$(MU2EERD_HOST_OUT)/main.o \
		$(EES_LDFLAGS) -L$(MYLIBS) $(DEV_LIBS)
