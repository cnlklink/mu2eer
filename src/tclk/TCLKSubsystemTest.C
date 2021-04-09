/**
 * TCLKSubsystemTest.C
 *
 * This file contains unit tests (CppUTest) for the TCLKSubsystem class.
 *
 * @author jdiamond
 */

#include <algorithm>

#include "CppUTest/TestHarness.h"

#include "TCLKSubsystem.H"

using namespace Mu2eER;
using namespace std;

/**
 * Use a default configuration for testing
 */
static ConfigurationManager _cm;

/**
 * A shared memory block for the mock to dump it's state to
 */
static TCLKSMB _tclkSMB;

/**
 * A global TCLKSubsystem object used for all tests
 */
static TCLKSubsystem* _tclk;

/**
 * Core Group
 *
 * Most tests.
 */
TEST_GROUP( TCLKSubsystemCoreGroup )
{
  void setup()
  {
    _tclk = new TCLKSubsystem( _cm, _tclkSMB );
  }

  void teardown()
  {
    delete _tclk;
  }
};

/**
 * Test Initial State
 */
TEST( TCLKSubsystemCoreGroup, InitialState )
{
  STRCMP_EQUAL( "none", _tclkSMB.driverNameGet().c_str() );
  CHECK_EQUAL( 0, _tclkSMB.eventCounterGet( 0x02 ) );
}

/**
 * initialize() should set the driver name
 */
TEST( TCLKSubsystemCoreGroup, InitializeSetsDriverName )
{
  _tclk->initialize();

  STRCMP_EQUAL( "mock", _tclkSMB.driverNameGet().c_str() );
}

/**
 * Should see 1 or 2 FTP reset events after waiting for 5 seconds
 */
TEST( TCLKSubsystemCoreGroup, FTPResetEventCounter )
{
  _tclk->initialize();

  CHECK_EQUAL( 0, _tclkSMB.eventCounterGet( 0x02 ) );

  /*
    sleep( 5 );
    CHECK_EQUAL( 1, _tclkSMB.eventCounterGet( 0x02 ) );
  */
}
