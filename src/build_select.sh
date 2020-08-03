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

do_remote "Removing existing $ENVIRONMENT link", "rm ${BOOT_LOCATION}/${ENVIRONMENT} ||true"
do_remote "Creating $ENVIRONMENT link", "cd ${BOOT_LOCATION}; ln -s $BUILD_NUM $ENVIRONMENT"

printf "Done!\n";
