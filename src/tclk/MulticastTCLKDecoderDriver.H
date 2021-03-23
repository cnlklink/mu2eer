/**
 * MulticastTCLKDecoderDriver.H
 *
 * This file contains the definition of the MulticastTCLKDecoderDriver class.
 *
 * @author jdiamond
 */

#ifndef MULTICASTTCLKDECODERDRIVER_H
#define MULTICASTTCLKDECODERDRIVER_H

#include <cstdio>
#include <cstdint>
#include <ctime>
#include <mcastrecv.h>

#include "ITCLKDecoderDriver.H"

namespace Mu2eER
{
  /**
   * MulticastTCLKDecoderDriver
   *
   * Implements a TCLK decoder that utilizes the multicast event broadcast available on AD networks.  This class
   * wraps the functionality of the libmcastrecv library and adapts it to the ITCLKDecoderDriver interface.
   */
  class MulticastTCLKDecoderDriver : public ITCLKDecoderDriver
  {
  public:
    /**
     * Constructor
     */
    MulticastTCLKDecoderDriver();

    /**
     * Desctructor
     */
    ~MulticastTCLKDecoderDriver();

  private:
    /**
     * Priority for the libmcastrecv thread
     */
    const unsigned int MCAST_THREAD_PRIORITY = 95;
  };
};

#endif
