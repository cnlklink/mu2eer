/**
 * Controller.H
 *
 * This file contains the definition of the Controller class.
 *
 * @author jdiamond
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ConfigurationManager.H"
#include "SharedMemoryManager.H"
#include "SpillStateMachine.H"

namespace Mu2eER
{
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
     */
    Controller();

    /**
     * Get Shared Memory Manager
     *
     * @return Read-only shared memory manager
     */
    const SharedMemoryManager& shmmGet() const;

    /**
     * Shutdown
     *
     * Shutdown mu2eerd.
     */
    void shutdown();

    /**
     * Start
     *
     * Start-up mu2eerd controller.
     */
    void start();

  private:
    /**
     * Manages the mu2eerd configuration
     */
    ConfigurationManager _cm;

    /**
     * Manages the shared memory API
     */
    SharedMemoryManager _shmm;

    /**
     * Interface to the spill state machine
     */
    SpillStateMachine _ssm;
  };
};

#endif
