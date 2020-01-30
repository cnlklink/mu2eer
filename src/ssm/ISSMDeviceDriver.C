/**
 * ISSMDeviceDriver.C
 *
 * This file contains the implementation of the ISSMDeviceDriver class.
 *
 * @author jdiamond
 */

#include "ISSMDeviceDriver.H"

using namespace Mu2eER;
using namespace std;

ISSMDeviceDriver::ISSMDeviceDriver()
{
}

ISSMDeviceDriver::~ISSMDeviceDriver()
{
}

ostream& Mu2eER::operator<<( ostream& os, const ISSMDeviceDriver::State state )
{
  switch( state )
    {
    case ISSMDeviceDriver::STATE_IDLE: os << "idle"; break;
    case ISSMDeviceDriver::STATE_INIT:  os << "init"; break;
    case ISSMDeviceDriver::STATE_BETWEEN_CYCLES: os << "between_cycles"; break;
    case ISSMDeviceDriver::STATE_START_CYCLE: os << "start_cycle"; break;
    case ISSMDeviceDriver::STATE_BETWEEN_SPILLS: os << "between_spills"; break;
    case ISSMDeviceDriver::STATE_READ_IBEAM: os << "read_ibeam"; break;
    case ISSMDeviceDriver::STATE_RAMP: os << "ramp"; break;
    case ISSMDeviceDriver::STATE_SPILL: os << "spill"; break;
    case ISSMDeviceDriver::STATE_START_EOS_TIMER: os << "start_eos_timer"; break;
    case ISSMDeviceDriver::STATE_SPILL_PARABOLA: os << "spill_parabola"; break;
    case ISSMDeviceDriver::STATE_START_CLEANUP_TIMER: os << "start_cleanup_timer"; break;
    case ISSMDeviceDriver::STATE_AFTER_SPILL: os << "after_spill"; break;
    case ISSMDeviceDriver::STATE_END_CYCLE: os << "end_cycle"; break;
    case ISSMDeviceDriver::STATE_LEARNING: os << "learning"; break;
    case ISSMDeviceDriver::STATE_ABORT: os << "abort"; break;

    default: os << "UNDEFINED"; break;
    }

  return os;
}
