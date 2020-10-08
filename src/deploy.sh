#!/bin/bash
################################################################################
#
# deploy.sh
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
# Prints usage message.
#
usage()
{
    printf "Usage: $0\n"
}

JENKINS_BUILDNUM_FILE=$STABLE_BUILD/output/target/etc/jenkins_build_number
if [ ! -s "$JENKINS_BUILDNUM_FILE" ]; then
    printf "Jenkins build number file does not exist: $JENKINS_BUILDNUM_FILE.  Aborting!\n"
    exit 1;
fi
JENKINS_BUILDNUM=`cat $JENKINS_BUILDNUM_FILE`

IMAGE_LOCATION=$BOOT_LOCATION/$JENKINS_BUILDNUM

# Copy kernel+rootfs image and device tree to nova TFTP area...
printf "Deploying mu2eer build number $JENKINS_BUILDNUM...\n"
printf "  Boot location is $BOOT_LOCATION\n"

do_remote "Creating destination directory on nova", "mkdir -p $IMAGE_LOCATION"

if [ -n "$IMAGE_FILE" ]; then
    IMAGE_SOURCE=$STABLE_BUILD/output/images/$IMAGE_FILE
    IMAGE_DEST=nova:$IMAGE_LOCATION/$IMAGE_FILE
    do_scp $IMAGE_SOURCE $IMAGE_DEST
fi

if [ -n "$DT_FILE" ]; then
    DT_SOURCE=$STABLE_BUILD/output/images/$DT_FILE
    DT_DEST=nova:$IMAGE_LOCATION/$DT_FILE
    do_scp $DT_SOURCE $DT_DEST
fi

if [ -n "$ROOTFS_FILE" ]; then
    ROOTFS_SOURCE=$STABLE_BUILD/output/images/$ROOTFS_FILE
    ROOTFS_DEST=nova:$IMAGE_LOCATION/$ROOTFS_FILE
    do_scp $ROOTFS_SOURCE $ROOTFS_DEST
fi

if [ -n "$DABBEL_TEST_FILE" ]; then
    DABBEL_SOURCE=$STABLE_BUILD/$DABBEL_DIR/$DABBEL_TEST_FILE
    DABBEL_DEST=nova:$IMAGE_LOCATION/$DABBEL_TEST_FILE
    do_scp $DABBEL_SOURCE $DABBEL_DEST
fi

if [ -n "$DABBEL_PROD_FILE" ]; then
    DABBEL_SOURCE=$STABLE_BUILD/$DABBEL_DIR/$DABBEL_PROD_FILE
    DABBEL_DEST=nova:$IMAGE_LOCATION/$DABBEL_PROD_FILE
    do_scp $DABBEL_SOURCE $DABBEL_DEST
fi

printf "Done!\n";
printf "Use './build_select.sh [test|production] $JENKINS_BUILDNUM' and reboot your nodes.\n";

