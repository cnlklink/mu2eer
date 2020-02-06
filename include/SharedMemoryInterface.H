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
#include "ssm.H"

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
    SharedMemoryInterface()
    {
      strncpy( _api_version, API_VERSION, strlen( API_VERSION ) );
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
     * Current state of the Spill State Machine
     */
    ssm_state_t _ssm_state;
  };
};

#endif
