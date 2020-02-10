/**
 * SharedMemoryClientTests.C
 *
 * This file contains unit tests for the SharedMemoryClient class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

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
 * Test Instantiation
 *
 * Verify instantiation and connecting to the shared memory API.
 */
TEST( ConstructionGroup, Instantiate )
{
  SharedMemoryClient shmc( "mu2eer_client_test" );

  STRCMP_EQUAL( API_VERSION, shmc.versionGet().c_str() );
}
