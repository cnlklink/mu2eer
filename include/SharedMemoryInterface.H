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
#include "SpillStateMachineSMB.H"

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
     * Spill State Machine Block
     */
    SpillStateMachineSMB _ssmBlock;
  };
};

#endif
