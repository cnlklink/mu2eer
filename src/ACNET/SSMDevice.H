/**
 * SSMDevice.H
 *
 * This file contains the definition of the SSMDevice class.
 *
 * @author jdiamond and rtadkins
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
     * Status & control device / Control property type
     */
    typedef uint32_t control_t;

    /**
     * Spill counter device / reading property type
     */
    typedef uint32_t spill_counter_read_t;

    /**
     * State device / reading property type
     */
    typedef uint16_t state_read_t;

    /**
     * Status & control device / Status property type
     */
    typedef uint32_t status_t;

    /**
     * Time-in-spill device / reading property type
     */
    typedef uint32_t tis_read_t;

    /**
     * Ideal spill device / reading property type
     */
    typedef uint32_t ideal_spill_read_t;

    /**
     * SSDN Attribute IDs
     */
    enum
      {
        ATTR_STATE_READING = 0x00,
        ATTR_SPILL_COUNTER_READING = 0x01,
        ATTR_STATUS_CONTROL = 0x02,
        ATTR_TIS_READING = 0x03,
        ATTR_IDEAL_SPILL_READING = 0x04
      };

    /**
     * SSM Device / Control property commands
     */
    enum
      {
        CONTROL_RESET = 0x00,
        CONTROL_START = 0x01,
        CONTROL_FAULT = 0x03
      };

    /**
     * Size of the spill counter device / reading property return buffer
     */
    static const unsigned int SPILL_COUNTER_READING_MAX = 1;

    /**
     * Size of the state device / reading property return buffer
     */
    static const unsigned int STATE_READING_MAX = 1;

    /**
     * Size of the time-in-spill device / reading property return buffer
     */
    static const unsigned int TIS_READING_MAX = 1;

    /**
     * Size of the ideal spill device / reading property return buffer
     */
    static const unsigned int IDEAL_SPILL_READING_MAX = 16000;

    /**
     * Constructor
     *
     * @param mqName mu2eerd control message queue name
     * @param shmName mu2eerd shared memory region name
     */
    SSMDevice( string mqName, string shmName );

    /**
     * Spill Counter / Reading Property
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void spillCounterRead( Array<spill_counter_read_t>& dest, ReqInfo const* reqinfo );

    /**
     * State Device / Reading Property
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void stateRead( Array<state_read_t>& dest, ReqInfo const* reqinfo );

    /**
     * Status & Control / Status property
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void statusCtrlRead( Array<status_t>& dest, ReqInfo const* reqinfo );

    /**
     * Status & Control / Control property
     *
     * @param src ACNET setting buffer
     * @param reqinfo ACNET request object
     */
    void statusCtrlWrite( Array<const control_t>& src, ReqInfo const* reqinfo );

    /**
     * Time-in-Spill Device / Reading Property
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void timeInSpillRead( Array<tis_read_t>& dest, ReqInfo const* reqinfo );

    /**
     * Ideal Spill / Reading Property
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void idealSpillRead( Array<ideal_spill_read_t>& dest, ReqInfo const* reqinfo );

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
