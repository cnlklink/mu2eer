/**
 * SharedMemoryClient.H
 *
 * This file contains the definition of the SharedMemoryClient class.
 *
 * @author jdiamond
 */

#ifndef SHAREDMEMORYCLIENT_H
#define SHAREDMEMORYCLIENT_H

#include <chrono>
#include <thread>

#include "api.H"
#include "mu2eerd.H"
#include "SharedMemoryInterface.H"

using namespace std;

namespace Mu2eER
{
  /**
   * SharedMemoryClient
   *
   * Implements the shared memory API for communication with mu2eerd.
   */
  class SharedMemoryClient
  {
  public:
    /**
     * Get Config File
     *
     * @return Path to the active configuration file
     */
    string configFileGet() const;

    /**
     * Get Current State
     *
     * @return State of the mu2eerd daemon
     */
    mu2eerd_state_t currentStateGet() const;

    /**
     * Get PID
     *
     * @return PID of the mu2eerd daemon
     */
    pid_t pidGet() const;

    /**
     * Constructor
     *
     * @param name Name of the shared memory segment to attach to
     */
    SharedMemoryClient( const string& name );

    /**
     * Destructor
     */
    ~SharedMemoryClient();

    /**
     * Get Read-only SSM Block
     *
     * @return Const reference to the Spill State Machine block.
     */
    const SpillStateMachineSMB& ssmBlockGet() const;

    /**
     * Get Start Time for mu2eerd
     *
     * @return Start time for the mu2eerd process
     */
    time_t startTimeGet() const;

    /**
     * Get Read-only TCLK Block
     *
     * @return Const reference to the TCLK block.
     */
    const TCLKSMB& tclkBlockGet() const;

    /**
     * Get Uptime
     *
     * Using the start time recorded in the shared memory block, calculates and returns the
     * uptime for mu2eerd.
     *
     * @return Uptime for mu2eerd (in seconds)
     */
    time_t uptimeGet() const;

    /**
     * Get API Version
     *
     * @return API version string
     */
    string versionGet() const;

    /**
     * Wait For State
     *
     * Wait for mu2eerd to enter the given state.
     *
     * @param waitForState State to wait for
     * @param interval How long to wait in ms
     * @param tries Number of tries before giving up (0: try forever, timeout = interval * tries)
     */
    void waitForState( mu2eerd_state_t waitForState,
                       unsigned int interval = 5,
                       unsigned int tries = 0 ) const;

    /**
     * Wait For Spill State Machine
     *
     * Wait for the spill state machine to enter the given state.
     *
     * @param waitForState State to wait for
     * @param interval How long to wait in ms
     * @param tries Number of tries before giving up (0: try forever, timeout = interval * tries)
     */
    void waitForSSMState( ssm_state_t waitForState,
                          unsigned int interval = 5,
                          unsigned int tries = 0 ) const;

  private:
    /**
     * Shared memory file descriptor
     */
    int _fd;

    /**
     * Shared memory region's name
     */
    string _name;

    /**
     * Pointer to the shared memory region
     */
    void* _ptr;

    /**
     * _ptr cast to a SharedMemoryInterface*
     */
    SharedMemoryInterface* _shmi;

    /**
     * The size of the shared memory region
     */
    unsigned long int _size;
  };
};

#endif
