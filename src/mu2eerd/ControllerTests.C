/**
 * ControllerTests.C
 *
 * This file contains unit tests for the Controller class.
 *
 * @author jdiamond
 */

#include <chrono>
#include <iostream>
#include <thread>

#include "CppUTest/TestHarness.h"

#include "Controller.H"
#include "ControlMQClient.H"
#include "mu2eerd.H"
#include "SharedMemoryClient.H"

using namespace Mu2eER;
using namespace std;

/**
 * ms wait between state checks
 */
static const unsigned int MILLIS_WAIT = 5;

/**
 * Global ConfigurationManager object
 */
static ConfigurationManager* _cm;

/**
 * Global Controller object
 */
static Controller* _ctlr;

/**
 * Global shared memory client
 */
static SharedMemoryClient* _shmc;

/**
 * Global control message queue client
 */
static ControlMQClient* _mqc;

/**
 * Construction Tests
 *
 * Tests related to constructing/destructing the Controller.
 */
TEST_GROUP( ConstructionGroup )
{
  void setup()
  {
  }

  void teardown()
  {
  }
};

/**
 * Operation Tests
 *
 * Tests related to the operation of the Controller.
 */
TEST_GROUP( OperationGroup )
{
  void setup()
  {
    _cm = new ConfigurationManager();
    _ctlr = new Controller( *_cm, "/mu2eer_test", "mu2eer_test" );
    _shmc = new SharedMemoryClient( "mu2eer_test" );
    _mqc = new ControlMQClient( "/mu2eer_test" );
  }

  void teardown()
  {
    delete _mqc;
    delete _shmc;
    delete _ctlr;
    delete _cm;
  }
};

TEST( ConstructionGroup, InvalidMQName )
{
  ConfigurationManager cm;
  CHECK_THROWS( api_error, Controller( cm, "mu2eer_test", "mu2eer_test" ) );
}

TEST( ConstructionGroup, DuplicateSHMs )
{
  ConfigurationManager cm;
  Controller ctlrA( cm, "/mu2eer_test", "mu2eer_test" );
  
  CHECK_THROWS( api_error, Controller( cm, "/mu2eer_test2", "mu2eer_test" ) );
}

TEST( ConstructionGroup, DuplicateMQs )
{
  ConfigurationManager cm;
  Controller ctlrA( cm, "/mu2eer_test", "mu2eer_test" );
  
  CHECK_THROWS( api_error, Controller( cm, "/mu2eer_test", "mu2eer_test2" ) );
}

TEST( ConstructionGroup, InstatiateTwo )
{
  ConfigurationManager cm;
  Controller ctlrA( cm, "/mu2eer_test", "mu2eer_test" );
  Controller ctlrB( cm, "/mu2eer_test2", "mu2eer_test2" );
}

TEST( ConstructionGroup, Instatiation )
{
  ConfigurationManager cm;
  Controller ctlr( cm, "/mu2eer_test", "mu2eer_test" );

  SharedMemoryClient shmc( "mu2eer_test" );
  CHECK_EQUAL( MU2EERD_INITIALIZING, shmc.currentStateGet() );
}

TEST( ConstructionGroup, Destruction )
{
  {
    ConfigurationManager cm;
    Controller ctlr( cm, "/mu2eer_test", "mu2eer_test" );
  }

  // Verify that the shared memory segment was de-allocated by trying to connect
  CHECK_THROWS( api_error, SharedMemoryClient( "mu2eer_test" ) );
}

TEST( OperationGroup, StartupShutdown )
{
  CHECK_EQUAL( MU2EERD_INITIALIZING, _shmc->currentStateGet() );

  // Start a thread for the controller
  thread t( []() {
      try
        {
          _ctlr->start();
        }
      catch( controller_error e )
        {
          cerr << e.what() << endl;
        }
  } );
 
  _shmc->waitForState( MU2EERD_RUNNING );
  CHECK_EQUAL( MU2EERD_RUNNING, _shmc->currentStateGet() );

  // Send shutdown message
  _mqc->shutdown();

  // Wait for the controller to exit
  t.join();
  CHECK_EQUAL( MU2EERD_SHUTDOWN, _shmc->currentStateGet() );
}

TEST( OperationGroup, VerifyPID )
{
  // Startup the controller in another thread.
  thread t( []() {
      try
        {
          _cm->ssmGet().autoInitSet( true );
          _ctlr->start();
        }
      catch( controller_error e )
        {
          cerr << e.what() << endl;
        }
  } );

  _shmc->waitForState( MU2EERD_RUNNING );
  CHECK( _shmc->pidGet() > 1 );

  // Shutdown
  _mqc->shutdown();
  t.join();
}

TEST( OperationGroup, VerifyStartTime )
{
  // Startup the controller in another thread.
  thread t( []() {
      try
        {
          _cm->ssmGet().autoInitSet( true );
          _ctlr->start();
        }
      catch( controller_error e )
        {
          cerr << e.what() << endl;
        }
  } );

  _shmc->waitForState( MU2EERD_RUNNING );

  // Make sure mu2eerd was started in the last 2 seconds
  time_t now;
  time( &now );
  CHECK( _shmc->startTimeGet() > (now - 2) && now <= _shmc->startTimeGet()  );

  // Shutdown
  _mqc->shutdown();
  t.join();
}

TEST( OperationGroup, StartWithSSMAutoInit )
{
  // Startup the controller in another thread.
  thread t( []() {
      try
        {
          _cm->ssmGet().autoInitSet( true );
          _ctlr->start();
        }
      catch( controller_error e )
        {
          cerr << e.what() << endl;
        }
  } );

  _shmc->waitForState( MU2EERD_RUNNING );
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, _shmc->ssmBlockGet().currentStateGet() );

  // Shutdown
  _mqc->shutdown();
  t.join();
  CHECK_EQUAL( MU2EERD_SHUTDOWN, _shmc->currentStateGet() );
}

TEST( OperationGroup, BadMQMessages )
{
  // Startup the controller in another thread.
  thread t( []() {
      try
        {
          _ctlr->start();
        }
      catch( controller_error e )
        {
          return;
        }

      FAIL( "expected to throw controller_error" );
  } );
  _shmc->waitForState( MU2EERD_RUNNING );

  // Test invalid command
  _mqc->testBadCommand();
  t.join();
}

TEST( OperationGroup, InitializeSSM )
{
  // Startup the controller in another thread.
  thread t( []() {
      try
        {
          _ctlr->start();
        }
      catch( controller_error e )
        {
          cerr << "exception: " << e.what() << endl;
        }
  } );
  _shmc->waitForState( MU2EERD_RUNNING );

  _mqc->ssmInit();

  // Wait up for the SSM to transition
  for( unsigned int i = 0; i != 5; i++ )
    {
      if( SSM_BETWEEN_CYCLES == _shmc->ssmBlockGet().currentStateGet() )
        {
          break;
        }
      this_thread::sleep_for( chrono::milliseconds( MILLIS_WAIT ) );
    }

  CHECK_EQUAL( SSM_BETWEEN_CYCLES, _shmc->ssmBlockGet().currentStateGet() );

  _mqc->shutdown();
  t.join();
}
