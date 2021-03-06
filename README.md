# mu2eer
Mu2e Slow Extraction Regulator Front End software

## Introduction
This software runs on the Mu2e Slow Extraction Regulator ARM SoC.

## Building
To build Mu2eER, move into the src/ directory and run `make`.  This will build the mu2eerd, ACNET interfaces and the command line utility mu2eercli.  Binaries are built in the bin/ directory for the host and the target environment.

## Tests
Mu2eER has two test suites - a unit test suite and an acceptance test suite.

### Unit Tests
To run the Unit Test suite, move into the src/ directory and run `make tests`.  If all of the tests are successful, a coverage report is generated at the end - the last line indicates the total coverage of all the tests.

### Acceptance Tests
To run the Acceptance Test suite, move into the root directory and run `cucumber`.

Tags can be used to control which tests are run.  For example, acceptance tests that test the ACNET interfaces have an @acnet tag.  These tests must be run from a console environment with ACL installed (e.g. a clx node).  To only run the ACNET tests, use the `--tags` argument and supply it the @acnet tag:

`cucumber --tags @acnet`

If you're on adlinux, where ACNET and ACL are not available you can run all tests except the ACNET interfaces by specifying:

`cucumber --tags 'not @acnet'`

Tags can be combined with `and`.  For example, the `@long` tag is used for tests that take more than a few seconds.  If you want to run all of the ACNET tests but not the really long ones, use:

`cucumber --tags '@acnet and not @long'`

## Deployment
A defconfig for Mu2eER is in the ees-buildroot project (2015.08.x branch).  A Jenkins task called Mu2eER Buildroot builds this defconfig whenever the Mu2eER has a successful build or the Achilles Baseline has a successful build.

Two scripts handle the deployment of the target images that are produced by Mu2eER Buildroot.  The first script, `src/deploy.sh` will copy the target images to the `/fecode-bd` NFS share on `chablis` into a directory named for the Jenkins build number.  The second script, `src/build_select.sh` will enable the build for the given environment (test or production).

### Test Stand
The test-stand node is srsd.fnal.gov and is located in the TGC-100 Instrumentation lab area.  The board is connected to adinst130707.fnal.gov via USB-serial on COM7 at 115200 baud.  

A remote power switch is attached to COM1 at 9600 baud.  This can be used to remotely power-cycle the test stand.  Connect to the power switch with PuTTY and execute `pshow` to display power port status.  Use `pset 1 0` to turn power off and `pset 1 1` to turn power on.

### Production
The production node is to be determined.

### U-Boot Boot Script & Boot Parameters
The boot parameters are stored in `u-boot.scr` on the target's mmcblk0p1 partition (mounted at `/rfs`).  This file is actually a script run by u-boot during it's boot process.  It's responsible for configuring the network interface, tftp'ing the boot images into memory and then passing boot parameters to the Linux kernel.

`u-boot.scr` is a binary file generated with `mkimage`.  The script themselves are versioned controlled in the ees-buildroot project and the .scr files are generated by a post-build script (`board/fermi/achilles_mu2eer/bootscripts-post_build.sh`) as a part of the `fermi_achilles_mu2eer` build.  See `board/fermi/achilles_mu2eer/` for the text-based script files (\*.script).  Only the test target is handled right now.  When provisioning a new target, a new .script file should be prepared and a new mkimage step added to `bootscripts-post_build.sh`.  

After Buildroot completes a build of the `fermi_achilles_mu2eer` defconfig the u-boot script are placed in `output/images/\*.scr`.  If they change, they will need to be manually copied to the target.

### ACNET Device Database Scripts
DABBEL scripts for test and production environment ACNET devices are in the db/ directory.  A Makefile is provided to assist with maintaining and running these scripts.  To execute the scripts one must be on a development host with `acl` and `dabbel` commands available (e.g. a clx node).

To submit the test environment script use `make db_test`.  The script will be executed via `dabbel` in syntax-checking mode first, and if successful it will be submitted in modify mode.  To execute the script in syntax-checking mode only use `make db_test_check`.

If you create a new device with D80 (or modify an existing one) you can sync your copy of the database script with `make db_sync`.  This command uses `acl` to list all of the devices assigned to the test node `srsd` and then uses the DABBEL `lis` command to generate a `mod` script for each device.  To see a diff between your script and what would be generated by running `make db_sync` use the `make db_diff` command.

To see a summary of targets in `db/Makefile` run `make help`.

## Operation

### Daemon: mu2eerd
To start mu2eerd:
`/etc/init.d/S78mu2eerd start`

To stop mu2eerd:
`/etc/init.d/S78mu2eerd stop`

### Command-line interface: mu2eercli

#### pid Command
Displays the PID for the mu2eerd daemon process.
Usage:
`$ mu2eerd pid`

#### show Command
Displays diagnostic information for mu2eerd.
Usage:
`$ mu2eerd show`

#### shutdown Command
Requests that the mu2eerd daemon shut down.
Usage:
`$ mu2eerd shutdown`

### ACNET interfaces
