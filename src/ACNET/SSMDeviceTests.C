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
 * Global configuration manager used by _controller
 */
static ConfigurationManager* _cm;

/**
 * Global Controller object that setups up shared memory to be queried by tests
 */
static Controller* _controller;

/**
 * Global SharedMemoryClient for use by each test
 */
static SharedMemoryClient* _shmc;

/**
 * Global thread for _controller to run in
 */
static thread* _t;

/**
* State Group
*
* Tests related to the SSM state device properties.
*/
TEST_GROUP( SSMStateGroup )
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

    _shmc = new SharedMemoryClient( "mu2eer_test" );
    _shmc->waitForState( MU2EERD_RUNNING );
  }

  void teardown()
  {
    ControlMQClient mqc( "/mu2eer_test" );

    mqc.shutdown();
    _t->join();

    delete _shmc;
    delete _t;
    delete _controller;
    delete _cm;
  }
};

/**
 * State Read Test
 *
 * Test the SSM State device reading property.
 */
TEST( SSMStateGroup, SSMStateRead )
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
