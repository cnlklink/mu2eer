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
     * Configuration option
     */
    enum config_t
      {
        CONFIG_TEST,
        CONFIG_PRODUCTION
      };

    /**
     * Constructor
     *
     * Construct a daemon controller where the test and production start commands are the same.
     *
     * @param name Process name
     * @param startCmd Command to start the daemon
     * @param stopCmd Command to stop the daemon
     */
    DaemonController( string name, string startCmd, string stopCmd );

    /**
     * Constructor (w/ Test Configuration)
     *
     * Construct a daemon controller with separate start commands for the test configuration and the
     * production configuration.
     *
     * @param name Process name
     * @param prodStartCmd Command to start the daemon with a production configuration
     * @param testStartCmd Command to start the daemon with a test conifguration
     * @param stopCmd Command to stop the daemon
     */
    DaemonController( string name, string prodStartCmd, string testStartCmd, string stopCmd );
    
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
     *
     * @param config Test/production configuration (default: CONFIG_PRODUCTION)
     */
    void start( config_t config = CONFIG_PRODUCTION ) const;

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
     * Command used to start the daemon process w/ production configuration
     */
    string _startCmdProd;
    
    /**
     * Command used to start the daemon process w/ test configuration
     */
    string _startCmdTest;
    
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
