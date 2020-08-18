/**
* MockLinearDataTest.C
*
* This file contains unit tests (CppUTest) for the MockLinearData class.
*
* @author rtadkins
*/

#include <iostream>

#include "CppUTest/TestHarness.h"

#include "SharedMemoryManager.H"
#include "SpillStateMachine.H"

using namespace Mu2eER;
using namespace std;

/**
 * Use a default configuration for testing
 */
static ConfigurationManager _cm;

/**
 * Shared memory manager for testing
 */
static SharedMemoryManager _shmm( "mu2eer_test" );

/**
 * A global SpillStateMachine object used for all tests
 */
static SpillStateMachine* _ssm;

/**
 * A global SpillStateMachine object used for all tests
 */
static SpillStateMachineSMB* _ssmb;

/**
 * Initialization Group
 *
 * Tests related to initializing and tearing-down the SSM module.
 */
TEST_GROUP( MockLinearInitGroup )
{
  void setup()
  {
    cout << "Setting up mock linear test" << endl;
    _ssm = new SpillStateMachine( _cm, _shmm.ssmBlockGet() );
  }

  void teardown()
  {
    delete _ssm;
  }
};

/**
 * Test Initialization
 *
 * Verify that we can initialize the SSM module with a basic configuration
 * and initialize the shared memory vector.
 */
TEST( MockLinearInitGroup, Initialize )
{
  int i = 0, *arr, size = 0, j = 15999;

  auto& smb = _shmm.ssmBlockGet();

  size = smb.dataSizeGet();
  arr = smb.dataGet();

  cout << "Mock linear test : Testing Initializing spill state & shared memory" << endl;
  _ssm->initialize();
  smb.initialize();
  smb.addLinearData();
  for ( i = size - 1; i >= 0; i-- ) {
    CHECK_EQUAL( j-- , arr[i] );
  }

  cout << "Mock linear test done" << endl;

}
