/**
 * ResetCommand.H
 *
 * This file contains the definition of the ResetCommand class.
 *
 * @author jdiamond
 */

#ifndef RESETCOMMAND_H
#define RESETCOMMAND_H

#include "Command.H"

namespace Mu2eER
{
  /**
   * ResetCommand
   *
   * Implements a command for resetting the spill state machine.
   */
  class ResetCommand : public Command
  {
  public:
    /**
     * Constructor
     *
     * @param mqc Control message queue client reference
     * @param shmc Shared memory client reference
     */
    ResetCommand( ControlMQClient& mqc, SharedMemoryClient& shmc );

    // Override
    virtual void run( unsigned int argc, const char* argv[] );
  };
};

#endif
