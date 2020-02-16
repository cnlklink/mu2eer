/**
 * main.C
 *
 * This file contains the entry point for the mu2eerd daemon.
 *
 * @author jdiamond
 */

#include <fstream>
#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Controller.H"
#include "ControlMQClient.H"

using namespace Mu2eER;
using namespace std;

/**
 * SIGTERM Handler
 *
 * Responds to SIGTERM by shuting down the Controller.
 */
static void _handle_sigterm( int signal )
{
  ControlMQClient mqc( "/mu2eer" );
  mqc.shutdown();
}

int main( int argc, char* argv[] )
{
  // Become a daemon
  pid_t pid;

  // Fork off the parent process and exit
  if( 0 > (pid = fork()) )
    return EXIT_FAILURE;
  else if( pid > 0 )
    return EXIT_SUCCESS;

  // Set umask
  umask( 0 );

  // Close the standard file descriptors
  close( STDIN_FILENO );
  close( STDOUT_FILENO );
  close( STDERR_FILENO );

  // Start logging
  openlog( "mu2eerd", 0, LOG_USER );

  try
    {
      // Install handler for SIGINT
      struct sigaction sa;
      memset( &sa, 0, sizeof( sa ) );
      sa.sa_handler = _handle_sigterm;
      sigfillset( &sa.sa_mask );
      sigaction( SIGTERM, &sa, NULL );

      // Instantiate a Controller object
      ConfigurationManager cm;
      Controller ctlr( cm, "/mu2eer", "mu2eer" );

      // Enter command processing loop
      ctlr.start();
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught by main: %s", e.what() );
    }
  catch( exception e )
    {
      syslog( LOG_ERR, "generic exception caught by main: %s", e.what() );
    }

  syslog( LOG_INFO, "mu2eerd shut down." );

  return 0;
}
