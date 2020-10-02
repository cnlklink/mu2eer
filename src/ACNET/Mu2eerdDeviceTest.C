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
#include "DaemonController.H"
#include "Mu2eerdDevice.H"
#include "TestSystemController.H"

using namespace Mu2eER;
using namespace FFF;
using namespace std;

static TestSystemController _sysctlr;

static DaemonController 
_daemonCtlr( "mu2eerd", 
             "../bin/host/mu2eerd/mu2eerd", 
             "../bin/host/mu2eerd/mu2eerd -c ../etc/mu2eer.d/reference.conf 2>&1", 
             "../bin/host/mu2eercli/mu2eercli shutdown 2>&1" );
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

  DaemonController dctlr( "acnet_tests", "", "" );
  TestSystemController sysctlr;
  Mu2eerdDevice device( sysctlr, dctlr, "/mu2eer_test", "mu2eer_test" );

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
      CHECK_EQUAL( Mu2eerdDevice::DAEMON_STATUS_RUNNING |
                   Mu2eerdDevice::DAEMON_STATUS_SHM_CONNECTED |
                   Mu2eerdDevice::DAEMON_STATUS_CMQ_CONNECTED, 
                   buf );

      // Use a different process/shm/cmq name to test daemon process status
      Mu2eerdDevice deviceB( sysctlr,
                             DaemonController( "mu2eerd", "", "" ), 
                             "/mu2eer_test", 
                             "mu2eer_test" );
      deviceB.daemonStatus( dest, &request );
      CHECK_EQUAL( Mu2eerdDevice::DAEMON_STATUS_SHM_CONNECTED |
                   Mu2eerdDevice::DAEMON_STATUS_CMQ_CONNECTED,
                   buf );

      Mu2eerdDevice deviceC( sysctlr,
                             DaemonController( "acnet_tests", "", "" ), 
                             "/does_not_exist", 
                             "mu2eer_test" );
      deviceC.daemonStatus( dest, &request );
      CHECK_EQUAL( Mu2eerdDevice::DAEMON_STATUS_RUNNING |
                   Mu2eerdDevice::DAEMON_STATUS_SHM_CONNECTED, 
                   buf );

      Mu2eerdDevice deviceD( sysctlr,
                             DaemonController( "acnet_tests", "", "" ), 
                             "/mu2eer_test", 
                             "does_not_exist" );
      deviceD.daemonStatus( dest, &request );
      CHECK_EQUAL( Mu2eerdDevice::DAEMON_STATUS_RUNNING |
                   Mu2eerdDevice::DAEMON_STATUS_CMQ_CONNECTED, 
                   buf );
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

  TestSystemController sysctlr;
  DaemonController dctlr( "acnet_tests", "", "" );
  Mu2eerdDevice device( sysctlr, dctlr, "/mu2eer_test", "mu2eer_test" );;
  
  // Handle no shared memory by throwing Ex_DEVFAILED
  Mu2eerdDevice deviceB( sysctlr, dctlr, "/mu2eer_test", "does_not_exist" );
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
      Mu2eerdDevice::daemon_read_t expected_jenkins_num = 99;

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

      DaemonController dctlr( "acnet_tests", "", "" );
      TestSystemController sysctlr;
      Mu2eerdDevice device( sysctlr, dctlr, "/mu2eer_test", "mu2eer_test" );;

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
  TestSystemController sysctlr;
  DaemonController dctlr( "mu2eerd", "", "" );
  Mu2eerdDevice device( sysctlr, dctlr, "/mu2eer_test", "mu2eer_test" );

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

TEST( DaemonGroup, ControlStartStop )
{
  ReqInfo request;

  const Mu2eerdDevice::daemon_statusctrl_t startBuf = { Mu2eerdDevice::DAEMON_CONTROL_START };
  const Mu2eerdDevice::daemon_statusctrl_t stopBuf = { Mu2eerdDevice::DAEMON_CONTROL_STOP };
  Mu2eerdDevice::daemon_statusctrl_t readBuf;

  try
    {
      // Verify that the daemon is not running
      TestSystemController sysctlr;
      Mu2eerdDevice deviceA( sysctlr,
                             DaemonController( "mu2eerd", "", "" ), 
                             "/mu2eer_test", 
                             "mu2eer_test" );
      Array<Mu2eerdDevice::daemon_statusctrl_t> dest( &readBuf, Index( 0 ), Count( 1 ) );
      deviceA.daemonStatus( dest, &request );
      CHECK_EQUAL( 0, readBuf & Mu2eerdDevice::DAEMON_STATUS_RUNNING );

      // Send start command, start fails and should throw Ex_DEVFAILED
      Mu2eerdDevice deviceB( sysctlr,
                             DaemonController( "mu2eerd", 
                                               "../bin/host/mu2eerd/notfound 2>&1", 
                                               "../bin/host/mu2eercli/mu2eercli shutdown 2>&1" ), 
                             "/mu2eer_test", 
                             "mu2eer_test" );
      Array<const Mu2eerdDevice::daemon_statusctrl_t> startSrc( &startBuf, Index( 0 ), Count( 1 ) );
      CHECK_THROWS_ACNETERROR( Ex_DEVFAILED, deviceB.daemonControl( startSrc, &request ) );

      // Verify that the daemon is not running, again
      deviceA.daemonStatus( dest, &request );
      CHECK_EQUAL( 0, readBuf & Mu2eerdDevice::DAEMON_STATUS_RUNNING );

      // Start for real this time
      Mu2eerdDevice deviceC( sysctlr,
                             DaemonController( "mu2eerd", 
                                               "../bin/host/mu2eerd/mu2eerd 2>&1", 
                                               "../bin/host/mu2eercli/mu2eercli shutdown 2>&1" ), 
                             "/mu2eer_test", 
                             "mu2eer_test" );
      deviceC.daemonControl( startSrc, &request );

      // Verify that the daemon is running
      deviceA.daemonStatus( dest, &request );
      CHECK( Mu2eerdDevice::DAEMON_STATUS_RUNNING & readBuf );

      // Starting again should throw Ex_BADSET
      CHECK_THROWS_ACNETERROR( Ex_BADSET, deviceC.daemonControl( startSrc, &request ) );

      // Send stop request
      Array<const Mu2eerdDevice::daemon_statusctrl_t> stopSrc( &stopBuf, Index( 0 ), Count( 1 ) );
      deviceC.daemonControl( stopSrc, &request );

      // Verify that the daemon is not running
      deviceA.daemonStatus( dest, &request );
      CHECK_EQUAL( 0, readBuf & Mu2eerdDevice::DAEMON_STATUS_RUNNING );

      // Stopping again should throw Ex_BADSET
      CHECK_THROWS_ACNETERROR( Ex_BADSET, deviceC.daemonControl( stopSrc, &request ) );
    }
  catch( exception e )
    {
      cout << e.what() << endl;
      FAIL( "unexpected exception" );
    }
}

TEST( DaemonGroup, ControlReboot )
{
  try
    {
      // Construct reboot request
      ReqInfo request;
      const Mu2eerdDevice::daemon_statusctrl_t buf = { Mu2eerdDevice::DAEMON_CONTROL_SOFTREBOOT };
      Array<const Mu2eerdDevice::daemon_statusctrl_t> src( &buf, Index( 0 ), Count( 1 ) );

      // Device using the "test" SystemController
      TestSystemController sysctlr;
      Mu2eerdDevice device( sysctlr,
                            DaemonController( "mu2eerd", 
                                              "../bin/host/mu2eerd/mu2eerd 2>&1", 
                                              "../bin/host/mu2eercli/mu2eercli shutdown 2>&1" ), 
                            "/mu2eer_test", 
                            "mu2eer_test" );
      
      // Send reboot request
      device.daemonControl( src, &request );
      CHECK( sysctlr.isRebooting() );

      // Second request should throw Ex_BADSET because we're already rebooting
      CHECK_THROWS_ACNETERROR( Ex_BADSET, device.daemonControl( src, &request ) );
      CHECK( sysctlr.isRebooting() );
    }
  catch( exception e )
    {
      cout << e.what() << endl;
      FAIL( "unexpected exception" );
    }
}

/**
 * Tests for the Startup Device
 */
TEST_GROUP( StartupGroup )
{
  void setup()
  {
  }

  void teardown()
  {
    if( _daemonCtlr.isRunning() )
      {
        _daemonCtlr.stop();
      }
  }
};

/**
 * Test Startup Device Setting "OFF"
 *
 * Setting the Startup device to "OFF" should do nothing.  If mu2eerd is running, it stays running.
 * If mu2eerd is not running, it does not startup.
 */
TEST( StartupGroup, Off )
{
  try
    {
      ReqInfo request;

      // Device using the "test" SystemController
      Mu2eerdDevice device( _sysctlr, _daemonCtlr, "/mu2eer_test", "mu2eer_test" );

      Mu2eerdDevice::startup_set_t readBuf;

      // Handle bad offset / reading
      Array<Mu2eerdDevice::startup_set_t> destA( &readBuf, 
                                                 Index( Mu2eerdDevice::STARTUP_SET_MAX + 1 ), 
                                                 Count( 1 ) );
      CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.startupSetRead( destA, &request ) );
      
      // Handle bad length / reading
      Array<Mu2eerdDevice::startup_set_t> destB( &readBuf, 
                                                 Index( 0 ), 
                                                 Count( Mu2eerdDevice::STARTUP_SET_MAX + 1 ) );
      CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.startupSetRead( destB, &request ) );
      
      // Initial reading should be "Off"
      Array<Mu2eerdDevice::startup_set_t> destC( &readBuf, Index( 0 ), Count( 1 ) );
      device.startupSetRead( destC, &request );
      CHECK_EQUAL( Mu2eerdDevice::STARTUP_OFF, destC[0] );
      
      // Handle bad offset / setting
      const Mu2eerdDevice::startup_set_t onTestSetBuf = { Mu2eerdDevice::STARTUP_ON_TEST };
      Array<const Mu2eerdDevice::startup_set_t> srcA( &onTestSetBuf, 
                                                      Index( Mu2eerdDevice::STARTUP_SET_MAX + 1 ), 
                                                      Count( 1 ) );
      CHECK_THROWS_ACNETERROR( Ex_BADOFF, device.startupSetWrite( srcA, &request ) );
      
      // Handle bad length / setting
      Array<const Mu2eerdDevice::startup_set_t> srcB( &onTestSetBuf, 
                                                      Index( 0 ), 
                                                      Count( Mu2eerdDevice::STARTUP_SET_MAX + 1 ) );
      CHECK_THROWS_ACNETERROR( Ex_BADOFLEN, device.startupSetWrite( srcB, &request ) );
      
      // Construct an invalid setting
      const Mu2eerdDevice::startup_set_t invalidSetBuf = { 
        static_cast<Mu2eerdDevice::startup_set_t>( 0xBAD )
      };
      Array<const Mu2eerdDevice::startup_set_t> srcC( &invalidSetBuf, Index( 0 ), Count( 1 ) );
      CHECK_THROWS_ACNETERROR( Ex_DEVFAILED, device.startupSetWrite( srcC, &request ) );

      // Construct "On_Test" setting
      Array<const Mu2eerdDevice::startup_set_t> onTestSrc( &onTestSetBuf, Index( 0 ), Count( 1 ) );

      // Set to "On_test", test that the daemon starts
      device.startupSetWrite( onTestSrc, &request );
      CHECK_EQUAL( true, _daemonCtlr.isRunning() );

      // Verify reading is now "On_Test"
      device.startupSetRead( destC, &request );
      CHECK_EQUAL( Mu2eerdDevice::STARTUP_ON_TEST, destC[0] );

      // Ensure that test configuration was loaded
      SharedMemoryClient shmc( MU2EERD_SHM_NAME );
      STRCMP_EQUAL( "../etc/mu2eer.d/reference.conf", shmc.configFileGet().c_str() );

      // Setting to "On_Test" again should have no effect
      device.startupSetWrite( onTestSrc, &request );
      CHECK_EQUAL( true, _daemonCtlr.isRunning() );

      // Setting to "Off" should have no effect
      const Mu2eerdDevice::startup_set_t offSetBuf = { 
        static_cast<Mu2eerdDevice::startup_set_t>( Mu2eerdDevice::STARTUP_OFF )
      };
      Array<const Mu2eerdDevice::startup_set_t> offSetSrc( &offSetBuf, Index( 0 ), Count( 1 ) );
      device.startupSetWrite( offSetSrc, &request );
      CHECK_EQUAL( true, _daemonCtlr.isRunning() );
    }
  catch( exception e )
    {
      cout << e.what() << endl;
      FAIL( "unexpected exception" );
    }
}

/**
 * Test Startup Device Setting "On_Oper"
 *
 * Setting the Startup device to "On_Oper" should start mu2eerd with the operational configuration
 * file.
 */
TEST( StartupGroup, OnOper )
{
  try
    {
      ReqInfo request;
      
      // Device using the "test" SystemController
      Mu2eerdDevice device( _sysctlr, _daemonCtlr, "/mu2eer_test", "mu2eer_test" );
      
      // Set to "On_Prod", test that the daemon starts
      const Mu2eerdDevice::startup_set_t onProdSetBuf = { 
        static_cast<Mu2eerdDevice::startup_set_t>( Mu2eerdDevice::STARTUP_ON_OPER )
      };
      Array<const Mu2eerdDevice::startup_set_t> 
        onProdSrc( &onProdSetBuf, Index( 0 ), Count( 1 ) );
      device.startupSetWrite( onProdSrc, &request );
      CHECK_EQUAL( true, _daemonCtlr.isRunning() );
      
      // Verify reading is now "On_Prod"
      Mu2eerdDevice::startup_set_t readBuf;      
      Array<Mu2eerdDevice::startup_set_t> dest( &readBuf, Index( 0 ), Count( 1 ) );
      device.startupSetRead( dest, &request );
      CHECK_EQUAL( Mu2eerdDevice::STARTUP_ON_OPER, dest[0] );
      
      // Ensure that production configuration was loaded
      SharedMemoryClient shmc( MU2EERD_SHM_NAME );
      STRCMP_EQUAL( "none (default configuration loaded)", shmc.configFileGet().c_str() );
      
      // Setting to "On_Prod" again should have no effect
      device.startupSetWrite( onProdSrc, &request );
      CHECK_EQUAL( true, _daemonCtlr.isRunning() );
    }
  catch( exception e )
    {
      cout << e.what() << endl;
      FAIL( "unexpected exception" );
    }
}
