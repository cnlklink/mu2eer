/**
 * Command.C
 *
 * This file contains the implementation of the Command class.
 *
 * @author jdiamond
 */

#include <iostream>
#include <signal.h>
#include <thread>

#include "Command.H"
#include "errors.H"

using namespace Mu2eER;
using namespace std;

Command::Command( ControlMQClient& mqc, SharedMemoryClient& shmc )
  : _mqc( mqc ),
    _shmc( shmc )
{
}

void Command::_waitForSSMState( ssm_state_t state, unsigned int delay, unsigned int max )
{
  auto& smb = _shmc.ssmBlockGet();

  // Wait for SSM to enter the fault state
  unsigned int count = 0;
  while( state != smb.currentStateGet() )
    {
      cout << ".";
      cout.flush();
      this_thread::sleep_for( chrono::milliseconds( delay ) );

      if( ++count > max )
        {
          cout << " FAILED!" << endl;
          throw MU2EERCLI_TIMEOUT;
        }
    }  
}
