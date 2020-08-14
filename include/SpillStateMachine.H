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
#include "SpillStateMachineSMB.H"
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
     * @param smb Reference to the SSM shared memory block
     */
    SpillStateMachine( const ConfigurationManager& cm, SpillStateMachineSMB& smb );

    /**
     * Initialize
     *
     * Initializes the spill state machine.
     */
    void initialize();

    /**
     * Run
     *
     * Enter the SSM event loop.
     */
    void run();

<<<<<<< HEAD
    /**
     * Get State Machine Shared Memory Block
     *
     * @return Current shared memory block
     */
    SpillStateMachineSMB& sharedMemoryGet() const;

    /**
     * Wait for State Change
     *
     * Wait for the spill state machine to change state.
     *
     * @return New state
     */
    ssm_state_t waitForStateChange() const;
    
=======
>>>>>>> master
  private:
    /**
     * Configuration manager
     */
    const ConfigurationManager& _cm;

    /**
     * Spill State Machine shared memory block
     */
    SpillStateMachineSMB& _smb;

    /**
     * Spill state machine device driver object
     */
    unique_ptr<ISSMDeviceDriver> _ssmDev;

    /**
     * Update the Shared Memory Block
     *
     * Publish the state of the SSM to shared memory.
     */
    void _smbUpdate();

    /**
     * Wait for State Change
     *
     * Wait for the spill state machine to change state.
     *
     * @return New state
     */
    ssm_state_t _waitForStateChange() const;    
  };
};

#endif
