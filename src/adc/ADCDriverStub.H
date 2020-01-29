/**
 * ADCDriverStub.H
 *
 * This file contains the definition of the ADCDriverStub class.
 *
 * @author jdiamond
 */

#ifndef ADCDRIVERSTUB_H
#define ADCDRIVERSTUB_H

#include <vector>

#include "IADCDriver.H"

namespace Mu2eER
{
  /**
   * ADCDriverStub
   *
   * Implements a stub of ADCDriver for testing.
   */
  class ADCDriverStub : public IADCDriver
  {
  public:
    /**
     * Constructor
     */
    ADCDriverStub();

    // Override
    virtual ~ADCDriverStub();

    // Override
    virtual void waveformCopy( vector<waveform_sample_t>& dest, 
                               unsigned int offset, 
                               unsigned int n ) const;

  private:
    /**
     * Waveform test data
     */
    vector<waveform_sample_t> _waveformData;
  };
};

#endif
