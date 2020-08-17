/**
 * StartCommand.H
 *
 * This file contains the definition of the StartCommand class.
 *
 * @author jdiamond
 */

#ifndef STARTCOMMAND_H
#define STARTCOMMAND_H

#include "Command.H"

namespace Mu2eER
{
  /**
   * StartCommand
   *
   * Implements a command for starting the spill state machine.
   */
  class StartCommand : public Command
  {
  public:
    /**
     * Constructor
     *
     * @param mqc Control message queue client reference
     * @param shmc Shared memory client reference
     */
    StartCommand( ControlMQClient& mqc, SharedMemoryClient& shmc );

    // Override
    virtual void run( unsigned int argc, const char* argv[] );
  };
};

#endif
