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
#include "ISystemController.H"
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
     * Config reading device type
     */
    typedef char config_read_t;

    /**
     * Daemon reading device type
     */
    typedef uint32_t daemon_read_t;

    /**
     * Daemon status & control device type
     */
    typedef uint32_t daemon_statusctrl_t;

    /**
     * Startup setting device type
     */
    typedef uint32_t startup_set_t;

    /**
     * SSDN Attribute IDs
     */
    enum
      {
        ATTR_DAEMON_READ = 0x00,
        ATTR_DAEMON_STATUSCTRL = 0x01,
        ATTR_STARTUP_SET = 0x02,
        ATTR_CONFIG_READ = 0x03
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
        DAEMON_STATUS_RUNNING = 0x01,

        // Bit 1 - shared memory is connected
        DAEMON_STATUS_SHM_CONNECTED = 0x02,

        // Bit 2 - command queue is connected
        DAEMON_STATUS_CMQ_CONNECTED = 0x04
      };

    /**
     * Startup Device / Setting enum definitions
     *   Off     - Do not start mu2eerd on startup
     *   On_Test - Start mu2eerd w/ test configuration
     *   On_Oper - Start mu2eerd w/ operational configuration
     */
    enum
      {
        STARTUP_OFF = 0,
        STARTUP_ON_TEST = 1,
        STARTUP_ON_OPER = 2
      };

    /**
     * Size of the config reading buffer
     */
    static const unsigned int CONFIG_READ_MAX = 255;

    /**
     * Size of the daemon reading return buffer
     */
    static const unsigned int DAEMON_READ_MAX = 3;

    /**
     * Size of the daemon status & control return buffer
     */
    static const unsigned int DAEMON_STATUSCTRL_MAX = 1;

    /**
     * Size of the startup setting device array
     */
    static const unsigned int STARTUP_SET_MAX = 1;

    /**
     * Read Config
     *
     * Returns the configuration file path to ACNET.
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void configRead( Array<config_read_t>& dest, ReqInfo const* reqinfo );

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
     * @param sysCtlr A system controller object (used for rebooting)
     * @param daemonCtlr A daemon controller object (used for stopping/starting mu2eerd)
     * @param mqName Name of the mu2eerd control queue
     * @param shmName Name of the mu2eerd shared memory region
     */
    Mu2eerdDevice( ISystemController& sysCtlr,
                   DaemonController daemonCtlr, 
                   string mqName, 
                   string shmName );

    /**
     * Startup Device / Setting Property Read Method
     *
     * Returns the setting for the Startup device.
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void startupSetRead( Array<startup_set_t>& dest, ReqInfo const* reqinfo ); 

    /**
     * Startup Device / Setting Property Write Method
     *
     * Provides a new setting to the Startup device.
     *
     * @param src ACNET setting buffer
     * @param reqinfo ACNET request object
     */
    void startupSetWrite( Array<const startup_set_t>& src, ReqInfo const* reqinfo );

  private:
    /**
     * Controlls the daemon process
     */
    DaemonController _daemonCtlr;

    /**
     * mu2eerd Control message queue name
     */
    string _mqName;

    /**
     * mu2eerd Shared memory name
     */
    string _shmName;

    /**
     * Setting value for the Startup device
     */
    startup_set_t _startupVal;

    /**
     * System control
     */
    ISystemController& _sysCtlr;

    /**
     * On (Operation)
     *
     * Start mu2eerd w/ operational configuration.  If mu2eerd is already running, nothing is done.
     */
    void _onOper();

    /**
     * On (Test)
     *
     * Start mu2eerd w/ test configuration.  If mu2eerd is already running, nothing is done.
     */
    void _onTest();

    /**
     * Soft reboot
     */
    void _reboot();

    /**
     * Start Daemon
     */
    void _start();

    /**
     * Stop Daemon
     */
    void _stop();
  };
};

#endif
