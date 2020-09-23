/**
 * SystemController.C
 *
 * This file contains the implementation of the SystemController class.
 *
 * @author jdiamond
 */

#include <fstream>
#include <sys/reboot.h>
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
  
  sync();
  reboot( RB_AUTOBOOT );
}

SystemController::SystemController()
  : _rebooting( false )
{
}
