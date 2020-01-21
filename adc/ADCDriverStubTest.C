/**
 * ADCDriverStubTest.H
 *
 * This file contains unit tests (CppUTest) for the ADCDriverStub class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "ADCDriverStub.H"

using namespace Mu2eER;
using namespace std;

/**
 * ADCDriverStub object
 */
static ADCDriverStub _gADCDriver;

/**
 * ADCDriverStubGroup
 *
 * Tests related to the ADCDriverStub
 */
TEST_GROUP( ADCDriverStubGroup )
{
  void setup()
  {
  }

  void teardown()
  {
  }
};

/**
 * Copy Waveform Test
 *
 * Tests the waveformCopy function and verifies that it returns a linear ramp.
 */
TEST( ADCDriverStubGroup, CopyWaveform )
{
  // A buffer to copy the waveform into
  vector<ADCDriverStub::waveform_sample_t> buf;

  // Copy an entire waveform
  _gADCDriver.waveformCopy( buf, 0, ADCDriverStub::WAVEFORM_SAMPLES_MAX );

  // Veriy the correct number of samples were copied
  CHECK_EQUAL( ADCDriverStub::WAVEFORM_SAMPLES_MAX, buf.size() );

  // Verify that the samples increases linearly
  for( unsigned int i = 0; i != buf.size(); i++ )
    {
      CHECK_EQUAL( buf[i], static_cast<int>( i ) );
    }
}
