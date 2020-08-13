/**
 * SpillStateMachineSMB.H
 *
 * This file contains the definition of the SpillStateMachineSMB class.
 *
 * @author jdiamond
 */

#include <vector>

#ifndef SPILLSTATEMACHINESMB_H
#define SPILLSTATEMACHINESMB_H
//#include "ssm.H"

using namespace std;

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
     * Initialize
     *
     * Initializes the shared memory.
     */
    void initialize();

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

    /**
     * Get Sample Data Vector
     *
     * @return Sample data vector of the spill state machine
     */
    std::vector<unsigned int> currentStateDataGet() const;


  private:
    /**
     * Current state
     */
    ssm_state_t _currentState;

    /**
     * Data vector
     */
    std::vector<unsigned int> _dataVector;
  };
};

#endif
