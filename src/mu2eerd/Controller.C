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

Controller::Controller( ConfigurationManager& cm )
  : _cm( cm ),
    _shmm( "mu2eer" ),
    _ssm( _cm, _shmm.ssmBlockGet() )
{
  _shmm.currentStateSet( MU2EERD_INITIALIZING );
}

void Controller::shutdown()
{
  _shmm.currentStateSet( MU2EERD_SHUTDOWN );
}

void Controller::start()
{
  _shmm.currentStateSet( MU2EERD_STARTUP );

  if( _cm.ssmAutoInitGet() )
    {
      // Initialize the spill state machine
      _ssm.initialize();
    }

  _shmm.currentStateSet( MU2EERD_RUNNING );
}
