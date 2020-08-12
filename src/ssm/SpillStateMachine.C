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

ostream& Mu2eER::operator<<( ostream& os, const ssm_state_t state )
{
  switch( state )
    {
    case SSM_IDLE: os << "idle"; break;
    case SSM_INIT:  os << "init"; break;
    case SSM_BETWEEN_CYCLES: os << "between_cycles"; break;
    case SSM_START_CYCLE: os << "start_cycle"; break;
    case SSM_BETWEEN_SPILLS: os << "between_spills"; break;
    case SSM_READ_IBEAM: os << "read_ibeam"; break;
    case SSM_RAMP: os << "ramp"; break;
    case SSM_SPILL: os << "spill"; break;
    case SSM_START_EOS_TIMER: os << "start_eos_timer"; break;
    case SSM_SPILL_PARABOLA: os << "spill_parabola"; break;
    case SSM_START_CLEANUP_TIMER: os << "start_cleanup_timer"; break;
    case SSM_AFTER_SPILL: os << "after_spill"; break;
    case SSM_END_CYCLE: os << "end_cycle"; break;
    case SSM_LEARNING: os << "learning"; break;
    case SSM_ABORT: os << "abort"; break;
    case SSM_FAULT: os << "fault"; break;
    case SSM_UNKNOWN: os << "UNKNOWN"; break;

    default: os << "UNDEFINED"; break;
    }

  return os;
}

SpillStateMachine::SpillStateMachine( const ConfigurationManager& cm, SpillStateMachineSMB& smb )
  : _cm( cm ),
    _smb( smb ),
    _ssmDev( ISSMDeviceDriver::factory( _cm.ssmGet().driverGet() ) )
{
  // Configure SSM Device
  _ssmDev->configure( _cm.ssmGet() );

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

unsigned int SpillStateMachine::spillCounterGet() const
{
  return _ssmDev->spillCounterGet();
}

ssm_state_t SpillStateMachine::stateGet() const
{
  return _ssmDev->stateGet();
}

SpillStateMachineSMB& sharedMemoryGet() const
{
  return _ssmDev->sharedMemoryGet();
}

ssm_state_t SpillStateMachine::waitForStateChange() const
{
  return _ssmDev->waitForStateChange();
}
