/**
 * SSMDeviceDriverMock.H
 *
 * This file contains the definition of the SSMDeviceDriverMock class.
 *
 * @author jdiamond
 */

#ifndef SSMDEVICEDRIVERMOCK_H
#define SSMDEVICEDRIVERMOCK_H

#include <deque>
#include <vector>

#include "ISSMDeviceDriver.H"

namespace Mu2eER
{
  /**
   * SSMDeviceDriverMock
   *
   * A Mock that can stand in for SSMDeviceDriver during testing.
   */
  class SSMDeviceDriverMock : public ISSMDeviceDriver
  {
  public:
    /**
     * Constructor
     */
    SSMDeviceDriverMock();

    /**
     * Destructor
     */
    virtual ~SSMDeviceDriverMock();

    // Override
    virtual state_t stateGet() const;

    /**
     * State Sequence Set
     *
     * Set the sequence of states to be returned by waitForStateChange(..).
     *
     * @param sequence Sequence of states, 0 being the first
     */
    virtual void stateSequenceSet( const vector<ISSMDeviceDriver::state_t>& sequence );

    // Override
    virtual state_t waitForStateChange();

  private:
    /**
     * Current state
     */
    state_t _state;

    /**
     * A sequence of state changes to be played back by calls to waitForStateChange(..).
     */
    deque<state_t> _stateSequence;
  };
};

#endif
