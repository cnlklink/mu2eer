/**
 * TCLKDeviceTests.C
 *
 * This file contains unit tests for the TCLKDevice class.
 *
 * @author jdiamond
 */

#include <chrono>
#include <iostream>
#include <thread>

#include "CppUTest/TestHarness.h"

#include "testutils.H"

#include "../mu2eerd/Controller.H"
#include "ControlMQClient.H"
#include "TCLKDevice.H"

using namespace Mu2eER;
using namespace FFF;
using namespace std;

/**
 * Core Group
 *
 * Tests related to the TCLK ACNET devices.
 */
TEST_GROUP( TCLKCoreGroup )
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
 * Event Counter Read Test
 *
 * Test the TCLK Event Count device reading property
 */
TEST( TCLKCoreGroup, EventCountRead )
{
  // Construct an ACNET request and response buffer
  ReqInfo request;
  TCLKDevice::event_counter_read_t buf;
  Array<TCLKDevice::event_counter_read_t> dest( &buf, Index( 0 ), Count( 1 ) );

  // Read initial event counter, should return 0 in buf
  TCLKDevice device( "/mu2eer_test", "mu2eer_test" );
  device.eventCounterRead( dest, &request );
  CHECK_EQUAL( 0, buf );

  /*
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
  */
}

