# Show targets
help:
	@echo "make help		=> Display description of targets"
	@echo "make clean		=> Remove temporary and .lis files"
	@echo "make db_diff		=> Calculate a diff between .dabbel files and what's in the device database"
	@echo "make db_lint             => Check that all devices assigned to Mu2eER nodes are represented in .dabbel files"
	@echo "make db_sync		=> Synchronize .dabbel files with what's in the device database"
	@echo "                      	   !! WARNING !! this will over-write your .dabbel files"
	@echo "make db_test		=> Submit devices_test.dabbel to DABBEL"
	@echo "make db_test_check 	=> Run devices_test.dabbel in syntax-checking mode"

# Remove temporary and listing files
clean:
	@echo "-m-> Cleaning... "
	@-rm dbtemp *.lis

# Ensure that every device assigned to the test/production node appears in a database script
db_lint: db_test_check
	@echo "-m-> Checking that all SRSD devices appear in devices_test.dabbel..."
	@acl "list node=srsd '%nm'" | awk '{if(NR>5)print}' >dbtemp
	@dev_missing=0; \
	input="dbtemp"; \
	while IFS= read -r line; \
	do \
		if ! grep -q "MOD $$line " devices_test.dabbel; then \
			echo "$$line was not found in devices_test.dabbel"; \
			((dev_missing=dev_missing+1)); \
		fi \
	done < "$$input"; \
	if [ "$$dev_missing" -gt 0 ]; then \
		echo "$$dev_missing device(s) not found."; \
		exit 1; \
	fi
	@rm dbtemp
	@echo "-m-> Done, all devices found."

# Show a diff between the device database files and what is in the database right now
db_diff:
	@echo "-m-> Showing difference between devices_test.dabbel and device database: "
	@acl "list node=srsd '%nm'" | awk '{if(NR>5)print "LIS " $$1}' >dbtemp
	@dabbel dbtemp modify 
	-diff devices_test.dabbel dbtemp.lis
	@rm dbtemp
	@rm dbtemp.lis

# Sync the dabbel scripts w/ the device database
db_sync:
	@echo "-m-> Updating devices_test.dabbel... "
	@acl "list node=srsd '%nm'" | awk '{if(NR>5)print "LIS " $$1}' >dbtemp
	@dabbel dbtemp modify
	@mv dbtemp.lis devices_test.dabbel
	@rm dbtemp
	@echo "-m-> Device database files have been synced."

# Run the test-environment database script in syntax-checking mode
db_test_check:
	@echo "-m-> Checking devices_test.dabbel... "
	@dabbel devices_test.dabbel syntax
	@rm devices_test.lis

# Submit the test-environment database script
db_test: db_test_check
	@echo "-m-> Submitting devices_test.dabbel... "
	@dabbel devices_test.dabbel modify
	@cat devices_test.lis
	@rm devices_test.lis
