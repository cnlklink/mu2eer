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
     * Type of data returned by the Waveform reading property
     */
    typedef float waveform_read_t;

    /**
     * Size of the ADC waveform buffer
     */
    static const unsigned int WAVEFORM_READ_MAX = 512;

    /**
     * Maximum amount of data returned by the Waveform reading property
     */
    static const unsigned int WAVEFORM_READ_MAX_BYTES = 
      WAVEFORM_READ_MAX * sizeof( waveform_read_t );

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
    void waveformRead( Array<waveform_read_t>& dest, ReqInfo const* reqinfo __attribute((unused)) );
    
  private:
  };
};

#endif
