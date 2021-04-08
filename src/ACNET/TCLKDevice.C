/**
 * TCLKDevice.C
 *
 * This file contains the implementation of the TCLKDevice class.
 *
 * @author jdiamond
 */

#include <syslog.h>
#include <iostream>

#include "errors.H"
#include "TCLKDevice.H"

using namespace Mu2eER;
using namespace std;

TCLKDevice::TCLKDevice( string mqName, string shmName )
  : Device<32>( "TCLKDevice", "TCLK Device" ),
    _mqName( mqName ),
    _shmName( shmName )
{
  registerMethod( ATTR_EVENT_COUNTER_READING,
                  *this,
                  &TCLKDevice::eventCounterRead,
                  EVENT_COUNTER_READING_MAX );
  
}

void TCLKDevice::eventCounterRead( Array<TCLKDevice::event_counter_read_t>& dest,
                                   ReqInfo const* reqinfo )
{
  if( dest.offset.getValue() > static_cast<int>( EVENT_COUNTER_READING_MAX ) )
    {
      throw Ex_BADOFF;
    }

  if( (dest.offset.getValue() + dest.total.getValue()) >
      static_cast<int>( EVENT_COUNTER_READING_MAX ) )
    {
      throw Ex_BADOFLEN;
    }

  try
    {
      SharedMemoryClient shmc( _shmName );
      dest[0] = shmc.tclkBlockGet().eventCounterGet();
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in TCLKDevice::evemtCounterRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}
