/**
 * ITCLKDecoderDriver.H
 *
 * This file contains the definition of the TCLKDriver interface.
 *
 * @author jdiamond
 */

#ifndef ITCLKDECODERDRIVER_H
#define ITCLKDECODERDRIVER_H

#include <cstdint>
#include <vector>

using namespace std;

namespace Mu2eER
{
  /**
   * ITCLKDecoderDriver
   *
   * Defines an interface to a TCLK decoder driver.
   */
  class ITCLKDecoderDriver
  {
  public:
    /**
     * TCLK Event Data Type
     */
    typedef uint8_t tclk_event_t;

    /**
     * Some TCLK Event Constants
     */
    enum TCLK
      {
        TCLK_720HZ_EVENT = 0x07,
        TCLK_15HZ_EVENT = 0x0f,
        TCLK_1HZ_EVENT = 0x8f
      };

    /**
     * Destructor
     */
    virtual ~ITCLKDecoderDriver();

    /**
     * Add TCLK Event to TCLK Event List
     *
     * Adds a TCLK event to the list of events that we are interested in receiving.
     *
     * @param event Event to be added to the TCLK Event List
     */
    virtual void eventListAdd( tclk_event_t event ) = 0;

    /**
     * Get TCLK Event List
     *
     * Reads the active TCLK Event List from the hardware and returns it in a vector.
     *
     * @return TCLK Event List
     */
    virtual vector<tclk_event_t> eventListGet() const = 0;

    /**
     * Get Maximum Size of the TCLK Event List
     *
     * @return The maximum number of TCLK Events that we can subscribe to
     */
    virtual unsigned int eventListMaxGet() const = 0;

    /**
     * Wait For Event
     *
     * Waits for one of the events in the TCLK Event List to occur.
     *
     * @return TCLK event
     */
    virtual tclk_event_t waitForEvents() const = 0;
  };
};

#endif
