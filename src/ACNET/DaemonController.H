/**
 * DaemonController.H
 *
 * This file contains the definition of the DaemonController class.
 *
 * @author jdiamond
 */

#ifndef DAEMONCONTROLLER_H
#define DAEMONCONTROLLER_H

#include <string>

using namespace std;

namespace Mu2eER
{
  /**
   * DaemonController
   *
   * Implements an API for starting & stopping the mu2eerd daemon process.
   */
  class DaemonController
  {
  public:
    /**
     * Constructor
     *
     * @param name Process name
     * @param startCmd Command to start the daemon
     * @param stopCmd Command to stop the daemon
     */
    DaemonController( string name, string startCmd, string stopCmd );
    
    /**
     * Is Running
     *
     * @return True if the daemon process is running
     */
    bool isRunning() const;

    /**
     * Start Daemon
     *
     * Starts the daemon process.
     */
    void start() const;

    /**
     * Stop Daemon
     *
     * Stops the daemon process.
     */
    void stop() const;
    
  private:
    /**
     * The name of the daemon process
     */
    string _processName;
    
    /**
     * Command used to start the daemon process
     */
    string _startCmd;
    
    /**
     * Command used to stop the daemon process
     */
    string _stopCmd;

    /**
     * Exec
     *
     * Executes the given command, buffering the output so that it doesn't go to stdout.  Throws a 
     * runtime_error if the return code is not EXIT_SUCCESS.
     *
     * Note: If no output is desired, the used should redirect stderr to stdout via the command string, e.g.:
     *   command 2>&1
     *
     * @param cmd Command to execute
     */
    void _exec( string cmd ) const;
    
    /**
     * Get Process ID By Name
     *
     * Returns the process ID for the given process name (or -1 if not found).
     *
     * @param procName Process name
     * @return Process ID or -1 if not found
     */
    int _getProcIdByName( string procName ) const;
  };
};

#endif
