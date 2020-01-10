/**
 * ADCDeviceTest.H
 *
 * This file contains unit tests (CppUTest) for the ADCDevice class.
 *
 * @author jdiamond
 */

#include <cmath>
#include <iostream>

#include "CppUTest/TestHarness.h"

#include "ADCDevice.H"

using namespace Mu2eER;
using namespace FFF;
using namespace std;

/**
 * ADCDevice object
 */
static ADCDevice DEVICE;

/**
 * Waveform Test Buffer
 */
static float WAVEFORM_BUF[ADCDevice::WAVEFORM_READ_MAX];

/**
 * Reset Waveform Buffer
 *
 * Sets every element in WAVEFORM_BUF to INFINITY.
 */
static void _bufferReset()
{
  for( unsigned int i = 0; i != ADCDevice::WAVEFORM_READ_MAX; i++ )
    {
      WAVEFORM_BUF[i] = INFINITY;
    }
}

/**
 * Check for No Infinity
 *
 * Scans the waveform buffer for an elements equal to INFINITY.
 *
 * @param buf Reference to buffer
 * @param start Start index
 * @param count Number of elements to check
 * @return True if NO INFINITY is found
 */
static bool _checkForNoInfinity( Array<float> const& buf, unsigned int start, unsigned int count )
{
  for( unsigned int i = start; i != (start + count); i++ )
    {
      if( isinf( buf[i] ) )
        {
          return false;
        }
    }

  // No INFINITY found
  return true;
}

/**
 * WaveformGroup
 *
 * Tests related to the Waveform attribute.
 */
TEST_GROUP( WaveformGroup )
{
  void setup()
  {
    _bufferReset();
  }

  void teardown()
  {
  }
};

/**
 * WaveformGroup / ReadFirstValueTest
 *
 * Tests the reading property for the Waveform attribute.
 */
TEST( WaveformGroup, ReadFirstValueTest )
{
  // Create destination buffer
  Array<ADCDevice::waveform_read_t> dest( WAVEFORM_BUF, Index( 0 ), Count( 1 ) );

  // Create a request object
  ReqInfo request;

  // Read one value
  DEVICE.waveformRead( dest, &request );

  // Test
  CHECK( _checkForNoInfinity( dest, 0, 1 ) );
}

/**
 * WaveformGroup / Read All Test
 *
 * Tests that the reading property returns all values for the Waveform attribute.
 */
TEST( WaveformGroup, ReadAllValuesTest )
{
  ReqInfo request;

  // Read WAVEFORM_READ_MAX values
  Array<ADCDevice::waveform_read_t> dest( WAVEFORM_BUF, 
                                          Index( 0 ), 
                                          Count( ADCDevice::WAVEFORM_READ_MAX ) );
  DEVICE.waveformRead( dest, &request );

  CHECK( _checkForNoInfinity( dest, 0, ADCDevice::WAVEFORM_READ_MAX ) );
}

/**
 * WaveformGroup / Read Some from Test
 *
 * Tests that the reading property handles a requests for a slice of the waveform from the middle.
 */
TEST( WaveformGroup, ReadSomeValuesInMiddleTest )
{
  ReqInfo request;

  // Read multiple values from an offset of 100 into the waveform
  Array<ADCDevice::waveform_read_t> dest( WAVEFORM_BUF, Index( 100 ), Count( 100 ) );
  DEVICE.waveformRead( dest, &request );

  CHECK( _checkForNoInfinity( dest, 0, dest.total.getValue() ) );
}

/**
 * WaveformGroup / Read Some to End Test
 *
 * Tests that the reading property handles a request for a slice of the waveform up to the end.
 */
TEST( WaveformGroup, ReadSomeValuesToEndTest )
{
  ReqInfo request;

  // Read multiple values just up to the end of the waveform
  Array<ADCDevice::waveform_read_t> dest( WAVEFORM_BUF, 
                                          Index( ADCDevice::WAVEFORM_READ_MAX - 10 ), 
                                          Count( 10 ) );
  DEVICE.waveformRead( dest, &request );

  CHECK( _checkForNoInfinity( dest, 0, dest.total.getValue() ) );
}

/**
 * WaveformGroup / Read Out-of-Bounds Offset Test
 *
 * Tests that the reading property handles requests with invalid index & count parameters.
 */
TEST( WaveformGroup, ReadOutOfBoundsOffsetTest )
{
  ReqInfo request;

  try
    {
      // Create destination buffer with an Index outside of the expected range and read 
      // waveform data
      Array<ADCDevice::waveform_read_t> dest( WAVEFORM_BUF, 
                                              Index( ADCDevice::WAVEFORM_READ_MAX + 1 ), 
                                              Count( 1 ) );
      DEVICE.waveformRead( dest, &request );
    }
  catch( runtime_error e )
    {
      // Expected runtime_error to be thrown
      return;
    }

  // Test
  FAIL( "should have thrown runtime_error" );
}

/**
 * WaveformGroup / Read Out-of-Bounds Count Test
 *
 * Tests that the reading property handles requests with an out-of-bounds count parameter.
 */
TEST( WaveformGroup, ReadOutOfBoundsCountTest )
{
  ReqInfo request;

  try
    {
      // Offset is within range but count extends out of range
      Array<ADCDevice::waveform_read_t> dest( WAVEFORM_BUF, 
                                              Index( 1 ), 
                                              Count( ADCDevice::WAVEFORM_READ_MAX ) );
      DEVICE.waveformRead( dest, &request );
    }
  catch( runtime_error e )
    {
      // Expected runtime_error to be thrown
      return;
    }

  FAIL( "should have thrown runtime_error" );
}
