/**
 * SSMDeviceDriverMockTest.C
 *
 * This file contains unit tests (CppUTest) for the SSMDeviceDriverMock class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "SSMDeviceDriverMock.H"

using namespace Mu2eER;
using namespace std;

#include <sstream>

/**
 * A global SSMDeviceDriverMock object used for all tests
 */
static SSMDeviceDriverMock* _gDriver;

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
 * Test Enumeration to String Conversion
 *
 * Verify that we have defined a string for every possible state.
 */
TEST( CoreGroup, ToString )
{
  // The state register is 8-bits so there are 256 possible states.
  // Loop through each possible value.  If it is undefined we expect the string to be "undefined".
  for( uint8_t stateNum = 0; stateNum < 255; stateNum++ )
    {
      ISSMDeviceDriver::State state = static_cast<ISSMDeviceDriver::State>( stateNum );
      stringstream ss;
      ss << state;

      switch( stateNum )
        {
        case ISSMDeviceDriver::STATE_IDLE: STRCMP_EQUAL( "idle", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_INIT: STRCMP_EQUAL( "init", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_BETWEEN_CYCLES: STRCMP_EQUAL( "between_cycles", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_START_CYCLE: STRCMP_EQUAL( "start_cycle", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_BETWEEN_SPILLS: STRCMP_EQUAL( "between_spills", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_READ_IBEAM: STRCMP_EQUAL( "read_ibeam", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_RAMP: STRCMP_EQUAL( "ramp", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_SPILL: STRCMP_EQUAL( "spill", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_START_EOS_TIMER: STRCMP_EQUAL( "start_eos_timer", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_SPILL_PARABOLA: STRCMP_EQUAL( "spill_parabola", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_START_CLEANUP_TIMER: 
          STRCMP_EQUAL( "start_cleanup_timer", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_AFTER_SPILL: STRCMP_EQUAL( "after_spill", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_END_CYCLE: STRCMP_EQUAL( "end_cycle", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_LEARNING: STRCMP_EQUAL( "learning", ss.str().c_str() ); break;
        case ISSMDeviceDriver::STATE_ABORT: STRCMP_EQUAL( "abort", ss.str().c_str() ); break;
          
        default: STRCMP_EQUAL( "UNDEFINED", ss.str().c_str() ); break;
        }
    }
}
