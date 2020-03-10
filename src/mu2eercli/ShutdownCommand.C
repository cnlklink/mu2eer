/**
 * PIDCommand.C
 *
 * This file contains the implementation of the PIDCommand class.
 *
 * @author jdiamond
 */

#include <iostream>
#include <signal.h>
#include <thread>

#include "errors.H"
#include "ShutdownCommand.H"

using namespace Mu2eER;
using namespace std;

ShutdownCommand::ShutdownCommand( ControlMQClient& mqc, SharedMemoryClient& shmc )
  : Command( mqc, shmc )
{
}

void ShutdownCommand::run( unsigned int argc, const char* argv[] )
{
  pid_t pid = _shmc.pidGet();

  cout << "Requesting mu2eerd (pid: " << pid << ") shutdown." << endl;
  _mqc.shutdown();

  cout << "Waiting for mu2eerd to shutdown";
  unsigned int count = 0;
  while( 0 == kill( pid, 0 ) )
    {
      cout << ".";
      cout.flush();
      this_thread::sleep_for( chrono::seconds( 1 ) );

      if( ++count > 2 )
        {
          cout << "ABORTED!" << endl;
          throw MU2EERCLI_SHUTDOWN_ABORTED;
        }
    }

  cout << "Shutdown OK." << endl;
}
