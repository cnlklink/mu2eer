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

#include "../mu2eerd/Controller.H"
#include "Mu2eerdDevice.H"

using namespace Mu2eER;
using namespace FFF;
using namespace std;

TEST_GROUP( StateGroup )
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

TEST( StateGroup, Read )
{
  ReqInfo request;
  Mu2eerdDevice::state_read_t buf;

  // Read state
  Array<Mu2eerdDevice::state_read_t> dest( &buf, Index( 0 ), Count( 1 ) );
  Mu2eerdDevice device( "/mu2eer_test", "mu2eer_test" );;
  device.stateRead( dest, &request );
  CHECK_EQUAL( MU2EERD_INITIALIZING, buf );

  // Handle no shared memory by throwing Ex_DEVFAILED
  Mu2eerdDevice deviceB( "/mu2eer_test", "does_not_exist" );
  CHECK_THROWS( AcnetError, deviceB.stateRead( dest, &request ) );

  // Handle bad offset
  Array<Mu2eerdDevice::state_read_t> destB( &buf, 
                                            Index( Mu2eerdDevice::STATE_READ_MAX + 1 ), 
                                            Count( 1 ) );
  CHECK_THROWS( AcnetError, device.stateRead( destB, &request ) );

  // Handle bad length
  Array<Mu2eerdDevice::state_read_t> destC( &buf, 
                                            Index( 0 ), 
                                            Count( Mu2eerdDevice::STATE_READ_MAX + 1 ) );
  CHECK_THROWS( AcnetError, device.stateRead( destC, &request ) );
}

TEST( StateGroup, LotsOfReads )
{
  ReqInfo request;
  Mu2eerdDevice::state_read_t buf;

  // Test that we can handle a lot of quick reads
  Array<Mu2eerdDevice::state_read_t> dest( &buf, Index( 0 ), Count( 1 ) );
  Mu2eerdDevice device( "/mu2eer_test", "mu2eer_test" );;
  for( unsigned int i = 0; i != 65000; i++ )
    {
      device.stateRead( dest, &request );
      CHECK_EQUAL( MU2EERD_INITIALIZING, buf );
    }
}
