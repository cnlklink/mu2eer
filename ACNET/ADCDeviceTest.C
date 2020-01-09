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
static float WAVEFORM_BUF[ADCDevice::WAVEFORM_MAX];

/**
 * WaveformGroup
 *
 * Tests related to the Waveform attribute.
 */
TEST_GROUP( WaveformGroup )
{
  void setup()
  {
    // Reset the waveform buffer to all INFINITY!
    for( unsigned int i = 0; i != ADCDevice::WAVEFORM_MAX; i++ )
      {
        WAVEFORM_BUF[i] = INFINITY;
      }
  }

  void teardown()
  {
  }
};

/**
 * WaveformGroup / ReadTest
 *
 * Tests the reading property for the Waveform attribute.
 */
TEST( WaveformGroup, ReadFirstValueTest )
{
  // Create destination buffer
  Array<float> dest( WAVEFORM_BUF, Index( 0 ), Count( 1 ) );

  // Create a request object
  ReqInfo request;

  // Read one value
  DEVICE.waveformRead( dest, &request );

  // Test
  CHECK( !isinf( dest[0] ) );
}

/**
 * WaveformGroup / Read All Test
 *
 * Tests that the reading property returns all values for the Waveform attribute.
 */
TEST( WaveformGroup, ReadAllValuesTest )
{
  // Create destination buffer
  Array<float> dest( WAVEFORM_BUF, Index( 0 ), Count( ADCDevice::WAVEFORM_MAX ) );

  // Create a request object
  ReqInfo request;

  // Read WAVEFORM_MAX values
  DEVICE.waveformRead( dest, &request );

  // Test
  for( unsigned int i = 0; i != ADCDevice::WAVEFORM_MAX; i++ )
    {
      CHECK( !isinf( dest[i] ) );
    }
}

/**
 * WaveformGroup / Read Some from Middle Test
 *
 * Tests that the reading property handles a requests for a slice of the waveform from the middle.
 */
TEST( WaveformGroup, ReadSomeValuesInMiddleTest )
{
  // Create a request object
  ReqInfo request;

  // Read multiple values from an offset of 100 into the waveform
  Array<float> dest( WAVEFORM_BUF, Index( 100 ), Count( 100 ) );
  DEVICE.waveformRead( dest, &request );

  // Test
  for( unsigned int i = 0; i != dest.total.getValue(); i++ )
    {
      CHECK( !isinf( dest[i] ) );
    }
}

/**
 * WaveformGroup / Read Some to End Test
 *
 * Tests that the reading property handles a request for a slice of the waveform up to the end.
 */
TEST( WaveformGroup, ReadSomeValuesToEndTest )
{
  // Create a request object
  ReqInfo request;

  // Read multiple values just up to the end of the waveform
  Array<float> dest( WAVEFORM_BUF, Index( ADCDevice::WAVEFORM_MAX - 10 ), Count( 10 ) );
  DEVICE.waveformRead( dest, &request );

  // Test
  for( unsigned int i = 0; i != dest.total.getValue(); i++ )
    {
      CHECK( !isinf( dest[i] ) );
    }
}

/**
 * WaveformGroup / Read Out-of-Bounds Offset Test
 *
 * Tests that the reading property handles requests with invalid index & count parameters.
 */
TEST( WaveformGroup, ReadOutOfBoundsOffsetTest )
{
  // Create a request object
  ReqInfo request;

  try
    {
      // Create destination buffer with an Index outside of the expected range and read 
      // waveform data
      Array<float> dest( WAVEFORM_BUF, Index( ADCDevice::WAVEFORM_MAX + 1 ), Count( 1 ) );
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
  // Create a request object
  ReqInfo request;

  try
    {
      // Offset is within range but count extends out of range
      Array<float> dest( WAVEFORM_BUF, Index( 1 ), Count( ADCDevice::WAVEFORM_MAX ) );
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
