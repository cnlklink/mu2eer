/**
 * PIDCommand.C
 *
 * This file contains the implementation of the PIDCommand class.
 *
 * @author jdiamond
 */

#include <iostream>

#include "PIDCommand.H"

using namespace Mu2eER;
using namespace std;

PIDCommand::PIDCommand( ControlMQClient& mqc, SharedMemoryClient& shmc )
  : Command( mqc, shmc )
{
}

void PIDCommand::run( unsigned int argc, const char* argv[] )
{
  cout << _shmc.pidGet() << endl;
}
