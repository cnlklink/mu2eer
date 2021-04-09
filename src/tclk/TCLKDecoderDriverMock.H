/**
 * TCLKDecoderDriverMock.H
 *
 * This file contains the definition of the TCLKDecoderDriverMock class.
 *
 * @author jdiamond
 */

#ifndef TCLKDECODERDRIVERMOCK_H
#define TCLKDECODERDRIVERMOCK_H

#include <queue>

#include "ITCLKDecoderDriver.H"
#include "TCLKSMB.H"

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
     * Constructor
     */
    TCLKDecoderDriverMock();

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

    /**
     * Set TCLK Event Sequence
     *
     * Sets a sequence of TCLK events to be played back through calls to waitForEvents(..).
     */
    void eventSequenceSet( const vector<tclk_event_t>& sequence );

    // Override
    virtual string nameGet() const;

    // Override
    virtual tclk_event_t waitForEvents();

  private:
    /**
     * Maximum capacity of the TCLK Event List
     */
    static const unsigned int EVENT_LIST_MAX = 16;

    /**
     * TCLK Event List
     */
    vector<tclk_event_t> _eventList;

    /**
     * A sequence of TCLK events programmed by the test code to be played back to the API through
     * calls to waitForEvents(..)
     */
    deque<tclk_event_t> _eventSequence;
  };
};

#endif
