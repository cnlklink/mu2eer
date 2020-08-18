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
  int i = 0;
  int size = 0;
  int j = 15999;
  const int *arr[16000] = {0};

  auto& smb = _shmm.ssmBlockGet();
  _ssm->initialize();

  //size = smb.dataSizeGet();

  cout << "Mock linear is starting" << endl;
  //cout << "Data size is " << smb.dataSizeGet() << endl;

  /*
  arr = smb.dataGet();

  cout << "Mock linear test : Testing Initializing spill state & shared memory" << endl;
  smb.initialize();
  smb.addLinearData();
  for ( i = size - 1; i >= 0; i-- ) {
    CHECK_EQUAL( j , arr[i] );
    j--;
  }
  */

  cout << "Mock linear test done" << endl;

}
