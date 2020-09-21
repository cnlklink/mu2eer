/**
 * Mu2eerdDeviceTest.C
 *
 * This file contains unit tests for the Mu2eerdDevicce class.
 *
 * @author jdiamond
 */

#include <chrono>
#include <iostream>
#include <thread>

#include "CppUTest/TestHarness.h"
#include "testutils.H"

#include "../mu2eerd/Controller.H"
#include "Mu2eerdDevice.H"

using namespace Mu2eER;
using namespace FFF;
using namespace std;

/**
 * Tests for the Daemon Device
 */
TEST_GROUP( DaemonGroup )
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
 * Test Daemon Device / Status property
 */
TEST( DaemonGroup, Status )
{
  ReqInfo request;
  Mu2eerdDevice::daemon_statusctrl_t buf;
  Array<Mu2eerdDevice::daemon_statusctrl_t> 
    dest( &buf, Index( 0 ), Count( Mu2eerdDevice::DAEMON_STATUSCTRL_MAX ) );

  Mu2eerdDevice device( "acnet_tests", "/mu2eer_test", "mu2eer_test" );

  // Handle bad offset
  Array<Mu2eerdDevice::daemon_read_t> 
    destB( &buf, Index( Mu2eerdDevice::DAEMON_STATUSCTRL_MAX + 1 ), Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.daemonStatus( destB, &request ) );

  // Handle bad length
  Array<Mu2eerdDevice::daemon_read_t> 
    destC( &buf, Index( 0 ), Count( Mu2eerdDevice::DAEMON_STATUSCTRL_MAX + 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.daemonStatus( destC, &request ) );

  try
    {
      // Initial status
      device.daemonStatus( dest, &request );
      CHECK_EQUAL( Mu2eerdDevice::DAEMON_STATUS_RUNNING, buf );

      // Use a different process name to test daemon process status
      Mu2eerdDevice deviceB( "mu2eerd", "/mu2eer_test", "mu2eer_test" );
      deviceB.daemonStatus( dest, &request );
      CHECK_EQUAL( 0, buf );
    }
  catch( Error e )
    {
      FAIL( "daemonStatus threw an unexpected exception" );
    }
}

/**
 * Test Daemon Device / Reading property
 */
TEST( DaemonGroup, ReadingArray )
{
  ReqInfo request;
  Mu2eerdDevice::daemon_read_t buf[Mu2eerdDevice::DAEMON_READ_MAX];
  Array<Mu2eerdDevice::daemon_read_t> 
    dest( buf, Index( 0 ), Count( Mu2eerdDevice::DAEMON_READ_MAX ) );

  Mu2eerdDevice device( "acnet_tests", "/mu2eer_test", "mu2eer_test" );;
  
  // Handle no shared memory by throwing Ex_DEVFAILED
  Mu2eerdDevice deviceB( "acnet_tests", "/mu2eer_test", "does_not_exist" );
  CHECK_THROWS_ACNETERROR( Ex_DEVFAILED, deviceB.daemonRead( dest, &request ) );

  // Handle bad offset
  Array<Mu2eerdDevice::daemon_read_t> destB( buf, 
                                             Index( Mu2eerdDevice::DAEMON_READ_MAX + 1 ), 
                                             Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.daemonRead( destB, &request ) );

  // Handle bad length
  Array<Mu2eerdDevice::daemon_read_t> destC( buf, 
                                             Index( 0 ), 
                                             Count( Mu2eerdDevice::DAEMON_READ_MAX + 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.daemonRead( destC, &request ) );

  try
    {
      // 1 second delay to register an uptime > 0
      this_thread::sleep_for( chrono::seconds( 1 ) );

      SharedMemoryClient shmc( "mu2eer_test" );
      Mu2eerdDevice::daemon_read_t expected_uptime = shmc.uptimeGet();
      Mu2eerdDevice::daemon_read_t expected_pid = shmc.pidGet();
      Mu2eerdDevice::daemon_read_t expected_jenkins_num = 123;

      // Read the whole array
      device.daemonRead( dest, &request );
      CHECK_EQUAL( expected_uptime, dest[0] );
      CHECK_EQUAL( expected_pid, dest[1] );
      CHECK_EQUAL( expected_jenkins_num, dest[2] );

      // Read each array element individually
      Array<Mu2eerdDevice::daemon_read_t> 
        destD( buf, 
               Index( Mu2eerdDevice::DAEMON_READ_IDX_UPTIME ),
               Count( 1 ) );
      device.daemonRead( destD, &request );
      CHECK_EQUAL( expected_uptime, dest[0] );

      Array<Mu2eerdDevice::daemon_read_t> 
        destE( buf, 
               Index( Mu2eerdDevice::DAEMON_READ_IDX_PID ),
               Count( 1 ) );
      device.daemonRead( destE, &request );
      CHECK_EQUAL( expected_pid, dest[0] );

      Array<Mu2eerdDevice::daemon_read_t> 
        destF( buf, 
               Index( Mu2eerdDevice::DAEMON_READ_IDX_JENKINS_NUM ),
               Count( 1 ) );
      device.daemonRead( destF, &request );
      CHECK_EQUAL( expected_jenkins_num, dest[0] );

      // Read a slice of the array
      Array<Mu2eerdDevice::daemon_read_t> 
        destG( buf, 
               Index( Mu2eerdDevice::DAEMON_READ_IDX_PID ),
               Count( 2 ) );
      device.daemonRead( destG, &request );
      CHECK_EQUAL( expected_pid, dest[0] );
      CHECK_EQUAL( expected_jenkins_num, dest[1] );
    }
  catch( Error e )
    {
      FAIL( "daemonRead threw an unexpected exception" );
    }
}

TEST( DaemonGroup, LotsOfReads )
{
  ReqInfo request;
  Mu2eerdDevice::daemon_read_t buf[Mu2eerdDevice::DAEMON_READ_MAX];

  SharedMemoryClient shmc( "mu2eer_test" );
  Mu2eerdDevice::daemon_read_t expected_pid = shmc.pidGet();

  try
    {
      // Test that we can handle a lot of quick reads
      Array<Mu2eerdDevice::daemon_read_t> 
        dest( buf, Index( Mu2eerdDevice::DAEMON_READ_IDX_PID ), Count( 1 ) );
      Mu2eerdDevice device( "acnet_tests", "/mu2eer_test", "mu2eer_test" );;
      for( unsigned int i = 0; i != 65000; i++ )
        {
          device.daemonRead( dest, &request );
          CHECK_EQUAL( expected_pid, buf[0] );
        }
    }
  catch( ... )
    {
      FAIL( "unexpected exception caught" );
    }
}

TEST( DaemonGroup, Control )
{
  // Construct a request for the "START" command
  ReqInfo request;
  const Mu2eerdDevice::daemon_statusctrl_t buf = { Mu2eerdDevice::DAEMON_CONTROL_START };
  Mu2eerdDevice device( "mu2eerd", "/mu2eer_test", "mu2eer_test" );

  // Verify the devide handles a bad offset
  Array<const Mu2eerdDevice::daemon_statusctrl_t> srcA( &buf, Index( 1 ), Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.daemonControl( srcA, &request ) );

  // Verify the device handles a bad length
  Array<const Mu2eerdDevice::daemon_statusctrl_t> srcB( &buf, Index( 0 ), Count( 2 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.daemonControl( srcB, &request ) );

  // Verify the device handles a bad command
  const Mu2eerdDevice::daemon_statusctrl_t bufBad = { 
    static_cast<Mu2eerdDevice::daemon_statusctrl_t>( 65535 ) 
  };
  Array<const Mu2eerdDevice::daemon_statusctrl_t> srcC( &bufBad, Index( 0 ), Count( 1 ) );
  CHECK_THROWS_ACNETERROR( Ex_BADSET, device.daemonControl( srcC, &request ) );
}

/*
TEST( DaemonGroup, ControlStart )
{
  FAIL( "TODO" );
}
*/
