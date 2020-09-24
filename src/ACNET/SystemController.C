/**
 * SystemController.C
 *
 * This file contains the implementation of the SystemController class.
 *
 * @author jdiamond
 */

#include <chrono>
#include <fstream>
#include <sys/reboot.h>
#include <thread>
#include <unistd.h>

#include "SystemController.H"

using namespace std;
using namespace Mu2eER;

bool SystemController::isRebooting() const
{
  return _rebooting;
}

unsigned int SystemController::jenkinsNumberGet() const
{
  fstream jnfile( "/etc/jenkins_build_number", ios_base::in );
  unsigned int num;

  jnfile >> num;

  return num;
}

void SystemController::doReboot()
{
  _rebooting = true;
  
  // Start a thread that executes the reboot after waiting a half-second to give us just a little
  // time to acknowledge success before the system reboots
  thread rebootT( []() {
      this_thread::sleep_for( chrono::milliseconds( 500 ) );
      sync();
      reboot( RB_AUTOBOOT );
    } );
}

SystemController::SystemController()
  : _rebooting( false )
{
}
