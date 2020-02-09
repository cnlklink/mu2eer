/**
 * SpillStateMachine.C
 *
 * This file contains the implementation of the SpillStateMachine class.
 *
 * @author jdiamond
 */

#include <iostream>

#include "SpillStateMachine.H"
#include "SpillStateMachineSMB.H"
#include "SSMDeviceDriverMock.H"

using namespace Mu2eER;
using namespace std;

/**
 * The SSM module was not initialized
 */
ssm_error SSM_NOT_READY( "SSM Not Ready" );

SpillStateMachine::SpillStateMachine( const ConfigurationManager& cm, SpillStateMachineSMB& smb )
  : _cm( cm ),
    _smb( smb ),
    _ssmDev( ISSMDeviceDriver::factory( _cm.ssmDriverGet() ) )
{
  _smb.currentStateSet( _ssmDev->stateGet() );
}

void SpillStateMachine::initialize()
{
  cout << "Initializing Spill Statate Machine...";

  // Initialize firmware
  _ssmDev->initialize();
  _smb.currentStateSet( _ssmDev->stateGet() );

  cout << " done." << endl;
}

ssm_state_t SpillStateMachine::stateGet() const
{
  return _ssmDev->stateGet();
}
