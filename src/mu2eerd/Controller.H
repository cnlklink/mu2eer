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

#include "ConfigurationManager.H"
#include "SharedMemoryManager.H"
#include "SpillStateMachine.H"

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
     * Create Message Queue
     *
     * Sets up the control message queue
     */
    void _createMQ();

    /**
     * Handle Shutdown
     *
     * Invoked by _processMessages().  Handles a shutdown request.
     */
    void _handleShutdown();

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
