/**
 * ADCDevice.C
 *
 * This file contains the implementation of the ADCDevice class.
 *
 * @author jdiamond
 */

#include <cmath>
#include <vector>

#include "../adc/IADCDriver.H"

#include "ADCDevice.H"

using namespace Mu2eER;

/**
 * Copy To ACNET
 *
 * Copies from a vector of type T into the ACNET Array<> buffer
 *
 * @param dest ACNET destination buffer
 * @param vec Source vector
 */
template<typename T> 
void copyToACNETArray( Array<T>& dest, vector<T>& vec )
{
  unsigned int i = 0;
  for( auto& v : vec )
    {
      dest[i++] = v;
    }
}

ADCDevice::ADCDevice( unique_ptr<IADCDriver> adcDrv ) 
  : Device<32>( "ADCDevice", "Mu2eER ADC Device" ),
    _adcDrv( move( adcDrv ) )
{
  // Register read/write methods for each attribute
  registerMethod( ATTR_WAVEFORM_READ, *this, &ADCDevice::waveformRead, WAVEFORM_READ_MAX );
}

void ADCDevice::waveformRead( Array<waveform_read_t>& dest, ReqInfo const* reqinfo __attribute((unused)) )
{
  if( dest.offset.getValue() > WAVEFORM_READ_MAX )
    {
      throw runtime_error( "Bad offset" );
    }

  if( (dest.offset.getValue() + dest.total.getValue()) > WAVEFORM_READ_MAX )
    {
      throw runtime_error( "Bad offset + count" );
    }

  // Copy waveform data to ACNET response buffer
  vector<waveform_read_t> v;
  _adcDrv->waveformCopy( v, dest.offset.getValue(), dest.total.getValue() );
  copyToACNETArray<waveform_read_t>( dest, v );
}
