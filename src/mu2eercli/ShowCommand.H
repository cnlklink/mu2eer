/**
 * ShowCommand.H
 *
 * This file contains the definition of the ShowCommand class.
 *
 * @author jdiamond
 */

#ifndef SHOWCOMMAND_H
#define SHOWCOMMAND_H

#include "Command.H"

namespace Mu2eER
{
  /**
   * ShowCommand
   *
   * Implements a command for displaying system status and diagnostic information from the CLI.
   */
  class ShowCommand : public Command
  {
  public:
    /**
     * Constructor
     *
     * @param mqc Control message queue client reference
     * @param shmc Shared memory client reference
     */
    ShowCommand( ControlMQClient& mqc, SharedMemoryClient& shmc );

    // Override
    virtual void run( unsigned int argc, const char* argv[] );
  };
};

#endif
