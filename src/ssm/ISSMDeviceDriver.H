/**
 * ISSMDeviceDriver.H
 *
 * This file contains the definition of the ISSMDeviceDriver interface.
 *
 * @author jdiamond
 */

#ifndef ISSMDEVICEDRIVER_H
#define ISSMDEVICEDRIVER_H

#include <ostream>

using namespace std;

namespace Mu2eER
{
  /**
   * ISSMDeviceDriver
   *
   * Defines an interface for interacting with the FPGA Spill State Machine.  The FPGA Spill State Machine
   * generates a hardware interrupt when state transitions occur and provides a register for reading the
   * current state.
   */
  class ISSMDeviceDriver
  {
  public:
    /**
     * Spill State Machine State Definitions
     */
    enum state_t
      {
        STATE_IDLE = 0,
        STATE_INIT = 1,
        STATE_BETWEEN_CYCLES = 2,
        STATE_START_CYCLE = 3,
        STATE_BETWEEN_SPILLS = 4,
        STATE_READ_IBEAM = 5,
        STATE_RAMP = 6,
        STATE_SPILL = 7,
        STATE_START_EOS_TIMER = 8,
        STATE_SPILL_PARABOLA = 21,
        STATE_START_CLEANUP_TIMER = 19,
        STATE_AFTER_SPILL = 20,
        STATE_END_CYCLE = 22,
        STATE_LEARNING = 14,
        STATE_ABORT = 23,
        STATE_FAULT = 255
      };

    /**
     * Constructor
     */
    ISSMDeviceDriver();

    /**
     * Destructor
     */
    virtual ~ISSMDeviceDriver();

    /**
     * Read State
     *
     * Returns the current state of Spill State Machine.
     *
     * @return Current state
     */
    virtual state_t stateGet() const = 0;

    /**
     * Wait for State Change
     *
     * Blocks until a state change occurs and notification is received from the hardware.
     *
     * @return New state
     */
    virtual state_t waitForStateChange() = 0;

  private:
  };

  /**
   * Stream Insertion Operator
   *
   * Converts state to a string and insert it into the given stream.
   *
   * @param os Output stream
   * @param state State to convert
   * @return Output stream
   */
  ostream& operator<<( ostream& os, const ISSMDeviceDriver::state_t state );
};


#endif
