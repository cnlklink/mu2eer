/**
 * DumpCommand.C
 *
 * This file contains the implementation of the DumpCommand class.
 *
 * @author rtadkins
 */

 #include <iostream>

 #include "DumpCommand.H"

 using namespace Mu2eER;
 using namespace std;

 DumpCommand::DumpCommand( ControlMQClient& mqc, SharedMemoryClient& shmc )
   : Command( mqc, shmc )
 {
 }

 void DumpCommand::run( unsigned int argc, const char* argv[] )
 {
   cout << _shmc.pidGet() << endl;
 }
