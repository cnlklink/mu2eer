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
  //_registerCDevMethods();
}

void TCLKDevice::eventCounterRead( Array<TCLKDevice::event_counter_read_t>& dest,
                                   ReqInfo const* reqinfo )
{
  dest[0] = 0;
  /*
  if( dest.offset.getValue() > static_cast<int>( SPILL_COUNTER_READING_MAX ) )
    {
      throw Ex_BADOFF;
    }

  if( (dest.offset.getValue() + dest.total.getValue()) >
      static_cast<int>( SPILL_COUNTER_READING_MAX ) )
    {
      throw Ex_BADOFLEN;
    }

  try
    {
      SharedMemoryClient shmc( _shmName );
      dest[0] = shmc.ssmBlockGet().spillCounterGet();
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::spillCounterRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
  */
}
