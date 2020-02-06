/**
 * SharedMemoryManagerTests.C
 *
 * This file contains unit tests (CppUTest) for the SpillStateMachine class.
 *
 * @author jdiamond
 */

#include <iostream>

#include "CppUTest/TestHarness.h"

#include "SharedMemoryManager.H"

using namespace Mu2eER;
using namespace std;

/**
 * Initialization Group
 *
 * Tests related to constructing and destructing the shared memory interface.
 */
TEST_GROUP( InitGroup )
{
  void setup()
  {
  }

  void teardown()
  {
  }
};

/**
 * Test Construction
 *
 * Verify that the shared memory interface is constructed properly.
 */
TEST( InitGroup, Construction )
{
  {
    try
      {
        SharedMemoryManager shmmA( "mu2eer_test" );
    
        CHECK_THROWS( api_error, SharedMemoryManager( "mu2eer_test" ) );
      }
    catch( api_error e )
      {
        // This should always succeed unless previous tests seqfaulted and orphaned a previous
        // allocation (check /dev/shm)
        cerr << "Exception: " << e.what() << endl;
        FAIL( "Failed to create shared memory when it should have." );
      }
  }

  // This will throw if shmmA did not clean-up when it went out of scope
  SharedMemoryManager shmmB( "mu2eer_test" );

  // Verify API version
  STRCMP_EQUAL( API_VERSION, shmmB.versionGet().c_str() );
}
