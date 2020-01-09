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
 * Size of the Waveform Test Buffer
 */
static const unsigned int WAVEFORM_MAX = 512;

/**
 * Waveform Test Buffer
 */
static float WAVEFORM_BUF[WAVEFORM_MAX];

/**
 * WaveformGroup
 *
 * Tests related to the Waveform attribute.
 */
TEST_GROUP( WaveformGroup )
{
  void setup()
  {
    // Reset the waveform buffer to all 999.99's
    for( unsigned int i = 0; i != WAVEFORM_MAX; i++ )
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
  ADCDevice device;
  device.waveformRead( dest, &request );

  // Test
  CHECK( !isinf( dest[0] ) );
}

/**
 * WaveformGroup / Read Multiple Test
 *
 * Tests that the reading property returns multiple values for the Waveform attribute.
 */
TEST( WaveformGroup, ReadMultipleValuesTest )
{
  // Create destination buffer
  Array<float> dest( WAVEFORM_BUF, Index( 0 ), Count( WAVEFORM_MAX ) );

  // Create a request object
  ReqInfo request;

  // Read WAVEFORM_MAX values
  ADCDevice device;
  device.waveformRead( dest, &request );

  // Test
  for( unsigned int i = 0; i != WAVEFORM_MAX; i++ )
    {
      CHECK( !isinf( dest[i] ) );
    }
}
