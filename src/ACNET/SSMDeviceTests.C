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

/**
 * Control Tests
 *
 * Tests setting the "START" command for the SSM device basic control property.
 */
TEST( CoreGroup, ControlStart )
{
  // Construct a request for the "START" command
  ReqInfo request;
  const SSMDevice::control_t buf = { SSMDevice::CONTROL_START };
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );
  
  // Verify the devide handles a bad offset
  Array<const SSMDevice::control_t> srcA( &buf, Index( 1 ), Count( 1 ) );
  CHECK_THROWS( AcnetError, device.statusCtrlWrite( srcA, &request ) );
  
  // Verify the device handles a bad length
  Array<const SSMDevice::control_t> srcB( &buf, Index( 0 ), Count( 2 ) );
  CHECK_THROWS( AcnetError, device.statusCtrlWrite( srcB, &request ) );
      
  // Verify the device handles a bad command
  const SSMDevice::control_t bufBad = { static_cast<SSMDevice::control_t>( 65535 ) };
  Array<const SSMDevice::control_t> srcC( &bufBad, Index( 0 ), Count( 1 ) );
  CHECK_THROWS( AcnetError, device.statusCtrlWrite( srcC, &request ) );
  
  // Verify that we are in the IDLE state
  SharedMemoryClient smc( Controller::TEST_DAEMON_SHM_NAME );
  CHECK_EQUAL( SSM_IDLE, smc.ssmBlockGet().currentStateGet() );
  
  // Send
  Array<const SSMDevice::control_t> srcD( &buf, Index( 0 ), Count( 1 ) );
  device.statusCtrlWrite( srcD, &request );
  
  // After starting the SSM mock driver should run this it's spill cycles and end in the FAULT state
  smc.waitForSSMState( SSM_FAULT, 100, 10 );
  CHECK_EQUAL( SSM_FAULT, smc.ssmBlockGet().currentStateGet() );
}
