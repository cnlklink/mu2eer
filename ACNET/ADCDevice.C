/**
 * ADCDevice.C
 *
 * This file contains the implementation of the ADCDevice class.
 *
 * @author jdiamond
 */

#include "ADCDevice.H"

using namespace Mu2eER;

ADCDevice::ADCDevice() 
  : Device<32>( "ADCDevice", "Mu2eER ADC Device" )
{
  // Register read/write methods for each attribute
}

void ADCDevice::waveformRead( Array<float>& dest, ReqInfo const* reqinfo __attribute((unused)) )
{
  dest[0] = 0.0;
  for( unsigned int i = 0; i != dest.total.getValue(); i++ )
    {
      dest[i] = i * 1.1;
    }
}
