/**
 * TCLKDecoderMockTest.C
 *
 * This file contains unit tests (CppUTest) for the TCLKDecoderDriverMock class.
 *
 * @author jdiamond
 */

#include <algorithm>

#include "CppUTest/TestHarness.h"

#include "TCLKDecoderDriverMock.H"

using namespace Mu2eER;
using namespace std;

/**
 * A global TCLKDecoderDriverMock object used for all tests
 */
static TCLKDecoderDriverMock* _gDriver;

/**
 * DecoderGroup
 *
 * Tests related to the TCLK Decoder functions.
 */
TEST_GROUP( DecoderGroup )
{
  void setup()
  {
    // Create a new TCLKDecoderDriverMock for each test
    _gDriver = new TCLKDecoderDriverMock();
  }

  void teardown()
  {
    delete _gDriver;
  }
};

/**
 * Get Empty TCLK Event List Test
 * 
 * Tests that the TCLKDecoderDriverMock returns an empty list after it is instantiated.
 */
TEST( DecoderGroup, GetEmptyEventList )
{
  auto events = _gDriver->eventListGet();

  CHECK_EQUAL( 0, events.size() );
}

/**
 * Add Single Event Test
 *
 * Tests that we can add a single TCLK event with eventListAdd() and read it back with 
 * eventListGet().
 */
TEST( DecoderGroup, AddSingleEvent )
{
  _gDriver->eventListAdd( ITCLKDecoderDriver::TCLK_1HZ_EVENT );

  auto events = _gDriver->eventListGet();

  CHECK_EQUAL( 1, events.size() );
  CHECK_EQUAL( ITCLKDecoderDriver::TCLK_1HZ_EVENT, events[0] );
}

/**
 * Add Multiple Events Test
 *
 * Tests that we can add multiple TCLK events with eventListAdd() and read it back with 
 * eventListGet()
 */
TEST( DecoderGroup, AddMultipleEvents )
{
  _gDriver->eventListAdd( ITCLKDecoderDriver::TCLK_1HZ_EVENT );
  _gDriver->eventListAdd( ITCLKDecoderDriver::TCLK_15HZ_EVENT );
  _gDriver->eventListAdd( ITCLKDecoderDriver::TCLK_720HZ_EVENT );

  auto events = _gDriver->eventListGet();

  CHECK_EQUAL( 3, events.size() );
  CHECK( events.end() != 
         find( events.begin(), events.end(), ITCLKDecoderDriver::TCLK_1HZ_EVENT ) );
  CHECK( events.end() != 
         find( events.begin(), events.end(), ITCLKDecoderDriver::TCLK_15HZ_EVENT ) );
  CHECK( events.end() != 
         find( events.begin(), events.end(), ITCLKDecoderDriver::TCLK_720HZ_EVENT ) );
}

TEST( DecoderGroup, ExceptionOnTooManyEvents )
{
  try
    {
      for( ITCLKDecoderDriver::tclk_event_t event = 0; 
           event != _gDriver->eventListMaxGet() + 1; 
           event++ )
        {
          _gDriver->eventListAdd( event );
        }
    }
  catch( runtime_error e )
    {
      auto events = _gDriver->eventListGet();

      CHECK_EQUAL( _gDriver->eventListMaxGet(), events.size() );

      return;
    }

  FAIL( "Expected runtime_error!" );
}

TEST( DecoderGroup, DuplicateEventsAreIgnored )
{
  _gDriver->eventListAdd( ITCLKDecoderDriver::TCLK_1HZ_EVENT );
  _gDriver->eventListAdd( ITCLKDecoderDriver::TCLK_1HZ_EVENT );

  auto events = _gDriver->eventListGet();

  CHECK_EQUAL( 1, events.size() );
}
