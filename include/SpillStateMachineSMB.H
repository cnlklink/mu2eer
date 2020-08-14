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
#include "ssm.H"

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
     *
     */
    SpillStateMachineSMB( ssm_state_t _currentState, std::vector<unsigned int> _dataVector );

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
<<<<<<< HEAD
     * Get Sample Data Vector
     *
     * @return Sample data vector of the spill state machine
     */
    std::vector<unsigned int> currentStateDataGet() const;
=======
     * Get Spill Counter
     *
     * @return Spill counter
     */
    unsigned int spillCounterGet() const;

    /**
     * Set Spill Counter
     *
     * @param sc New spill counter value
     */
    void spillCounterSet( unsigned int sc );

    /**
     * Get Time-in-Spill
     *
     * @return Time spent in the last spill cycle (ms)
     */
    float timeInSpillGet() const;

    /**
     * Set Time-in-Spill
     *
     * @param tis New time-in-spill value
     */
    void timeInSpillSet( float tis );
>>>>>>> master

  private:
    /**
     * Current state
     */
    ssm_state_t _currentState;

    /**
<<<<<<< HEAD
     * Data vector
     */
    std::vector<unsigned int> _dataVector;
=======
     * Number of spills since the SSM was initialized
     */
    unsigned int _spillCounter;

    /**
     * Time spent in the last spill cycle (ms)
     */
    float _timeInSpill;
>>>>>>> master
  };
};

#endif
