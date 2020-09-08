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
#include <thread>

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
     * Deconstructor
     */
    ~SpillStateMachine();

    /**
     * Fault
     *
     * Place the Spill State Machine in the fault state (for testing / debugging).
     */
    void fault();

    /**
     * Initialize
     *
     * Initializes the spill state machine.
     */
    void initialize();

    /**
     * Reset
     *
     * Resets the spill state machine to the initial / idle state.
     */
    void reset();

    /**
     * Run
     *
     * Enter the SSM event loop.
     */
    void run();

    /**
     * Stop
     *
     * Interrupt and wait for the SSM thread to stop.
     */
    void stop();

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
     * SSM thread
     */
    unique_ptr<thread> _thread;

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
