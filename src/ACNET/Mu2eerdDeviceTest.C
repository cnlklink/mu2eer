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

static ConfigurationManager* _cm;

static Controller* _controller;

static thread* _t;

TEST_GROUP( StateGroup )
{
  void setup()
  {
    _cm = new ConfigurationManager();
    _controller = new Controller( *_cm, "/mu2eer_test", "mu2eer_test" );

    _t = new thread( []() {
        try
          {
            _controller->start();
          }
        catch( controller_error e )
          {
            cerr << "exception: " << e.what() << endl;
          }
    } );

    SharedMemoryClient shmc( "mu2eer_test" );
    shmc.waitForState( MU2EERD_RUNNING );
  }

  void teardown()
  {
    ControlMQClient mqc( "/mu2eer_test" );

    mqc.shutdown();
    _t->join();

    delete _t;
    delete _controller;
    delete _cm;
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
