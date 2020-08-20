/**
 * main.C
 *
 * This file contains the entry point for the mu2eerd daemon.
 *
 * @author jdiamond
 */

#include <fstream>
#include <iostream>
#include <limits.h>
#include <signal.h>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.H"
#include "Controller.H"
#include "ControlMQClient.H"
#include "errors.H"

using namespace Mu2eER;
using namespace std;

/**
 * SIGTERM Handler
 *
 * Responds to SIGTERM by shuting down the Controller.
 */
static void _handle_sigterm( int signal )
{
  ControlMQClient mqc( MU2EERD_CMQ_NAME );
  mqc.shutdown();
}

int main( int argc, char* argv[] )
{
  bool loadConfigFlag = false;
  ConfigurationManager cm;

  // The rules for configuration are -
  //   -c flag takes precedent, but the file must exist or mu2eerd aborts
  //   If there's no -c flag then /etc/mu2eer.d/<hostname>-mu2eerd.conf is loaded by default
  //   And if that file does not exist then the default configuration is used
  // tldr; Only -c will cause mu2eerd to fail
  string cfgfile = ConfigurationManager::hostConfigFileGet();
  fstream fs;
  fs.open( cfgfile );
  if( !fs.fail() )
    {
      loadConfigFlag = true;
    }

  try
    {
      // Parse command-line options
      int c;
      while( (c = getopt( argc, argv, "c:" )) != -1 )
        {
          switch( c )
            {
            case 'c':
              cfgfile = optarg;
              loadConfigFlag = true;
              break;
              
            case '?':
              cerr << "invalid argument!" << endl;
              return 1;
              break;
              
            default:
              cerr << "unknown argument!" << endl;
              return 1;
            }
        }
      
      // Load configuration file
      if( loadConfigFlag )
        {
          cm.load( cfgfile );
        }
    }
  catch( Error e )
    {
      cerr << e.what() << endl;
      return 2;
    }
      
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
  
  // Install handler for SIGINT
  struct sigaction sa;
  memset( &sa, 0, sizeof( sa ) );
  sa.sa_handler = _handle_sigterm;
  sigfillset( &sa.sa_mask );
  sigaction( SIGTERM, &sa, NULL );
  
  try
    {
      // Instantiate a Controller object
      Controller ctlr( cm, MU2EERD_CMQ_NAME, MU2EERD_SHM_NAME );
      
      // Enter command processing loop
      ctlr.start();
    }
  catch( Error e )
    {
      syslog( LOG_ERR, "Error caught by main: %s", e.what() );
      return 3;
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught by main: %s", e.what() );
      return 3;
    }
  catch( exception e )
    {
      syslog( LOG_ERR, "generic exception caught by main: %s", e.what() );
      return 3;
    }

  syslog( LOG_INFO, "mu2eerd shut down." );

  return 0;
}
