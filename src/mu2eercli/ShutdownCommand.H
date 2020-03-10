/**
 * ShutdownCommand.H
 *
 * This file contains the definition of the ShutdownCommand class.
 *
 * @author jdiamond
 */

#ifndef SHUTDOWNCOMMAND_H
#define SHUTDOWNCOMMAND_H

#include "Command.H"

namespace Mu2eER
{
  /**
   * ShutdownCommand
   *
   * Implements a command for shutting down mu2eerd properly.
   */
  class ShutdownCommand : public Command
  {
  public:
    /**
     * Constructor
     *
     * @param mqc Control message queue client reference
     * @param shmc Shared memory client reference
     */
    ShutdownCommand( ControlMQClient& mqc, SharedMemoryClient& shmc );

    // Override
    virtual void run( unsigned int argc, const char* argv[] );
  };
};

#endif
