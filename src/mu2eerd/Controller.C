/**
 * Controller.C
 *
 * This file contains the implementation of the Controller class.
 *
 * @author jdiamond
 */

#include "Controller.H"

using namespace Mu2eER;
using namespace std;

Controller::Controller()
  : _shmm( "mu2eer" ),
    _ssm( _cm, _shmm.ssmBlockGet() )
{
}

const SharedMemoryManager& Controller::shmmGet() const
{
  return _shmm;
}

void Controller::shutdown()
{
}

void Controller::start()
{
  if( _cm.ssmAutoInitGet() )
    {
      // Initialize the spill state machine
      _ssm.initialize();
    }
}
