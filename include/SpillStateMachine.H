/**
 * SpillStateMachine.H
 *
 * This file contains the definition of the SpillStateMachine class.
 *
 * @author jdiamond
 */

#ifndef SPILLSTATEMACHINE_H
#define SPILLSTATEMACHINE_H

#include <memory>

#include "ConfigurationManager.H"
#include "ISSMDeviceDriver.H"
#include "ssm.H"

using namespace std;

namespace Mu2eER
{
  /**
   * Spill State Machine
   *
   * This class provides a facade for the Spill State Machine (SSM) module.  The Spill State Machine
   * mooduleis responsible for interfacing and cooordinating with the Spill State Machine as
   * implemented in the FPGA firmware.
   */
  class SpillStateMachine
  {
  public:
    /**
     * Constructor
     *
     * @param cm Reference to the configuration manager
     */
    SpillStateMachine( const ConfigurationManager& cm );

    /**
     * Initialize
     *
     * Initializes the spill state machine.
     */
    void initialize();

    /**
     * Get State
     *
     * @return Current state
     */
    ssm_state_t stateGet() const;
    
  private:
    /**
     * Configuration manager
     */
    const ConfigurationManager& _cm;

    /**
     * Spill state machine device driver object
     */
    unique_ptr<ISSMDeviceDriver> _ssmDev;
  };
};

#endif
