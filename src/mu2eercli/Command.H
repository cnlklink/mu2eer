/**
 * Command.H
 *
 * This file contains the definition of the Command class.
 *
 * @author jdiamond
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "ControlMQClient.H"
#include "SharedMemoryClient.H"

using namespace Mu2eER;

namespace Mu2eER
{
  /**
   * Command
   *
   * Abstract Command class.  Defines an interface for the Commands provided by mu2eercli.
   */
  class Command
  {
  public:
    /**
     * Command
     *
     * @param mqc Control message queue client reference
     * @param shmc Shared memory client reference
     */
    Command( ControlMQClient& mqc, SharedMemoryClient& shmc );

    /**
     * Run Command
     *
     * @param argc Argument count from the command line
     * @param argv Argument list from the command line
     */
    virtual void run( unsigned int argc, const char* argv[] ) = 0;

  protected:
    /**
     * Reference to the mu2eerd message queue client
     */
    ControlMQClient& _mqc;

    /**
     * Reference to the mu2eerd shared memory interface
     */
    SharedMemoryClient& _shmc;

    /**
     * Wait for SSM State
     *
     * Waits for the SSM to transition to the given state.  Prints a "." every delay until the
     * max is reached.  Throws MU2EERCLI_TIMEOUT if max is reached.
     *
     * @param state State to wait for
     * @param delay Delay between state checks
     * @param max Maximum number of state checks 
     */
    void _waitForSSMState( ssm_state_t state, unsigned int delay = 1000, unsigned int max = 3 );
  };
};

#endif
