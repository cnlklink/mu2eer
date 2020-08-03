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

## Deployment
A defconfig for Mu2eER is in the ees-buildroot project (2015.08.x branch).  A Jenkins task called Mu2eER Buildroot builds this defconfig whenever the Mu2eER has a successful build or the Achilles Baseline has a successful build.

Two scripts handle the deployment of the target images that are produced by Mu2eER Buildroot.  The first script, `src/deploy.sh` will copy the target images to the `/fecode-bd` NFS share on `chablis` into a directory named for the Jenkins build number.  The second script, `src/build_select.sh` will enable the build for the given environment (test or production).
