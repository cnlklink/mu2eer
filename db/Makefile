# Show targets
help:
	@echo "make help		=> Display description of targets"
	@echo "make clean		=> Remove temporary and .lis files"
	@echo "make db_diff		=> Calculate a diff between .dabbel files and what's in the device database"
	@echo "make db_sync		=> Synchronize .dabbel files with what's in the device database"
	@echo "                      	   !! WARNING !! this will over-write your .dabbel files"
	@echo "make db_test_check 	=> Run devices_test.dabbel in syntax-checking mode"
	@echo "make db_test		=> Submit devices_test.dabbel to DABBEL"

# Remove temporary and listing files
clean:
	@echo "-m-> Cleaning... "
	@-rm dbtemp *.lis

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
