/**
 * ADCDeviceTest.H
 *
 * This file contains unit tests (CppUTest) for the ADCDevice class.
 *
 * @author jdiamond
 */

#include <cmath>
#include <iostream>
#include <limits>

#include "CppUTest/TestHarness.h"

#include "../adc/ADCDriverStub.H"
#include "ADCDevice.H"

using namespace Mu2eER;
using namespace FFF;
using namespace std;

/**
 * gWaveformBuf is initialized to this value
 */
static const ADCDevice::waveform_read_t WAVEFORM_BUF_INIT_VALUE = 99;

/**
 * ADCDevice object
 */
static unique_ptr<ADCDevice> _gDevice( new ADCDevice( unique_ptr<IADCDriver>( new ADCDriverStub() ) ) );

/**
 * Waveform Test Buffer
 */
static ADCDevice::waveform_read_t _gWaveformBuf[ADCDevice::WAVEFORM_READ_MAX];

/**
 * Reset Waveform Buffer
 *
 * Sets every element in _gWaveformBuf to the maximum finit value.
 */
static void _bufferReset()
{
  for( unsigned int i = 0; i != ADCDevice::WAVEFORM_READ_MAX; i++ )
    {
      _gWaveformBuf[i] = WAVEFORM_BUF_INIT_VALUE;
    }
}

/**
 * Check for Linear Ramp
 *
 * Verifies that the waveform elements returned by the driver are arranged in a linear ramp.
 *
 * @param buf Reference to buffer
 * @param start Start index
 * @param count Number of elements to check
 * @return True if elements form a linear ramp
 */
static bool _checkForLinearRamp( Array<ADCDevice::waveform_read_t> const& buf, 
                                 unsigned int start, 
                                 unsigned int count )
{
  auto last = buf[start];
  for( unsigned int i = start; i != (start + count); i++ )
    {
      if( i == start )
        continue;

      if( buf[i] != last + 1 )
        return false;

      last = buf[i];
    }

  // Elements form a linear ramp
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
 * WaveformGroup / Read All Test
 *
 * Tests that the reading property returns all values for the Waveform attribute.
 */
TEST( WaveformGroup, ReadAllValuesTest )
{
  ReqInfo request;

  // Read WAVEFORM_READ_MAX values
  Array<ADCDevice::waveform_read_t> dest( _gWaveformBuf, 
                                          Index( 0 ), 
                                          Count( ADCDevice::WAVEFORM_READ_MAX ) );
  _gDevice->waveformRead( dest, &request );

  CHECK( _checkForLinearRamp( dest, 0, ADCDevice::WAVEFORM_READ_MAX ) );
}

/**
 * WaveformGroup / ReadFirstValueTest
 *
 * Tests the reading property for the Waveform attribute.
 */
TEST( WaveformGroup, ReadFirstValueTest )
{
  // Create destination buffer
  Array<ADCDevice::waveform_read_t> dest( _gWaveformBuf, Index( 0 ), Count( 1 ) );

  // Create a request object
  ReqInfo request;

  // Read one value
  _gDevice->waveformRead( dest, &request );

  // Test
  CHECK( dest[0] != WAVEFORM_BUF_INIT_VALUE );
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
      Array<ADCDevice::waveform_read_t> dest( _gWaveformBuf, 
                                              Index( 1 ), 
                                              Count( ADCDevice::WAVEFORM_READ_MAX ) );
      _gDevice->waveformRead( dest, &request );
    }
  catch( AcnetError e )
    {
      CHECK( e.getValue() == Ex_BADOFLEN.getValue() );
      return;
    }

  FAIL( "should have thrown runtime_error" );
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
      Array<ADCDevice::waveform_read_t> dest( _gWaveformBuf, 
                                              Index( ADCDevice::WAVEFORM_READ_MAX + 1 ), 
                                              Count( 1 ) );
      _gDevice->waveformRead( dest, &request );
    }
  catch( AcnetError e )
    {
      CHECK( e.getValue() == Ex_BADOFF.getValue() );
      return;
    }

  // Test
  FAIL( "should have thrown Ex_BADOFLEN" );
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
  Array<ADCDevice::waveform_read_t> dest( _gWaveformBuf, Index( 100 ), Count( 100 ) );
  _gDevice->waveformRead( dest, &request );

  CHECK( _checkForLinearRamp( dest, 0, dest.total.getValue() ) );
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
  Array<ADCDevice::waveform_read_t> dest( _gWaveformBuf, 
                                          Index( ADCDevice::WAVEFORM_READ_MAX - 10 ), 
                                          Count( 10 ) );
  _gDevice->waveformRead( dest, &request );

  CHECK( _checkForLinearRamp( dest, 0, dest.total.getValue() ) );
}
