/**
 * ADCDriverMock.C
 *
 * This file contains the definition of the ADCDriverMock class.
 *
 * @author jdiamond
 */

#include "ADCDriverStub.H"

using namespace Mu2eER;

ADCDriverStub::ADCDriverStub()
  : _waveformData( WAVEFORM_SAMPLES_MAX )
{
  // Initialize the waveform data with a linear ramp
  for( unsigned int i = 0; i != WAVEFORM_SAMPLES_MAX; i++ )
    {
      _waveformData[i] = i;
    }
}

ADCDriverStub::~ADCDriverStub()
{
}

void ADCDriverStub::waveformCopy( vector<waveform_sample_t>& dest, 
                                  unsigned int offset, 
                                  unsigned int n ) const
{
  for( unsigned int i = 0; i != n; i++ )
    {
      dest.push_back( offset + i );
    }
}
