/**
 * Mu2eerdDevice.C
 *
 * This file contains the implementation of the Mu2eerdDevice class.
 *
 * @author jdiamond
 */

#include "Mu2eerdDevice.H"

using namespace Mu2eER;

Mu2eerdDevice::Mu2eerdDevice( string mqName, string shmName )
  : Device<32>( "Mu2eerdDevice", "mu2eerd Device" ),
    _mqc( mqName ),
    _shmc( shmName )
{
  registerMethod( ATTR_STATE_READ, *this, &Mu2eerdDevice::stateRead, STATE_READ_MAX );
}

void Mu2eerdDevice::stateRead( Array<state_read_t>& dest, ReqInfo const* reqinfo __attribute((unused)) )
{
  if( dest.offset.getValue() > static_cast<int>( STATE_READ_MAX ) )
    {
      throw Ex_BADOFF;
    }

  if( (dest.offset.getValue() + dest.total.getValue()) > static_cast<int>( STATE_READ_MAX ) )
    {
      throw Ex_BADOFLEN;
    }

  dest[0] = _shmc.ssmBlockGet().currentStateGet();
}
