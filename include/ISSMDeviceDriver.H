/**
 * ISSMDeviceDriver.H
 *
 * This file contains the definition of the ISSMDeviceDriver interface.
 *
 * @author jdiamond
 */

#ifndef ISSMDEVICEDRIVER_H
#define ISSMDEVICEDRIVER_H

#include <memory>
#include <ostream>
#include <string>

#include "ssm.H"

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
     * ISSMDeviceDriver Factory
     *
     * @param driverId Identifier for the type of device driver to create
     * @return Device driver (returns a SSMDeviceDriverMock if driverId is not found)
     */
    static unique_ptr<ISSMDeviceDriver> factory( string driverId );

    /**
     * Initialize firmware
     *
     * Send a message to the firmware to initlialize (or re-initialize).
     */
    virtual void initialize() = 0;

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
    virtual ssm_state_t stateGet() const = 0;

    /**
     * Wait for State Change
     *
     * Blocks until a state change occurs and notification is received from the hardware.
     *
     * @return New state
     */
    virtual ssm_state_t waitForStateChange() = 0;

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
  ostream& operator<<( ostream& os, const ssm_state_t state );
};


#endif
