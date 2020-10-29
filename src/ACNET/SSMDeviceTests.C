/**
 * SSMDeviceTests.C
 *
 * This file contains unit tests for the SSMDevice class.
 *
 * @author jdiamond and rtadkins
 */

#include <chrono>
#include <iostream>
#include <thread>

#include "CppUTest/TestHarness.h"

#include "testutils.H"

#include "../mu2eerd/Controller.H"
#include "SpillStateMachineSMB.H"

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
    Controller::testDaemonStart( "../etc/mu2eer.d/reference.conf" );

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
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, buf );

  // Handle no shared memory by throwing Ex_DEVFAILED
  SSMDevice deviceB( "/mu2eer_test", "does_not_exist" );
  CHECK_THROWS_ACNETERROR( Ex_DEVFAILED, deviceB.stateRead( dest, &request ) );

  // Handle bad offset
  Array<SSMDevice::state_read_t> destB( &buf, Index( SSMDevice::STATE_READING_MAX + 1 ), Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.stateRead( destB, &request ) );

  // Handle bad length
  Array<SSMDevice::state_read_t> destC( &buf, Index( 0 ), Count( SSMDevice::STATE_READING_MAX + 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.stateRead( destC, &request ) );
}

/**
 * Spill Count Read Test
 *
 * Test the Spill Count device reading property
 */
TEST( CoreGroup, SpillCountRead )
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
  CHECK_THROWS_ACNETERROR( Ex_DEVFAILED, deviceB.spillCounterRead( dest, &request ) );

  // Handle bad offset
  Array<SSMDevice::spill_counter_read_t> destB( &buf,
                                                Index( SSMDevice::SPILL_COUNTER_READING_MAX + 1 ),
                                                Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.spillCounterRead( destB, &request ) );

  // Handle bad length
  Array<SSMDevice::spill_counter_read_t> destC( &buf,
                                                Index( 0 ),
                                                Count( SSMDevice::SPILL_COUNTER_READING_MAX + 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.spillCounterRead( destC, &request ) );

  // Run a few cycles...
  ControlMQClient cmq( "/mu2eer_test" );
  SharedMemoryClient shmc( "mu2eer_test" );
  cmq.start();
  shmc.waitForSSMState( SSM_FAULT, 100, 10 );
  CHECK_EQUAL( SSM_FAULT, shmc.ssmBlockGet().currentStateGet() );

  // There should have been 5 cycles
  device.spillCounterRead( dest, &request );
  CHECK_EQUAL( 5, buf );
}

/**
 * Control Property Tests
 *
 * Verify the control device.
 */
TEST( CoreGroup, Control )
{
  // Construct a request for the "START" command
  ReqInfo request;
  const SSMDevice::control_t buf = { SSMDevice::CONTROL_START };
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );

  // Verify the devide handles a bad offset
  Array<const SSMDevice::control_t> srcA( &buf, Index( 1 ), Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.statusCtrlWrite( srcA, &request ) );

  // Verify the device handles a bad length
  Array<const SSMDevice::control_t> srcB( &buf, Index( 0 ), Count( 2 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.statusCtrlWrite( srcB, &request ) );

  // Verify the device handles a bad command
  const SSMDevice::control_t bufBad = { static_cast<SSMDevice::control_t>( 65535 ) };
  Array<const SSMDevice::control_t> srcC( &bufBad, Index( 0 ), Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADSET, device.statusCtrlWrite( srcC, &request ) );

  // Verify that the device throws Ex_DEVFAILED when it can't connect to command queue
  SSMDevice deviceB( "/does_not_exist", "does_not_exist" );
  Array<const SSMDevice::control_t> srcD( &buf, Index( 0 ), Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_DEVFAILED, deviceB.statusCtrlWrite( srcD, &request ) );
}

/**
 * Control Property / Start Command Tests
 *
 * Tests setting the "START" command for the SSM device basic control property.
 */
TEST( CoreGroup, ControlStart )
{
  // Construct a request for the "START" command
  ReqInfo request;
  const SSMDevice::control_t buf = { SSMDevice::CONTROL_START };
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );

  // Verify that we are in the BETWEEN_CYCLES state
  SharedMemoryClient smc( Controller::TEST_DAEMON_SHM_NAME );
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, smc.ssmBlockGet().currentStateGet() );

  // Send
  Array<const SSMDevice::control_t> src( &buf, Index( 0 ), Count( 1 ) );
  device.statusCtrlWrite( src, &request );

  // After starting the SSM mock driver should run this it's spill cycles and end
  // in the FAULT state
  smc.waitForSSMState( SSM_FAULT, 100, 10 );
  CHECK_EQUAL( SSM_FAULT, smc.ssmBlockGet().currentStateGet() );
}

/**
 * Control Property / Reset Command Tests
 *
 * Tests setting the "RESET" command for the SSM device basic control property.
 */
TEST( CoreGroup, ControlReset )
{
  // Construct a request for the "RESET" command
  ReqInfo request;
  const SSMDevice::control_t buf = { SSMDevice::CONTROL_RESET };
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );

  // Verify that we are in the BETWEEN_CYCLES state
  SharedMemoryClient smc( Controller::TEST_DAEMON_SHM_NAME );
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, smc.ssmBlockGet().currentStateGet() );

  // First, start the SSM
  ControlMQClient cmq( Controller::TEST_DAEMON_CMQ_NAME );
  cmq.start();

  // After starting the SSM mock driver should run this it's spill cycles and end
  // in the FAULT state
  smc.waitForSSMState( SSM_FAULT, 100, 10 );
  CHECK_EQUAL( SSM_FAULT, smc.ssmBlockGet().currentStateGet() );

  // Now send the reset command
  Array<const SSMDevice::control_t> src( &buf, Index( 0 ), Count( 1 ) );
  device.statusCtrlWrite( src, &request );

  // And the SSM mock driver should return to the IDLE state
  smc.waitForSSMState( SSM_IDLE, 100, 10 );
  CHECK_EQUAL( SSM_IDLE, smc.ssmBlockGet().currentStateGet() );
}

/**
 * Control Property / Fault Command Tests
 *
 * Tests setting the "FAULT" command for the SSM device basic control property.
 */
TEST( CoreGroup, ControlFault )
{
  try
    {
      // Construct a request for the "FAULT" command
      ReqInfo request;
      const SSMDevice::control_t buf = { SSMDevice::CONTROL_FAULT };
      SSMDevice device( "/mu2eer_test", "mu2eer_test" );

      // Verify that we are in the BETWEEN_CYCLES state
      SharedMemoryClient smc( Controller::TEST_DAEMON_SHM_NAME );
      CHECK_EQUAL( SSM_BETWEEN_CYCLES, smc.ssmBlockGet().currentStateGet() );

      // Now send the fault command
      Array<const SSMDevice::control_t> src( &buf, Index( 0 ), Count( 1 ) );
      device.statusCtrlWrite( src, &request );

      // And the SSM mock driver should go to the FAULT state
      smc.waitForSSMState( SSM_FAULT, 100, 10 );
      CHECK_EQUAL( SSM_FAULT, smc.ssmBlockGet().currentStateGet() );
    }
  catch( AcnetError e )
    {
      FAIL( "unexpected AcnetError caught" );
    }
  catch( Error e )
    {
      FAIL( "unexpected Error caught" );
    }
}

/**
 * Time-in-Spill Read Test
 *
 * Test the Time-in-Spill device reading property
 */
TEST( CoreGroup, TimeInSpill )
{
  // Construct an ACNET request and response buffer
  ReqInfo request;
  SSMDevice::tis_read_t buf;
  Array<SSMDevice::tis_read_t> dest( &buf, Index( 0 ), Count( 1 ) );

  // Read time in spill, should return 0 in buf
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );
  device.timeInSpillRead( dest, &request );
  CHECK_EQUAL( 0, buf );

  // Handle no shared memory by throwing Ex_DEVFAILED
  SSMDevice deviceB( "/mu2eer_test", "does_not_exist" );
  CHECK_THROWS( AcnetError, deviceB.timeInSpillRead( dest, &request ) );

  // Handle bad offset
  Array<SSMDevice::tis_read_t> destB( &buf,
                                      Index( SSMDevice::TIS_READING_MAX + 1 ),
                                      Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.timeInSpillRead( destB, &request ) );

  // Handle bad length
  Array<SSMDevice::tis_read_t> destC( &buf,
                                      Index( 0 ),
                                      Count( SSMDevice::TIS_READING_MAX + 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.timeInSpillRead( destC, &request ) );

  // Run a few cycles...
  ControlMQClient cmq( "/mu2eer_test" );
  SharedMemoryClient shmc( "mu2eer_test" );
  cmq.start();
  shmc.waitForSSMState( SSM_FAULT, 100, 10 );
  CHECK_EQUAL( SSM_FAULT, shmc.ssmBlockGet().currentStateGet() );

  // The last time in spill should be 107ms
  device.timeInSpillRead( dest, &request );
  CHECK_EQUAL( 107, buf );
}

/**
 * Ideal Spill Read Test
 *
 * Test the Ideal Spill device reading property
 */
TEST( CoreGroup, IdealSpillReadInitial )
{
  int i = 0, j = 15999, size = SSMDevice::IDEAL_SPILL_READING_MAX;

  // Construct an ACNET request and response buffer
  ReqInfo request;
  SSMDevice::ideal_spill_read_t* spill_buf = new SSMDevice::ideal_spill_read_t[16000];

  Array<SSMDevice::ideal_spill_read_t> dest( spill_buf, Index( 0 ), Count( SSMDevice::IDEAL_SPILL_READING_MAX ) );
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );

  device.idealSpillRead( dest, &request );

  for ( i = 0; i < size; i++ ) {
    CHECK_EQUAL( j, (int) dest[i] );
    j--;
  }

  // Handle no shared memory by throwing Ex_DEVFAILED
  SSMDevice deviceB( "/mu2eer_test", "does_not_exist" );
  CHECK_THROWS_ACNETERROR( Ex_DEVFAILED, deviceB.idealSpillRead( dest, &request ) );

  // Handle bad offset
  Array<SSMDevice::ideal_spill_read_t> destB( spill_buf,
                                                Index( SSMDevice::IDEAL_SPILL_READING_MAX + 1 ),
                                                Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.idealSpillRead( destB, &request ) );

  // Handle bad length
  Array<SSMDevice::ideal_spill_read_t> destC( spill_buf,
                                                Index( 0 ),
                                                Count( SSMDevice::IDEAL_SPILL_READING_MAX + 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.idealSpillRead( destC, &request ) );

  delete[] spill_buf;
  }

/**
 * Ideal Spill Read Slice Test
 *
 * Test the Ideal Spill device reading property of a slice of the data
 */
TEST( CoreGroup, IdealSpillReadSlice )
{
  int i = 0, j = 0, size = SSMDevice::IDEAL_SPILL_READING_MAX,
   count = 100, lower_bound = 0;

  lower_bound = size - count - 1;

  // Construct an ACNET request and response buffer
  ReqInfo request;
  SSMDevice::ideal_spill_read_t* spill_buf = new SSMDevice::ideal_spill_read_t[16000];

  Array<SSMDevice::ideal_spill_read_t> dest( spill_buf, Index( lower_bound ), Count( count ) );
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );

  device.idealSpillRead( dest, &request );

  j = count;
  for ( i = 0; i < count; i++ ) {
    CHECK_EQUAL( j, (int) dest[i] );
    j--;
  }

  delete[] spill_buf;
  }

/**
 * Actual Spill Read Test
 *
 * Test the Ideal Spill device reading property
 */
TEST( CoreGroup, ActualSpillReadInitial )
{
  int i = 0, j = 15999, size = SSMDevice::IDEAL_SPILL_READING_MAX;

  // Construct an ACNET request and response buffer
  ReqInfo request;
  SSMDevice::actual_spill_read_t* spill_buf = new SSMDevice::actual_spill_read_t[16000];

  Array<SSMDevice::actual_spill_read_t> dest( spill_buf, Index( 0 ), Count( SSMDevice::IDEAL_SPILL_READING_MAX ) );
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );

  device.actualSpillRead( dest, &request );

  for ( j = size - 1; j >= 0; j-- )
  {
    if ( j % 1000 == 0 )
    {
      j-=100;
      CHECK_EQUAL( j, (int) dest[i] );
    } else
    {
      CHECK_EQUAL( j, (int) dest[i] );
    }
    i++;
  }

  // Handle no shared memory by throwing Ex_DEVFAILED
  SSMDevice deviceB( "/mu2eer_test", "does_not_exist" );
  CHECK_THROWS_ACNETERROR( Ex_DEVFAILED, deviceB.idealSpillRead( dest, &request ) );

  // Handle bad offset
  Array<SSMDevice::ideal_spill_read_t> destB( spill_buf,
                                                Index( SSMDevice::IDEAL_SPILL_READING_MAX + 1 ),
                                                Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.idealSpillRead( destB, &request ) );

  // Handle bad length
  Array<SSMDevice::ideal_spill_read_t> destC( spill_buf,
                                                Index( 0 ),
                                                Count( SSMDevice::IDEAL_SPILL_READING_MAX + 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.idealSpillRead( destC, &request ) );

 delete[] spill_buf;
 }

/**
 * Error Signal Read Test
 *
 * Test the Error Signal device reading property
 */
TEST( CoreGroup, ErrorSignalReadInitial )
{
  int i = 0, j = 15999, size = SSMDevice::IDEAL_SPILL_READING_MAX;
  int _ideal[16000] = {0};
  int _actual[16000] = {0};
  int _error[16000] = {0};
  // Construct an ACNET request and response buffer
  ReqInfo request;
  SSMDevice::error_signal_read_t* spill_buf = new SSMDevice::error_signal_read_t[16000];

  Array<SSMDevice::error_signal_read_t> dest( spill_buf, Index( 0 ), Count( SSMDevice::IDEAL_SPILL_READING_MAX ) );
  SSMDevice device( "/mu2eer_test", "mu2eer_test" );

  device.errorSignalRead( dest, &request );

  j = 0;
  for (i = 15999; i >= 0; i--)
  {
    _ideal[j++] = i;
  }

  i = 0, j = 0;
  for ( i = 15999; i >= 0; i-- )
  {
    if ( i % 1000 == 0 )
    {
      i-=100;
      _actual[j++] = i;
    } else
    {
      _actual[j++] = i;
    }
  }

  for ( i = 0; i < size; i++ ) {
    _error[i] = _ideal[i] - _actual[i];
  }

  for ( i = 0; i < size; i++ ) {
    CHECK_EQUAL( _error[i], (int) dest[i] );
  }

  // Handle no shared memory by throwing Ex_DEVFAILED
  SSMDevice deviceB( "/mu2eer_test", "does_not_exist" );
  CHECK_THROWS_ACNETERROR( Ex_DEVFAILED, deviceB.errorSignalRead( dest, &request ) );

  // Handle bad offset
  Array<SSMDevice::error_signal_read_t> destB( spill_buf,
                                                Index( SSMDevice::IDEAL_SPILL_READING_MAX + 1 ),
                                                Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.errorSignalRead( destB, &request ) );

  // Handle bad length
  Array<SSMDevice::error_signal_read_t> destC( spill_buf,
                                                Index( 0 ),
                                                Count( SSMDevice::IDEAL_SPILL_READING_MAX + 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.errorSignalRead( destC, &request ) );

 delete[] spill_buf;
 }
