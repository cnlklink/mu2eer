/**
 * SSMDeviceDriverMockTest.C
 *
 * This file contains unit tests (CppUTest) for the SSMDeviceDriverMock class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "ConfigurationManager.H"
#include "SSMDeviceDriverMock.H"

using namespace Mu2eER;
using namespace std;

#include <sstream>

/**
 * A global SSMDeviceDriverMock object used for all tests
 */
static SSMDeviceDriverMock* _gDriver;

/**
 * Constructor Group
 *
 * Test SSMDeviceDriverMock constructors.
 */
TEST_GROUP( ConstructorGroup )
{
  void setup()
  {
  }

  void teardown()
  {
  }
};

/**
 * CoreGroup
 *
 * Tests related to receiving state transition notifications and receiving status from the device.
 */
TEST_GROUP( CoreGroup )
{
  void setup()
  {
    // Create a new SSMDeviceDriverMock for each test
    _gDriver = new SSMDeviceDriverMock();
  }

  void teardown()
  {
    delete _gDriver;
  }
};

/**
 * ConfigGroup
 *
 * Tests related to configuring the device.
 */
TEST_GROUP( ConfigGroup )
{
  void setup()
  {
    // Create a new SSMDeviceDriverMock for each test
    _gDriver = new SSMDeviceDriverMock();
  }

  void teardown()
  {
    delete _gDriver;
  }
};

/**
 * Test Default Constructor
 */
TEST( ConstructorGroup, DefaultConstructor )
{
  SSMDeviceDriverMock dev;

  CHECK_EQUAL( SSM_IDLE, dev.stateGet() );
  CHECK_EQUAL( SSM_FAULT, dev.waitForStateChange() );
}

/**
 * Test Sequence Constructor
 */
TEST( ConstructorGroup, SequenceConstructor )
{ 
  SSMDeviceDriverMock dev( { SSM_INIT, SSM_BETWEEN_CYCLES } );

  CHECK_EQUAL( SSM_IDLE, dev.stateGet() );
  CHECK_EQUAL( SSM_INIT, dev.waitForStateChange() );
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, dev.waitForStateChange() );
  CHECK_EQUAL( SSM_FAULT, dev.waitForStateChange() );
}

/**
 * Test Initialize
 *
 * Verifies that calling initialize() puts the device into the first state in the sequence.
 */
TEST( CoreGroup, Initialize )
{
  // First verify that the initial state is IDLE
  CHECK_EQUAL( SSM_IDLE, _gDriver->stateGet() );

  // Program a simple state sequence
  _gDriver->stateSequenceSet( { SSM_BETWEEN_CYCLES, SSM_START_CYCLE, SSM_BETWEEN_SPILLS } );

  // Initialization should place us in the first state in the sequence
  _gDriver->initialize();
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, _gDriver->stateGet() );

  // Verify the rest of the sequence
  CHECK_EQUAL( SSM_START_CYCLE, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_BETWEEN_SPILLS, _gDriver->waitForStateChange() );

  // Verify that we go to FAULT when the sequence is completed
  CHECK_EQUAL( SSM_FAULT, _gDriver->waitForStateChange() );

  // Verify that calling initialize() again resets the sequence
  _gDriver->initialize();
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, _gDriver->stateGet() );
}

/**
 * Test Enumeration to String Conversion
 *
 * Verify that we have defined a string for every possible state.
 */
TEST( CoreGroup, ToString )
{
  // The state register is 8-bits so there are 256 possible states.
  // Loop through each possible value.  If it is undefined we expect the string to be "undefined".
  for( uint16_t stateNum = 0; stateNum < 256; stateNum++ )
    {
      ssm_state_t state = static_cast<ssm_state_t>( stateNum );
      stringstream ss;
      ss << state;

      switch( stateNum )
        {
        case SSM_IDLE: STRCMP_EQUAL( "idle", ss.str().c_str() ); break;
        case SSM_INIT: STRCMP_EQUAL( "init", ss.str().c_str() ); break;
        case SSM_BETWEEN_CYCLES: STRCMP_EQUAL( "between_cycles", ss.str().c_str() ); break;
        case SSM_START_CYCLE: STRCMP_EQUAL( "start_cycle", ss.str().c_str() ); break;
        case SSM_BETWEEN_SPILLS: STRCMP_EQUAL( "between_spills", ss.str().c_str() ); break;
        case SSM_READ_IBEAM: STRCMP_EQUAL( "read_ibeam", ss.str().c_str() ); break;
        case SSM_RAMP: STRCMP_EQUAL( "ramp", ss.str().c_str() ); break;
        case SSM_SPILL: STRCMP_EQUAL( "spill", ss.str().c_str() ); break;
        case SSM_START_EOS_TIMER: STRCMP_EQUAL( "start_eos_timer", ss.str().c_str() ); break;
        case SSM_SPILL_PARABOLA: STRCMP_EQUAL( "spill_parabola", ss.str().c_str() ); break;
        case SSM_START_CLEANUP_TIMER: STRCMP_EQUAL( "start_cleanup_timer", ss.str().c_str() ); break;
        case SSM_AFTER_SPILL: STRCMP_EQUAL( "after_spill", ss.str().c_str() ); break;
        case SSM_END_CYCLE: STRCMP_EQUAL( "end_cycle", ss.str().c_str() ); break;
        case SSM_LEARNING: STRCMP_EQUAL( "learning", ss.str().c_str() ); break;
        case SSM_ABORT: STRCMP_EQUAL( "abort", ss.str().c_str() ); break;
        case SSM_FAULT: STRCMP_EQUAL( "fault", ss.str().c_str() ); break;
        case SSM_UNKNOWN: STRCMP_EQUAL( "UNKNOWN", ss.str().c_str() ); break;
          
        default: STRCMP_EQUAL( "UNDEFINED", ss.str().c_str() ); break;
        }
    }
}

/**
 * Read State
 *
 * Verify that we can read the state and that this initial state is idle.
 */
TEST( CoreGroup, ReadState )
{
  CHECK_EQUAL( SSM_IDLE, _gDriver->stateGet() );
}

/**
 * Wait for Single State Change
 *
 * Verify that we can wait for a single spill state change to occur.
 */
TEST( CoreGroup, WaitForSingleStateChange )
{
  _gDriver->stateSequenceSet( { SSM_INIT } );

  // Initial state should be idle
  CHECK_EQUAL( SSM_IDLE, _gDriver->stateGet() );

  auto state = _gDriver->waitForStateChange();
  CHECK_EQUAL( SSM_INIT, state );
  CHECK_EQUAL( SSM_INIT, _gDriver->stateGet() );

  // A wait for more state changes should return a fault state
  state = _gDriver->waitForStateChange();
  CHECK_EQUAL( SSM_FAULT, state );
  CHECK_EQUAL( SSM_FAULT, _gDriver->stateGet() );
}

/**
 * Wait for Multiple State Changes
 *
 * Verifies that we can wait for multiple state changes to occur.
 */
TEST( CoreGroup, WaitForMultipleStateChanges )
{
  _gDriver->stateSequenceSet( { SSM_INIT,
        SSM_BETWEEN_CYCLES,
        SSM_START_CYCLE,
        SSM_BETWEEN_SPILLS,
        SSM_RAMP } );

  CHECK_EQUAL( SSM_IDLE, _gDriver->stateGet() );
  CHECK_EQUAL( SSM_INIT, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_START_CYCLE, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_BETWEEN_SPILLS, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_RAMP, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_FAULT, _gDriver->waitForStateChange() );
}

/**
 * Test loadSpillSequence Method
 *
 * Verifies that the loadSpillSequenceMethod creates n number of expected spills.
 */
TEST( CoreGroup, LoadSpillSequence )
{
  CHECK_EQUAL( SSM_IDLE, _gDriver->stateGet() );

  // No spills
  _gDriver->loadSpillSequence( 0 );
  _gDriver->initialize();
  CHECK_EQUAL( SSM_FAULT, _gDriver->waitForStateChange() );

  // 1 spills
  _gDriver->loadSpillSequence( 1 );
  _gDriver->initialize();
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, _gDriver->stateGet() );
  CHECK_EQUAL( SSM_READ_IBEAM, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_RAMP, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_SPILL, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_AFTER_SPILL, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_END_CYCLE, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_LEARNING, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, _gDriver->waitForStateChange() );
  CHECK_EQUAL( SSM_FAULT, _gDriver->waitForStateChange() );
}

/**
* Test Spill Counter
*
* Verifies that the spill counter increments each time we enter the SSM_SPILL state.
*/
TEST( CoreGroup, SpillCounter )
{
  // Make a sequence with 2 spills
  _gDriver->loadSpillSequence( 2 );

  _gDriver->initialize();
  CHECK_EQUAL( 0, _gDriver->spillCounterGet() );

  // Step through the entire sequence
  while( SSM_FAULT != _gDriver->waitForStateChange() );

  CHECK_EQUAL( 2, _gDriver->spillCounterGet() );
}

/**
 * Test Time-in-Spill Register
 *
 * Verifies that the time-in-spill register returns 0 after initialization and 107 after 
 * a spill sequence.
 */
TEST( CoreGroup, TimeInSpill )
{
  // Make a sequence with 1 spill
  _gDriver->loadSpillSequence( 1 );

  // Verify that timeInSpill is initialized to 0
  _gDriver->initialize();
  CHECK_EQUAL( 0, _gDriver->timeInSpillGet() );

  // Verify that timeInSpill is 107 after a spill sequence
  while( SSM_FAULT != _gDriver->waitForStateChange() );
  CHECK_EQUAL( 107, _gDriver->timeInSpillGet() );

  // Verify that timeInSpill returns to 0 after re-initializing
  _gDriver->initialize();
  CHECK_EQUAL( 0, _gDriver->timeInSpillGet() );
}

/**
 * Test Default Config Load
 *
 * Verifies that we load a default configuration without error.
 */
TEST( ConfigGroup, LoadDefaultConfig )
{
  // Make a default configuration
  ConfigurationManager defaultConf;
  auto ssmconf = defaultConf.ssmGet();

  // Load
  _gDriver->configure( ssmconf );

  // Verify
  CHECK_EQUAL( false, ssmconf.autoInitGet() );
  STRCMP_NOCASE_EQUAL( "mock", ssmconf.driverGet().c_str() );
  CHECK_EQUAL( 0, ssmconf.mockSpillsGet() );
}
