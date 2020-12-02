/**
 * SpillStateMachine.C
 *
 * This file contains the implementation of the SpillStateMachine class.
 *
 * @author jdiamond and rtadkins
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

  _smbUpdate();
}

SpillStateMachine::~SpillStateMachine()
{
  // Make sure the thread exits
  stop();
}

void SpillStateMachine::fault()
{
  _ssmDev->fault();
  // Turn LED off
  _ssmDev->ledOff();
  _smbUpdate();
}

void SpillStateMachine::initialize()
{
  // Initialize firmware
  _ssmDev->initialize();
  // Turn LED on
  _ssmDev->ledOn();
  _smbUpdate();
}

void SpillStateMachine::reset()
{
  // Stop the thread
  stop();

  // Reset the SSM
  _ssmDev->reset();

  _smbUpdate();
}

void SpillStateMachine::run()
{
  // Start thread
  _thread = unique_ptr<thread>( new thread( [this]() {
        while( SSM_FAULT != _ssmDev->stateGet() )
          {
            _waitForStateChange();

            _smbUpdate();
          }
      } ) );

  // Update the SMB to show that the thread is running
  _smbUpdate();
}

void SpillStateMachine::stop()
{
  if( _thread.get() == nullptr )
    {
      // Thread is not running
      return;
    }

  // Wait for thread to stop
  _thread->join();

  // Update the SMB to show that the thread is not running
  _thread.reset( nullptr );
  // Turn LED off
  _ssmDev->ledOff();
  _smbUpdate();
}

void SpillStateMachine::_smbUpdate()
{
  _smb.currentStateSet( _ssmDev->stateGet() );
  _smb.spillCounterSet( _ssmDev->spillCounterGet() );
  _smb.timeInSpillSet( _ssmDev->timeInSpillGet() );
  _smb.threadRunningSet( _thread.get() != nullptr );
  _smb.ledStateSet( _ssmDev->ledStateGet() );
  _smb.initialize();
}

ssm_state_t SpillStateMachine::_waitForStateChange() const
{
  return _ssmDev->waitForStateChange();
}
