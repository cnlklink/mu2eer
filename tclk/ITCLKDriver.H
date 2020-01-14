/**
 * ITCLKDriver.H
 *
 * This file contains the definition of the TCLKDriver interface.
 *
 * @author jdiamond
 */

#ifndef ITCLKDRIVER_H
#define ITCLKDRIVER_H

#include <cstdint>

using namespace std;

namespace Mu2eER
{
  /**
   * ITCLKDriver
   *
   * Defines an interface to a TCLK decoder driver.
   */
  class ITCLKDriver
  {
  public:
    /**
     * TCLK Event Data Type
     */
    typedef uint8_t tclk_event_t;
  };
};

#endif
