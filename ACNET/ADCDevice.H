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

#include "../adc/IADCDriver.H"

using namespace FFF;
using namespace std;

namespace Mu2eER
{
  /**
   * ADCDevice
   *
   * Implements a ACSys/cdev interface to the ADC functions of Mu2eER.
   */
  class ADCDevice : public Device<32>
  {
  public:
    /**
     * Type of data returned by the Waveform reading property
     */
    typedef IADCDriver::waveform_sample_t waveform_read_t;

    /**
     * SSDN Attribute IDs
     */
    enum
      {
        ATTR_WAVEFORM_READ = 0x00
      };

    /**
     * Size of the ADC waveform buffer
     */
    static const unsigned int WAVEFORM_READ_MAX = 16000;

    /**
     * Maximum amount of data returned by the Waveform reading property
     */
    static const unsigned int WAVEFORM_READ_MAX_BYTES = 
      WAVEFORM_READ_MAX * sizeof( waveform_read_t );

    /**
     * Constructor
     *
     * @param driver ADC driver object
     */
    ADCDevice( unique_ptr<IADCDriver> driver );

    /**
     * Reading Property for Waveform data
     *
     * @param dest ACNET destination buffer
     * @param reqinfo ACNET request object
     */
    void waveformRead( Array<waveform_read_t>& dest, ReqInfo const* reqinfo );
    
  private:
    /**
     * The ADC driver
     */
    unique_ptr<IADCDriver> _adcDrv;
  };
};

#endif
