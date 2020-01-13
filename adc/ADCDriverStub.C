/**
 * ADCDriverMock.C
 *
 * This file contains the definition of the ADCDriverMock class.
 *
 * @author jdiamond
 */

#include "ADCDriverStub.H"

using namespace Mu2eER;

ADCDriverStub::~ADCDriverStub()
{
}

void ADCDriverStub::waveformCopy( unique_ptr<waveform_sample_t> dest, 
                                  unsigned int offset, 
                                  unsigned int n ) const
{
  // TODO
}
