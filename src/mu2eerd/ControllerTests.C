/**
 * ControllerTests.C
 *
 * This file contains unit tests for the Controller class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "Controller.H"
#include "mu2eerd.H"
#include "SharedMemoryClient.H"

using namespace Mu2eER;
using namespace std;

/**
 * Global ConfigurationManager object
 */
static ConfigurationManager* _cm;

/**
 * Global Controller object
 */
static Controller* _ctlr;

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
    _ctlr = new Controller( *_cm );
  }

  void teardown()
  {
    delete _ctlr;
    delete _cm;
  }
};

TEST( ConstructionGroup, Instatiation )
{
  ConfigurationManager cm;
  Controller ctlr( cm );
}

TEST( ConstructionGroup, Destruction )
{
  {
    ConfigurationManager cm;
    Controller ctlr( cm );
  }

  // Verify that the shared memory segment was de-allocated by trying to connect
  CHECK_THROWS( api_error, SharedMemoryClient( "mu2eer" ) );
}

TEST( OperationGroup, StartupShutdown )
{
  SharedMemoryClient shmc( "mu2eer" );

  CHECK_EQUAL( MU2EERD_INITIALIZING, shmc.currentStateGet() );

  _ctlr->start();
  
  CHECK_EQUAL( MU2EERD_RUNNING, shmc.currentStateGet() );

  _ctlr->shutdown();

  CHECK_EQUAL( MU2EERD_SHUTDOWN, shmc.currentStateGet() );
}

TEST( OperationGroup, StartWithSSMAutoInit )
{
  _cm->ssmAutoInitSet( true );

  SharedMemoryClient shmc( "mu2eer" );

  _ctlr->start();

  CHECK_EQUAL( MU2EERD_RUNNING, shmc.currentStateGet() );
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, shmc.ssmBlockGet().currentStateGet() );
}
