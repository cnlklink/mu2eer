ALL_CLEAN += tclk_clean

tclk_clean:
	$(EES_OUT) rm -f tclk/*.o
	$(EES_OUT) rm -f tclk/tests
