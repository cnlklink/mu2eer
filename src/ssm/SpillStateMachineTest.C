/**
 * SpillStateMachineTest.C
 *
 * This file contains unit tests (CppUTest) for the SpillStateMachine class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "SpillStateMachine.H"

using namespace Mu2eER;
using namespace std;

/**
 * A global SpillStateMachine object used for all tests
 */
static SpillStateMachine* _ssm;

/**
 * Initialization Group
 *
 * Tests related to initializing and tearing-down the SSM module.
 */
TEST_GROUP( InitGroup )
{
  void setup()
  {
    _ssm = new SpillStateMachine();
  }

  void teardown()
  {
    delete _ssm;
  }
};

/**
 * Test Initialization
 *
 * Verify that we can initializae the SSM module with a basic configuration
 */
TEST( InitGroup, Initialize )
{
  CHECK_THROWS( ssm_error, _ssm->stateGet() );

  _ssm->initialize();

  CHECK_EQUAL( SSM_BETWEEN_CYCLES, _ssm->stateGet() );
}
