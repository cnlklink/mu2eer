/**
 * Controller.H
 *
 * This file contains the definition of the Controller class.
 *
 * @author jdiamond
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <mqueue.h>
#include <thread>

#include "ConfigurationManager.H"
#include "SharedMemoryManager.H"
#include "SpillStateMachine.H"
#include "tclk.H"

using namespace std;

namespace Mu2eER
{
  /**
   * Exceptions thrown by the controller
   */
  typedef runtime_error controller_error;

  /**
   * Controller
   *
   * Main control class for the mu2eerd daemon.
   */
  class Controller
  {
  public:
    /**
     * Test Daemon's Message Queue Name
     */
    static const string TEST_DAEMON_CMQ_NAME;

    /**
     * Test Daemon's Shared Memory Name
     */
    static const string TEST_DAEMON_SHM_NAME;

    /**
     * Constructor
     *
     * @param cm Reference to the ConfigurationManager
     * @param mqName Control message queue name (start with a /)
     * @param shmName Shared memory region name
     */
    Controller( ConfigurationManager& cm, string mqName, string shmName );

    /**
     * Destructor
     */
    ~Controller();

    /**
     * Start
     *
     * Start-up mu2eerd controller.
     */
    void start();

    /**
     * Start Test Daemon
     *
     * Instantiates a test version of mu2eerd.
     *
     * @param cfile Configuration file name (default: none)
     */
    static void testDaemonStart( string cfile = "" );

    /**
     * Test Daemon Cleanup
     *
     * Waits for the test daemon to finish and then releases it's resources.
     */
    static void testDaemonCleanup();

  private:
    /**
     * Depth of the control message queue
     */
    static const ssize_t MQ_MAX_MESSAGES = 3;

    /**
     * Manages the mu2eerd configuration
     */
    ConfigurationManager& _cm;

    /**
     * Control message queue ID
     */
    mqd_t _mqId;

    /**
     * Control message queue name
     */
    string _mqName;

    /**
     * Manages the shared memory API
     */
    SharedMemoryManager _shmm;

    /**
     * Interface to the spill state machine
     */
    SpillStateMachine _ssm;

    /**
     * TCLK sub-system
     */
    TCLKSubsystem _tclk;

    /**
     * Test Daemon Configuration
     */
    static ConfigurationManager* _testdCM;

    /**
     * Test Daemon Controller
     */
    static Controller* _testdCtlr;

    /**
     * Test Daemon Thread
     */
    static thread* _testdThread;

    /**
     * Create Message Queue
     *
     * Sets up the control message queue
     */
    void _createMQ();

    /**
     * Handle Fault
     *
     * Invoked by _processMessages().  Handles a fault request.
     */
    void _handleFault();

    /**
     * Handle Reset
     *
     * Invoked by _processMessages().  Handles a reset request.
     */
    void _handleReset();

    /**
     * Handle Shutdown
     *
     * Invoked by _processMessages().  Handles a shutdown request.
     */
    void _handleShutdown();

    /**
     * Handle SSM Init
     *
     * Invoked by _processMessages().  Handles a SSM initialization request.
     */
    void _handleSSMInit();

    /**
     * Handle Start
     *
     * Invoked by _processMessages().  Handles a start request.
     */
    void _handleStart();

    /**
     * Process Control Messages
     *
     * Processes messages as they arrive on the control message queue.
     */
    void _processMessages();

    /**
     * Shutdown
     *
     * Shutdown mu2eerd.
     */
    void _shutdown();
  };
};

#endif
