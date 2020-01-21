/**
 * TCLKDecoderDriverMock.H
 *
 * This file contains the definition of the TCLKDecoderDriverMock class.
 *
 * @author jdiamond
 */

#ifndef TCLKDECODERDRIVERMOCK_H
#define TCLKDECODERDRIVERMOCK_H

#include "ITCLKDecoderDriver.H"

namespace Mu2eER
{
  /**
   * TCLKDecoderDriverMock
   *
   * Implements a mock that can stand-in for a TCLKDecoderDriver when testing in the development enivronment.
   */
  class TCLKDecoderDriverMock : public ITCLKDecoderDriver
  {
  public:
    /**
     * Destructor
     */
    virtual ~TCLKDecoderDriverMock();

    // Override
    virtual void eventListAdd( tclk_event_t event );

    // Override
    virtual vector<tclk_event_t> eventListGet() const;

    // Override
    virtual unsigned int eventListMaxGet() const;

    // Override
    virtual tclk_event_t waitForEvents() const;

  private:
    /**
     * Maximum capacity of the TCLK Event List
     */
    static const unsigned int EVENT_LIST_MAX = 16;

    /**
     * TCLK Event List
     */
    vector<tclk_event_t> _eventList;
  };
};

#endif
