/**
 * FaultCommand.C
 *
 * This file contains the implementation of the FaultCommand class.
 *
 * @author jdiamond
 */

#include <iostream>

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
  cout << "Request spill state machine go to the fault state...";

  _mqc.fault();

  _waitForSSMState( SSM_FAULT );

  cout << "Spill state machine is in the fault state." << endl;
}
