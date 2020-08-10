/**
 * ShowCommand.C
 *
 * This file contains the implementation of the ShowCommand class.
 *
 * @author jdiamond
 */

#include <iomanip>
#include <iostream>

#include "ShowCommand.H"

using namespace Mu2eER;
using namespace std;

ShowCommand::ShowCommand( ControlMQClient& mqc, SharedMemoryClient& shmc )
  : Command( mqc, shmc )
{
}

void ShowCommand::run( unsigned int argc, const char* argv[] )
{
  // Get the current time
  time_t now;
  time( &now );

  // Get the start time
  time_t startTime = _shmc.startTimeGet();
  tm* localST = localtime( &startTime );
  char startTimeStr[40];
  strftime( startTimeStr, sizeof( startTimeStr ), "%Y-%m-%d %H:%M:%S", localST );

  // Calculate uptime
  time_t uptime = (now - startTime);

  // Output uptime and start time
  cout << "mu2eerd running since " << startTimeStr
       << ", uptime: " << uptime << " seconds" 
       << endl;
}
