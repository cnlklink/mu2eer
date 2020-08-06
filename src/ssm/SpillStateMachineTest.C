/**
 * SpillStateMachineTest.C
 *
 * This file contains unit tests (CppUTest) for the SpillStateMachine class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "SharedMemoryManager.H"
#include "SpillStateMachine.H"

using namespace Mu2eER;
using namespace std;

/**
 * Use a default configuration for testing
 */
static ConfigurationManager _cm;

/**
 * Shared memory manager for testing
 */
static SharedMemoryManager _shmm( "mu2eer_test" );

/**
 * A global SpillStateMachine object used for all tests
 */
static SpillStateMachine* _ssm;

/**
 * Initialization Group
 *
 * Tests related to initializing and tearing-down the SSM module.
 */
TEST_GROUP( InitGroup )
{
  void setup()
  {
    _ssm = new SpillStateMachine( _cm, _shmm.ssmBlockGet() );
  }

  void teardown()
  {
    delete _ssm;
  }
};

/**
 * Test Initialization
 *
 * Verify that we can initializae the SSM module with a basic configuration
 */
TEST( InitGroup, Initialize )
{
  CHECK_EQUAL( SSM_IDLE, _ssm->stateGet() );

  _ssm->initialize();

  CHECK_EQUAL( SSM_BETWEEN_CYCLES, _ssm->stateGet() );

  CHECK_EQUAL( SSM_BETWEEN_CYCLES, _shmm.ssmBlockGet().currentStateGet() );
}

/**
 * Spill Counter Group
 *
 * Tests related to the operation of the spill counter.
 */
TEST_GROUP( SpillCounterGroup )
{
  void setup()
  {
    _cm.ssmGet().mockSpillsSet( 5 );
    _ssm = new SpillStateMachine( _cm, _shmm.ssmBlockGet() );
  }

  void teardown()
  {
    delete _ssm;
    _cm.ssmGet().mockSpillsSet( 0 );
  }
};

/**
 * Test Initial Spill Counter
 *
 * Test that the spill counter is 0 after the spill state machine is initialized.
 */
TEST( SpillCounterGroup, InitialValueIsZero )
{
  _ssm->initialize();
  CHECK_EQUAL( 0, _ssm->spillCounterGet() );
}

/**
 * Test Initialize Returns Spill Counter to Zero
 *
 * Test that initialize() will return the spill counter to 0.
 */
TEST( SpillCounterGroup, ResetToZero )
{
  // Ask for 5 fake spills
  _ssm->initialize();
  CHECK_EQUAL( 0, _ssm->spillCounterGet() );

  // Run through the fake spills
  while( SSM_FAULT != _ssm->waitForStateChange() );
  CHECK_EQUAL( 5, _ssm->spillCounterGet() );

  // Re-initialize
  _ssm->initialize();
  CHECK_EQUAL( 0, _ssm->spillCounterGet() );
}
