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
    // Override
    virtual void initialize();

    /**
     * Default Constructor
     */
    SSMDeviceDriverMock();

    /**
     * Constructor from State Sequence
     *
     * Initializes the state sequence during construction.
     *
     * @param sequence State sequence
     */
    SSMDeviceDriverMock( const vector<ssm_state_t>& sequence );

    /**
     * Destructor
     */
    virtual ~SSMDeviceDriverMock();

    // Override
    virtual ssm_state_t stateGet() const;

    /**
     * State Sequence Set
     *
     * Set the sequence of states to be returned by waitForStateChange(..).
     *
     * @param sequence Sequence of states, 0 being the first
     */
    virtual void stateSequenceSet( const vector<ssm_state_t>& sequence );

    // Override
    virtual ssm_state_t waitForStateChange();

  private:
    /**
     * Initial state sequence.  _stateSequence can be reset to this by calling initialize().
     */
    vector<ssm_state_t> _initialSequence;

    /**
     * Current state
     */
    ssm_state_t _state;

    /**
     * A sequence of state changes to be played back by calls to waitForStateChange(..).
     */
    deque<ssm_state_t> _stateSequence;

    /**
     * Reset Sequence
     *
     * Resets _stateSequence to _initialSequence.
     */
    void _resetSequence();

    /**
     * Next State
     *
     * Advances to the next state in the sequence.  If the sequence is empty, set to FAULT.
     *
     * @return The next state in the sequence
     */
    ssm_state_t _stateNext();
  };
};

#endif
