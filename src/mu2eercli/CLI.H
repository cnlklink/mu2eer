/**
 * CLI.H
 *
 * This file contains the definition of the CLI class.
 *
 * @author jdiamond
 */

#ifndef CLI_H
#define CLI_H

#include <map>
#include <memory>
#include <stdexcept>

#include "Command.H"
#include "ControlMQClient.H"
#include "errors.H"
#include "SharedMemoryClient.H"

using namespace std;

namespace Mu2eER
{
  /**
   * CLI
   *
   * Implements the command line interface.
   */
  class CLI
  {
  public:
    /**
     * Constructor
     *
     * @param mqcName Name of the mu2eerd control message queue
     * @param shmcName Name of the mu2eerd shared memory region
     */
    CLI( const string mqcName, const string shmcName );

    /**
     * Run
     *
     * @param argc Argument count from main()
     * @param argv Argument list from main()
     */
    void run( unsigned int argc, const char* argv[] );

  private:
    /**
     * Maps command strings to Command objects
     */
    map<string,unique_ptr<Command>> _commands;

    /**
     * Control message queue interface for mu2eerd
     */
    ControlMQClient _mqc;

    /**
     * Shared memory interface for mu2eerd
     */
    SharedMemoryClient _shmc;
  };
};

#endif
