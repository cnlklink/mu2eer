/**
 * ResetCommand.C
 *
 * This file contains the implementation of the ResetCommand class.
 *
 * @author jdiamond
 */

#include <iostream>
#include <signal.h>
#include <thread>

#include "errors.H"
#include "ResetCommand.H"

using namespace Mu2eER;
using namespace std;

ResetCommand::ResetCommand( ControlMQClient& mqc, SharedMemoryClient& shmc )
  : Command( mqc, shmc )
{
}

void ResetCommand::run( unsigned int argc, const char* argv[] )
{
  cout << "Requesting spill state machin reset...";

  try
    {
      _mqc.reset();

      _waitForSSMState( SSM_IDLE );

      cout << "Spill state machine has been reset." << endl;
    }
  catch( Error e )
    {
      cout << " FAILED!" << endl;
    }
}
