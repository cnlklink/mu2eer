/**
 * MockLinearDataTest.C
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

/*
 * A global SpillStateMachine object used for all tests
 */
static SpillStateMachine* _ssm;

/**
 * Shared memory manager for testing
 *   */
static SharedMemoryManager* _shmm;

/**
 * Initialization Group
 *
 * Tests related to initializing and tearing-down the SMM and SSM modules.
 */
TEST_GROUP( MockLinearGroup )
{
  void setup()
  {
    _shmm = new SharedMemoryManager( "mu2eer_test" );
    _ssm = new SpillStateMachine( _cm, _shmm->ssmBlockGet() );
  }

  void teardown()
  {
    delete _ssm;
    delete _shmm;
  }
};

/**
 * Test Initialization
 *
 * Verify that we can initialize the SSM and SMM modules with a basic configuration
 * and initialize the shared memory array.
*/
TEST( MockLinearGroup, Initialize )
{
   int i = 0, size = 0, j = 15999;
   const int *arr;

   auto& smb = _shmm->ssmBlockGet();
   _ssm->initialize();
   size = smb.idealSpillWaveFormSizeGet();
   smb.initialize();
   arr = smb.idealSpillWaveFormGet();

   for ( i = 0; i < size; i++ ) {
    CHECK_EQUAL( j , arr[i] );
    j--;
   }
}
