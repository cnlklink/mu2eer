/**
 * ISSMDeviceDriver.C
 *
 * This file contains the implementation of the ISSMDeviceDriver class.
 *
 * @author jdiamond
 */

#include "ISSMDeviceDriver.H"
#include "SSMDeviceDriverMock.H"

using namespace Mu2eER;
using namespace std;

ISSMDeviceDriver::ISSMDeviceDriver()
{
}

ISSMDeviceDriver::~ISSMDeviceDriver()
{
}

unique_ptr<ISSMDeviceDriver> ISSMDeviceDriver::factory( string driverId )
{
  if( 0 == driverId.compare( "mock" ) )
    {
      return unique_ptr<ISSMDeviceDriver>( new SSMDeviceDriverMock( {
          SSM_BETWEEN_CYCLES, 
          SSM_START_CYCLE, 
          SSM_BETWEEN_SPILLS, 
          SSM_RAMP, 
          SSM_FAULT
      } ) );
    }

  return unique_ptr<ISSMDeviceDriver>( new SSMDeviceDriverMock( { SSM_FAULT } ) );
}

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
