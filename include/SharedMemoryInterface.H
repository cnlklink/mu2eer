/**
 * SharedMemoryInterface.H
 *
 * This file contains the definition of the SharedMemoryInterface class.
 *
 * @author jdiamond
 */

#ifndef SHAREDMEMORYINTERFACE_H
#define SHAREDMEMORYINTERFACE_H

#include <cstring>
#include <string>

#include "api.H"
#include "mu2eerd.H"
#include "ssm.H"
#include "SpillStateMachineSMB.H"
#include "TCLKSMB.H"

using namespace std;

namespace Mu2eER
{
  /**
   * SharedMemoryInterface
   *
   * This class provides the layout of the shared memory segment and defines the interface between
   * mu2eerd and it's clients.
   */
  class SharedMemoryInterface
  {
  public:
    /**
     * Constructor
     */
    SharedMemoryInterface()
    {
      strncpy( _api_version, API_VERSION, strlen( API_VERSION ) );
    }

    /**
     * Get Config File
     *
     * @return Path to the active configuration file
     */
    inline string configFileGet() const
    {
      return string( _configFilePath );
    }

    /**
     * Set Config File
     *
     * @param path Path to the active configuration file
     */
    inline void configFileSet( string path )
    {
      path.copy( _configFilePath, path.size() + 1 );
      _configFilePath[path.size()] = '\0';
    }

    /**
     * Get Current State
     *
     * @return Current state of the mu2eerd daemon
     */
    inline mu2eerd_state_t currentStateGet() const
    {
      return _state;
    }

    /**
     * Set Current State
     *
     * @param state New state
     */
    inline void currentStateSet( mu2eerd_state_t state )
    {
      _state = state;
    }

    /**
     * Get PID
     *
     * @return PID for mu2eerd daemon
     */
    inline pid_t pidGet() const
    {
      return _pid;
    }

    /**
     * Set PID
     *
     * @param pid new PID
     */
    inline void pidSet( pid_t pid )
    {
      _pid = pid;
    }

    /**
     * Get Spill State Machine Block
     *
     * @return Reference to the SSM Block
     */
    inline SpillStateMachineSMB& ssmBlockGet()
    {
      return _ssmBlock;
    }

    /**
     * Get Read-only Spill State Machine Block
     *
     * @return Read-only reference to the SSM block
     */
    inline const SpillStateMachineSMB& ssmBlockGet() const
    {
      return _ssmBlock;
    }

    /**
     * Get mu2eerd Start Time
     *
     * @return Start time of the mu2eerd process
     */
    inline time_t startTimeGet() const
    {
      return _startTime;
    }

    /**
     * Set mu2eerd Start Time
     *
     * @param time Start time
     */
    inline void startTimeSet( time_t time )
    {
      _startTime = time;
    }

    /**
     * Get TCLK Block
     *
     * @return TCLKSMB Reference to the TCLK block
     */
    inline TCLKSMB& tclkBlockGet()
    {
      return _tclkBlock;
    }

    /**
     * Get TCLK Block
     *
     * @return TCLKSMB Read-only reference to the TCLK block
     */
    inline const TCLKSMB& tclkBlockGet() const
    {
      return _tclkBlock;
    }

    /**
     * Get Version
     *
     * @return API version string
     */
    inline string versionGet() const
    {
      return string( _api_version );
    }

  private:
    /**
     * API version
     */
    char _api_version[32];

    /**
     * Configuration file path
     */
    char _configFilePath[256];

    /**
     * mu2eerd pid
     */
    pid_t _pid;

    /**
     * Spill State Machine Block
     */
    SpillStateMachineSMB _ssmBlock;

    /**
     * mu2eerd start time
     */
    time_t _startTime;

    /**
     * mu2eerd state
     */
    mu2eerd_state_t _state;

    /**
     * TCLK block
     */
    TCLKSMB _tclkBlock;
  };
};

#endif
