/**
 * SSMDevice.H
 *
 * This file contains the definition of the SSMDevice class.
 *
 * @author jdiamond
 */

#ifndef SSMDEVICE_H
#define SSMDEVICE_H

#include "device.h"

#include "ControlMQClient.H"
#include "SharedMemoryClient.H"

using namespace FFF;
using namespace std;

namespace Mu2eER
{
  /**
   * SSMDevice
   *
   * Implements an ACSys/cdev interface to the Spill State Machine (SSM) module.
   */
  class SSMDevice : public Device<32>
  {
  public:
    /**
     * State device / reading property type
     */
    typedef uint16_t state_read_t;

    /**
     * SSDN Attribute IDs
     */
    enum
      {
        ATTR_STATE_READING = 0x00
      };

    /**
     * Size of the state device / reading property return buffer
     */
    static const unsigned int STATE_READING_MAX = 1;

    /**
     * Constructor
     *
     * @param mqName mu2eerd control message queue name
     * @param shmName mu2eerd shared memory region name
     */
    SSMDevice( string mqName, string shmName );

    /**
     * State Device / Reading Property
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void stateRead( Array<state_read_t>& dest, ReqInfo const* reqinfo );

  private:
    /**
     * Name of the mu2eerd control message queue
     */
    string _mqName;

    /**
     * Name of the mu2eerd shared memory region
     */
    string _shmName;
  };
};

#endif