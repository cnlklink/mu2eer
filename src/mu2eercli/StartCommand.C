/**
 * StartCommand.C
 *
 * This file contains the implementation of the StartCommand class.
 *
 * @author jdiamond
 */

#include <iostream>
#include <signal.h>
#include <thread>

#include "errors.H"
#include "StartCommand.H"

using namespace Mu2eER;
using namespace std;

StartCommand::StartCommand( ControlMQClient& mqc, SharedMemoryClient& shmc )
  : Command( mqc, shmc )
{
}

void StartCommand::run( unsigned int argc, const char* argv[] )
{
  auto& smb = _shmc.ssmBlockGet();

  if( smb.threadRunningGet() )
    {
      cout << "The spill state machine is already running." << endl;
      throw MU2EERCLI_START_ABORTED;
    }

  cout << "Starting spill state machine." << endl;
  _mqc.start();

  // Wait up to 2 seconds for the SSM thread to start
  unsigned int count = 0;
  while( !smb.threadRunningGet() )
    {
      cout << ".";
      cout.flush();
      this_thread::sleep_for( chrono::seconds( 1 ) );

      if( ++count > 2 )
        {
          cout << "FAILED!" << endl;
          throw MU2EERCLI_START_ABORTED;
        }
    }

  cout << "Spill state machine successfully started." << endl;
}
