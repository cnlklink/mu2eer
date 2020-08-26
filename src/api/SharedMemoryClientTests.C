/**
 * SharedMemoryClientTests.C
 *
 * This file contains unit tests for the SharedMemoryClient class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "../mu2eerd/Controller.H"
#include "ControlMQClient.H"
#include "SharedMemoryClient.H"
#include "SharedMemoryManager.H"

using namespace Mu2eER;
using namespace std;

/**
 * Manages a shared memory region for the client to test against
 */
static SharedMemoryManager* _shmm;

/**
 * Construction Group
 *
 * Tests related to constructing & destructing the SharedMemoryClient.
 */
TEST_GROUP( ConstructionGroup )
{
  void setup()
  {
    _shmm = new SharedMemoryManager( "mu2eer_client_test" );
  }

  void teardown()
  {
    delete _shmm;
  }
};

/**
 * WaitForState Group
 *
 * Tests the waitForState method.
 */
TEST_GROUP( WaitForStateGroup )
{
  void setup()
  {
    _shmm = new SharedMemoryManager( "mu2eer_client_test" );
  }

  void teardown()
  {
    delete _shmm;
  }
};

/**
 * WaitForSSM Group
 *
 * Tests the waitForSSMState method.
 */
TEST_GROUP( WaitForSSMStateGroup )
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
 * Test Instantiation
 *
 * Verify instantiation and connecting to the shared memory API.
 */
TEST( ConstructionGroup, Instantiate )
{
  SharedMemoryClient shmc( "mu2eer_client_test" );

  STRCMP_EQUAL( API_VERSION, shmc.versionGet().c_str() );
}

/**
 * Wait For State Test
 *
 * Verify that waitForState works for an indefinate amount of time.
 */
TEST( WaitForStateGroup, WaitForever )
{
  SharedMemoryClient shmc( "mu2eer_client_test" );
  CHECK_EQUAL( MU2EERD_INITIALIZING, shmc.currentStateGet() );

  thread t( []() {
      // Pause for a second before starting
      this_thread::sleep_for( chrono::milliseconds( 1000 ) );
      _shmm->currentStateSet( MU2EERD_RUNNING );
    } );

  shmc.waitForState( MU2EERD_RUNNING );
  CHECK_EQUAL( MU2EERD_RUNNING, shmc.currentStateGet() );

  t.join();
}

/**
 * Wait For Spill State Machine Test
 *
 * Verify that waitForSSMState works for an indefinite amout of time.
 */
TEST( WaitForSSMStateGroup, WaitForever )
{
  SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
  auto& ssm = shmc.ssmBlockGet();

  CHECK_EQUAL( SSM_IDLE, ssm.currentStateGet() );

  ControlMQClient cmq( Controller::TEST_DAEMON_CMQ_NAME );
  cmq.start();

  shmc.waitForSSMState( SSM_FAULT );
  CHECK_EQUAL( SSM_FAULT, ssm.currentStateGet() );
}
