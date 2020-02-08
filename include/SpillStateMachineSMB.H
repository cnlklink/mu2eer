/**
 * SpillStateMachineSMB.H
 *
 * This file contains the definition of the SpillStateMachineSMB class.
 *
 * @author jdiamond
 */

#ifndef SPILLSTATEMACHINESMB_H
#define SPILLSTATEMACHINESMB_H

#include "ssm.H"

namespace Mu2eER
{
  /**
   * SpillStateMachineSMB
   *
   * Shared memory block for the spill state machine.
   */
  class SpillStateMachineSMB
  {
  public:
    /**
     * Constructor
     */
    SpillStateMachineSMB();

    /**
     * Set Current State
     *
     * @param state Current spill state machine state
     */
    void currentStateSet( ssm_state_t state );

    /**
     * Get Current State
     *
     * @return Current state of the spill state machine
     */
    ssm_state_t currentStateGet() const;

  private:
    /**
     * Current state
     */
    ssm_state_t _currentState;
  };
};

#endif
