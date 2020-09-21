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
     * Daemon reading device type
     */
    typedef uint32_t daemon_read_t;

    /**
     * Daemon status & control device type
     */
    typedef uint32_t daemon_statusctrl_t;

    /**
     * SSDN Attribute IDs
     */
    enum
      {
        ATTR_DAEMON_READ = 0x00,
        ATTR_DAEMON_STATUSCTRL = 0x01
      };

    /**
     * Daemon Device / Control property commands
     */
    enum
      {
        DAEMON_CONTROL_SOFTREBOOT = 0x01,
        DAEMON_CONTROL_START = 0x02,
        DAEMON_CONTROL_STOP  = 0x00
      };

    /**
     * Daemon Device / Reading property array indeces
     */
    enum
      {
        DAEMON_READ_IDX_UPTIME = 0,
        DAEMON_READ_IDX_PID = 1,
        DAEMON_READ_IDX_JENKINS_NUM = 2
      };

    /**
     * Daemon Device / Status Property bit definitions
     */
    enum
      {
        // Bit 0 - daemon is running
        DAEMON_STATUS_RUNNING = 0x01
      };

    /**
     * Size of the daemon reading return buffer
     */
    static const unsigned int DAEMON_READ_MAX = 3;

    /**
     * Size of the daemon status & control return buffer
     */
    static const unsigned int DAEMON_STATUSCTRL_MAX = 1;

    /**
     * Daemon Device / Control property
     *
     * Process control commands for the Daemon device.
     *
     * @param src ACNET setting buffer
     * @param reqinfo ACNET request object
     */
    void daemonControl( Array<const daemon_statusctrl_t>& src, ReqInfo const* reqinfo );

    /**
     * Read Daemon Block
     *
     * Returns an array of information about the mu2eerd daemon to ACNET.
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void daemonRead( Array<daemon_read_t>& dest, ReqInfo const* reqinfo );

    /**
     * Daemon Device / Status Property
     *
     * Returns digital status for the Daemon device.
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void daemonStatus( Array<daemon_statusctrl_t>& dest, ReqInfo const* reqinfo );

    /**
     * Constructor
     *
     * @param daemonName Name of the mu2eerd process
     * @param mqName Name of the mu2eerd control queue
     * @param shmName Name of the mu2eerd shared memory region
     */
    Mu2eerdDevice( string daemonName, string mqName, string shmName );

  private:
    /**
     * mu2eerd process name
     */
    string _daemonName;

    /**
     * mu2eerd Control message queue name
     */
    string _mqName;

    /**
     * mu2eerd Shared memory name
     */
    string _shmName;

    /**
     * Daemon is Running
     *
     * @return True if the mu2eerd process is running
     */
    bool _daemonIsRunning() const;

    /**
     * Get Jenkins Number
     *
     * Reads and returns the value from /etc/jenkins_build_number.
     *
     * @return Jenkins build number
     */
    unsigned int _jenkinsNumberGet() const;
  };
};

#endif
