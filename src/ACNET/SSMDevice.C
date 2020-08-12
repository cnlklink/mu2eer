/**
 * SSMDevice.C
 *
 * This file contains the implementation of the SSMDevice class.
 *
 * @author jdiamond
 */

#include "SSMDevice.H"

using namespace Mu2eER;

SSMDevice::SSMDevice( string mqName, string shmName )
  : Device<32>( "SSMDevice", "Spill State Machine Device" ),
    _mqName( mqName ),
    _shmName( shmName )
{
  registerMethod( ATTR_STATE_READING, *this, &SSMDevice::stateRead, STATE_READING_MAX );
}

void SSMDevice::stateRead( Array<SSMDevice::state_read_t>& dest, ReqInfo const* reqinfo )
{
  if( dest.offset.getValue() > static_cast<int>( STATE_READING_MAX ) )
    {
      throw Ex_BADOFF;
    }

  if( (dest.offset.getValue() + dest.total.getValue()) > static_cast<int>( STATE_READING_MAX ) )
    {
      throw Ex_BADOFLEN;
    }

  try
    {
      SharedMemoryClient shmc( _shmName );
      dest[0] = shmc.ssmBlockGet().currentStateGet();
    }
  catch( runtime_error e )
    {
      throw Ex_DEVFAILED;
    }
}