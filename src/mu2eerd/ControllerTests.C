/**
 * ControllerTests.C
 *
 * This file contains unit tests for the Controller class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "Controller.H"
#include "SharedMemoryClient.H"

using namespace Mu2eER;
using namespace std;

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
    _ctlr = new Controller();
  }

  void teardown()
  {
    delete _ctlr;
  }
};

TEST( ConstructionGroup, Instatiation )
{
  Controller ctlr;
}

TEST( ConstructionGroup, Destruction )
{
  {
    Controller ctlr;
  }
}

TEST( OperationGroup, Start )
{
  SharedMemoryClient shmc( "mu2eer" );

  _ctlr->start();

  CHECK_EQUAL( SSM_IDLE, shmc.ssmBlockGet().currentStateGet() );
}

TEST( OperationGroup, Shutdown )
{
  _ctlr->start();
  _ctlr->shutdown();
}
