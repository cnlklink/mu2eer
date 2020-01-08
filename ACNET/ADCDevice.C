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
