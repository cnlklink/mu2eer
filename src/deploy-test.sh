#!/bin/bash

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
printf "Copy $ZIMAGE_SOURCE to $ZIMAGE_DEST...";
scp $STABLE_BUILD/output/images/zImage nova:$IMAGE_LOCATION/zImage 2>/tmp/scp_stderr 1>/tmp/scp_stdout
printf " success!\n";

DT_SOURCE=$STABLE_BUILD/output/images/linuxDT.dtb
DT_DEST=nova:$IMAGE_LOCATION/linuxDT.dtb
printf "Copy $DT_SOURCE to $DT_DEST...";
scp $STABLE_BUILD/output/images/linuxDT.dtb nova:$IMAGE_LOCATION/linuxDT.dtb 2>/tmp/scp_stderr 1>/tmp/scp_stdout
printf " success!\n";

printf "Done!\n";
