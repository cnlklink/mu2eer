/**
 * FaultCommand.C
 *
 * This file contains the implementation of the FaultCommand class.
 *
 * @author jdiamond
 */

#include <iostream>
#include <signal.h>
#include <thread>

#include "errors.H"
#include "FaultCommand.H"

using namespace Mu2eER;
using namespace std;

FaultCommand::FaultCommand( ControlMQClient& mqc, SharedMemoryClient& shmc )
  : Command( mqc, shmc )
{
}

void FaultCommand::run( unsigned int argc, const char* argv[] )
{
  auto& smb = _shmc.ssmBlockGet();

  cout << "Request spill state machine go to the fault state... ";

  _mqc.fault();

  // Wait for SSM to enter the fault state
  unsigned int count = 0;
  while( SSM_FAULT != smb.currentStateGet() )
    {
      cout << ".";
      cout.flush();
      this_thread::sleep_for( chrono::seconds( 1 ) );

      if( ++count > 3 )
        {
          cout << "FAILED!" << endl;
          throw MU2EERCLI_FAULT_ABORTED;
        }
    }

  cout << "Spill state machine is in the fault state." << endl;
}
