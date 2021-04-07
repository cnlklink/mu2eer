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
 * Global SharedMemoryManager object
 */
static SharedMemoryManager* _shmm;

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
 * Spill State Machine Group
 *
 * Tests related to the Spill State Machine shared memory block.
 */
TEST_GROUP( SMMGroup )
{
  void setup()
  {
    _shmm = new SharedMemoryManager( "mu2eer_test" );
  }

  void teardown()
  {
    delete _shmm;
  }
};

/**
 * TCLK Group
 *
 * Tests related to the TCLK shared memory block.
 */
TEST_GROUP( TCLKGroup )
{
  void setup()
  {
    _shmm = new SharedMemoryManager( "mu2eer_test" );
  }

  void teardown()
  {
    delete _shmm;
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

/**
 * Test Spill State Machine Block Construction
 *
 * Verify that the spill state machine block is constructed.
 */
TEST( SMMGroup, Construction )
{
  try
    {
      SpillStateMachineSMB& ssmb = _shmm->ssmBlockGet();

      CHECK_EQUAL( SSM_UNKNOWN, ssmb.currentStateGet() );
    }
  catch( api_error e )
    {
      cerr << "Exception: " << e.what() << endl;
      FAIL( "Failed to get SSM shared memory block" );
    }
}

/**
 * Test TCLK Block Construction
 *
 * Verify that the TCLK block is constructed.
 */
TEST( TCLKGroup, Construction )
{
  TCLKSMB& tclk = _shmm->tclkBlockGet();
  STRCMP_EQUAL( "none", tclk.driverNameGet().c_str() );
}
