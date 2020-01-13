/**
 * IADCDriver.H
 *
 * This file contains the definition of the IADCDriver interface.
 *
 * @author jdiamond
 */

#ifndef IADCDRIVER_H
#define IADCDRIVER_H

#include <cstdint>
#include <memory>
#include <vector>

using namespace std;

namespace Mu2eER
{
  /**
   * IADCDriver
   *
   * Defines an interface for the ADC driver.
   */
  class IADCDriver
  {
  public:
    /**
     * Waveform Data Type
     */
    typedef int16_t waveform_sample_t;

    /**
     * Maximum number of waveform samples
     */
    static const unsigned int WAVEFORM_SAMPLES_MAX = 16000;

    /**
     * Destructor
     */
    virtual ~IADCDriver();
    /**
     * Copy Waveform Data
     *
     * Copies raw waveform data from the hardware to the given buffer.
     *
     * @param dest Destination buffer
     * @param offset Offset into the waveform data to begin copying from
     * @param n Number of waveform samples to copy
     */
    virtual void waveformCopy( vector<waveform_sample_t>& dest, 
                               unsigned int offset, 
                               unsigned int n ) const = 0;
  };
};

#endif
