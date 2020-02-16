/**
 * Mu2eerdDevice.H
 *
 * This file contains the definition of the Mu2eerdDevice class.
 *
 * @author jdiamond
 */

#ifndef MU2EERDDEVICE_H
#define MU2EERDDEVICE_H

#include <string>

#include "device.h"

#include "ControlMQClient.H"
#include "SharedMemoryClient.H"

using namespace FFF;
using namespace std;

namespace Mu2eER
{
  /**
   * Mu2eerdDevice
   *
   * Implements an ACSys/cdev interface to mu2eerd.
   */
  class Mu2eerdDevice : public Device<32>
  {
  public:
    /**
     * State read device type
     */
    typedef uint16_t state_read_t;

    /**
     * SSDN Attribute IDs
     */
    enum
      {
        ATTR_STATE_READ = 0x00
      };

    /**
     * Size of the state return buffer
     */
    static const unsigned int STATE_READ_MAX = 1;

    /**
     * Constructor
     *
     * @param mqName Name of the mu2eerd control queue
     * @param shmName Name of the mu2eerd shared memory region
     */
    Mu2eerdDevice( string mqName, string shmName );

    /**
     * State Device Reading Property
     *
     * @param dest ACNET response buffer
     * @param reqinfo Reference to ACNET request object
     */
    void stateRead( Array<state_read_t>& dest, ReqInfo const* reqinfo );

  private:
    /**
     * mu2eerd Control message queue name
     */
    string _mqName;

    /**
     * mu2eerd Shared memory name
     */
    string _shmName;
  };
};

#endif
