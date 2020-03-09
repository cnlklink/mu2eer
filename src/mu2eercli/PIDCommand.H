/**
 * PIDCommand.H
 *
 * This file contains the definition of the PIDCommand class.
 *
 * @author jdiamond
 */

#ifndef PIDCOMMAND_H
#define PIDCOMMAND_H

#include "Command.H"

namespace Mu2eER
{
  /**
   * PIDCommand
   *
   * Implements a command for retrieving the process ID (pid) for mu2eerd from the shared memory
   * interface.
   */
  class PIDCommand : public Command
  {
  public:
    /**
     * Constructor
     *
     * @param mqc Control message queue client reference
     * @param shmc Shared memory client reference
     */
    PIDCommand( ControlMQClient& mqc, SharedMemoryClient& shmc );

    // Override
    virtual void run( unsigned int argc, const char* argv[] );
  };
};

#endif
