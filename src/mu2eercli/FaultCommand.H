/**
 * FaultCommand.H
 *
 * This file contains the definition of the FaultCommand class.
 *
 * @author jdiamond
 */

#ifndef FAULTCOMMAND_H
#define FAULTCOMMAND_H

#include "Command.H"

namespace Mu2eER
{
  /**
   * FaultCommand
   *
   * Implements a command for putting the spill state machine into the "fault" state.  This should
   * only be used for testing an debugging.
   */
  class FaultCommand : public Command
  {
  public:
    /**
     * Constructor
     *
     * @param mqc Control message queue client reference
     * @param shmc Shared memory client reference
     */
    FaultCommand( ControlMQClient& mqc, SharedMemoryClient& shmc );

    // Override
    virtual void run( unsigned int argc, const char* argv[] );
  };
};

#endif
