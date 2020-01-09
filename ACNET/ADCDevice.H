/**
 * ADCDevice.H
 *
 * This file contains the definition of the ADCDevice class.
 *
 * @author jdiamond
 */

#ifndef ADCDEVICE_H
#define ADCDEVICE_H

#include "device.h"

using namespace FFF;
using namespace std;

namespace Mu2eER
{
  class ADCDevice : public Device<32>
  {
  public:
    /**
     * Size of the ADC waveform buffer
     */
    static const unsigned int WAVEFORM_MAX = 512;

    /**
     * Constructor
     */
    ADCDevice();

    /**
     * Reading Property for Waveform data
     *
     * @param dest ACNET destination buffer
     * @param req ACNET request object
     */
    void waveformRead( Array<float>& dest, ReqInfo const* reqinfo __attribute((unused)) );
    
  private:
  };
};

#endif
