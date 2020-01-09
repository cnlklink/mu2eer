/**
 * ADCDeviceTest.H
 *
 * This file contains unit tests (CppUTest) for the ADCDevice class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "ADCDevice.H"

using namespace Mu2eER;
using namespace FFF;
using namespace std;

/**
 * WaveformGroup
 *
 * Tests related to the Waveform attribute.
 */
TEST_GROUP( WaveformGroup )
{
};

/**
 * WaveformGroup / ReadTest
 *
 * Tests the reading property for the Waveform attribute.
 */
TEST( WaveformGroup, ReadFirstValueTest )
{
  // Create destination buffer
  float buf[1];
  Array<float> dest( buf, Index( 0 ), Count( 1 ) );
  dest[0] = 999.99;

  // Create a request object
  ReqInfo request;

  // Read one value
  ADCDevice device;
  device.waveformRead( dest, &request );

  // Test
  CHECK_EQUAL( 0.0, dest[0] );
}
