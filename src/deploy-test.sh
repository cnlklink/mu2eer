#!/bin/bash

# do_scp
#
# Calls scp, redirecting stderr and stdout to a temporary file.  If scp fails, exit with
# a return status of 1.
#
# @param $1 Source file
# @param $2 Destination
#
do_scp()
{
    printf "Copy $1 to $2...";
    scp $1 $2 2>/tmp/scp_stderr 1>/tmp/scp_stdout
    if [ $? -eq 0 ] 
    then
        printf " success!\n";
    else
        printf " FAILED!\n"
        cat /tmp/scp_stderr
        exit 1;
    fi
}

# Buildroot build location
STABLE_BUILD=/usr/local/products/buildroot/achilles_mu2eer

# Test node
TEST_NODE=srsd

# Local of boot images on nova
IMAGE_LOCATION=/fecode-bd/linux_boot/ees/srs

# Copy kernel+rootfs image and device tree to nova TFTP area...
printf "Deploying mu2eerd_buildroot build #?? to $TEST_NODE...\n";

ZIMAGE_SOURCE=$STABLE_BUILD/output/images/zImage
ZIMAGE_DEST=nova:$IMAGE_LOCATION/zImage
do_scp $ZIMAGE_SOURCE $ZIMAGE_DEST

DT_SOURCE=$STABLE_BUILD/output/images/linuxDT.dtb
DT_DEST=nova:$IMAGE_LOCATION/linuxDT.dtb
do_scp $DT_SOURCE $DT_DEST

printf "Done!\n";
