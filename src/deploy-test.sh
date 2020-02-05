#!/bin/bash

# Buildroot build location
STABLE_BUILD=/usr/local/products/buildroot/achilles_mu2eer

# Test node
TEST_NODE=srsd

# Local of boot images on nova
IMAGE_LOCATION=/fecode-bd/linux_boot/ees/srs

# Copy kernel+rootfs image and device tree to nova TFTP area...
echo "Deploying mu2eerd_buildroot build #?? to $TEST_NODE...";

scp $STABLE_BUILD/output/images/zImage nova:$IMAGE_LOCATION/zImage
scp $STABLE_BUILD/output/images/linuxDT.dtb nova:$IMAGE_LOCATION/linuxDT.dtb

echo "Done!";
