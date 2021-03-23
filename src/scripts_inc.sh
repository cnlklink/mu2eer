################################################################################
#
# scripts_inc.sh
#
# Common functions shared between deploy.sh and build_select.sh.
#
# @author jdiamond
#
################################################################################

# Mu2eER Buildroot
BR_NAME=achilles_mu2eer

# Location of boot images on nova
BOOT_LOCATION=/fecode-bd/linux_boot/ees/srs

# Kernel image filename
IMAGE_FILE=zImage

# Device tree filename
DT_FILE="linuxDT.dtb"

# Rootfs filename
ROOTFS_FILE=

# Dabbel files
DABBEL_DIR=output/build/mu2eer-master/db
DABBEL_TEST_FILE=devices_test.dabbel
DABBEL_PROD_FILE=

STABLE_BUILD=/usr/local/products/buildroot/$BR_NAME

# do_remote
#
# Calls ssh to execute a remote command.
#
# @param $1 Message
# @param $2 Command
# @param $3 Remote host (default: nova)
#
do_remote()
{
    if [ "$3" == "" ]; then
        REMOTE_HOST="vxbuild1";
    else
        REMOTE_HOST=$3;
    fi

    printf "  $1... ";
    ssh -t $REMOTE_HOST "$2" 2>/tmp/ssh_stderr 1>/tmp/ssh_stdout
    if [ $? -eq 0 ]; then
        printf " success!\n";
    else
        printf " FAILED!\n";
        cat /tmp/ssh_stderr
        cat /tmp/ssh_stdout
        exit 1
    fi
}

# do_scp
#
# Calls scp, redirecting stderr and stdout to a temporary file.  If scp fails, 
# exit with a return status of 1.
#
# @param $1 Source file
# @param $2 Destination
#
do_scp()
{
    printf "  Copy $1 to $2...";
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

