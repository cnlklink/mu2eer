/**
 * ADCDriverStub.H
 *
 * This file contains the definition of the ADCDriverStub class.
 *
 * @author jdiamond
 */

#ifndef ADCDRIVERSTUB_H
#define ADCDRIVERSTUB_H

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
     * @Override
     */
    virtual ~ADCDriverStub();

    /**
     * @Override
     */
    virtual void waveformCopy( unique_ptr<waveform_sample_t> dest, 
                               unsigned int offset, 
                               unsigned int n ) const;
  };
};

#endif
