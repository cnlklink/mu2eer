#!/bin/bash
################################################################################
#
# build_select.sh
#
# Mu2eER Deployment Script.  Automates the installation of the Mu2eER Buildroot
# images produced by Jenkins to the test and production boot areas.
#
# @author jdiamond
#
################################################################################

. scripts_inc.sh

# usage
#
# Display usage information.
#
usage()
{
    printf "Usage: $0 [test|production] [jenkins build number]\n"
    printf "Displaying boot directory for Mu2eER...\n"
    ssh -t nova "ls -l $BOOT_LOCATION" 2>/tmp/ssh_stderr
}

ENVIRONMENT=$1
BUILD_NUM=$2

if [ -z "$BUILD_NUM" ]; then
    usage
    exit 1
fi

case $ENVIRONMENT in

    test | production)
        printf "Setting $ENVIRONMENT to build #$BUILD_NUM...\n"
        ;;

    "")
        usage
        exit 1
        ;;

    *)
        printf "Unrecognized environment: $ENVIRONMENT.  Use test or production.\n"
        exit 1
        ;;

esac

do_remote "Removing existing $ENVIRONMENT link" "rm ${BOOT_LOCATION}/${ENVIRONMENT} ||true"
do_remote "Creating $ENVIRONMENT link" "cd ${BOOT_LOCATION}; ln -s $BUILD_NUM $ENVIRONMENT"
do_scp "nova:${BOOT_LOCATION}/${BUILD_NUM}/devices_${ENVIRONMENT}.dabbel" ".dabbel_temp"
do_scp ".dabbel_temp" "clx50:~/"
do_remote "Validating ${DABBEL_TEST_FILE}" "dabbel ~/.dabbel_temp" "clx50"
do_remote "Submitting ${DABBEL_TEST_FILE} to DABBEL" "dabbel ~/.dabbel_temp modify" "clx50"
rm .dabbel_temp

printf "Done!\n";
