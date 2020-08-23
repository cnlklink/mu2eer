/**
* SSMDeviceTests.C
*
* This file contains unit tests for the SSMDevice class.
*
* @author jdiamond
*/

#include <chrono>
#include <iostream>
#include <thread>

#include "CppUTest/TestHarness.h"

#include "../mu2eerd/Controller.H"

#include "SSMDevice.H"

using namespace Mu2eER;
using namespace FFF;
using namespace std;

/**
* Core Group
*
* Tests related to the SSM ACNET devices.
*/
TEST_GROUP( CoreGroup )
{
  void setup()
  {
    Controller::testDaemonStart();

    SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
    shmc.waitForState( MU2EERD_RUNNING );
  }

  void teardown()
  {
    ControlMQClient mqc( Controller::TEST_DAEMON_CMQ_NAME );
    mqc.shutdown();

    Controller::testDaemonCleanup();
  }
};

/**
 * State Read Test
 *
 * Test the SSM State device reading property.
 */
TEST( CoreGroup, SSMStateRead )
{
  // Construct an ACNET request and response buffer
  ReqInfo request;
  SSMDevice::state_read_t buf;
  Array<SSMDevice::state_read_t> dest( &buf, Index( 0 ), Count( 1 ) );

  // Read state, should return SSM_IDLE in buf
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );
  device.stateRead( dest, &request );
  CHECK_EQUAL( SSM_IDLE, buf );

  // Handle no shared memory by throwing Ex_DEVFAILED
  SSMDevice deviceB( "/mu2eer_test", "does_not_exist" );
  CHECK_THROWS( AcnetError, deviceB.stateRead( dest, &request ) );

  // Handle bad offset
  Array<SSMDevice::state_read_t> destB( &buf, Index( SSMDevice::STATE_READING_MAX + 1 ), Count( 1 ) );
  CHECK_THROWS( AcnetError, device.stateRead( destB, &request ) );

  // Handle bad length
  Array<SSMDevice::state_read_t> destC( &buf, Index( 0 ), Count( SSMDevice::STATE_READING_MAX + 1 ) );
  CHECK_THROWS( AcnetError, device.stateRead( destC, &request ) );
}

/**
 * Spill Count Read Test
 *
 * Test the Spill Count device reading property
 */
TEST( CoreGroup, SpillCountReadInitial )
{
  // Construct an ACNET request and response buffer
  ReqInfo request;
  SSMDevice::spill_counter_read_t buf;
  Array<SSMDevice::spill_counter_read_t> dest( &buf, Index( 0 ), Count( 1 ) );

  // Read spill counter, should return 0 in buf
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );
  device.spillCounterRead( dest, &request );
  CHECK_EQUAL( 0, buf );

  // Handle no shared memory by throwing Ex_DEVFAILED
  SSMDevice deviceB( "/mu2eer_test", "does_not_exist" );
  CHECK_THROWS( AcnetError, deviceB.spillCounterRead( dest, &request ) );

  // Handle bad offset
  Array<SSMDevice::spill_counter_read_t> destB( &buf, 
                                                Index( SSMDevice::SPILL_COUNTER_READING_MAX + 1 ), 
                                                Count( 1 ) );
  CHECK_THROWS( AcnetError, device.spillCounterRead( destB, &request ) );

  // Handle bad length
  Array<SSMDevice::spill_counter_read_t> destC( &buf, 
                                                Index( 0 ), 
                                                Count( SSMDevice::SPILL_COUNTER_READING_MAX + 1 ) );
  CHECK_THROWS( AcnetError, device.spillCounterRead( destC, &request ) );
}
