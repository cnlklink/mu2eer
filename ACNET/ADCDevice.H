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
     * Constructor
     */
    ADCDevice();
    
  private:
  };
};

#endif
