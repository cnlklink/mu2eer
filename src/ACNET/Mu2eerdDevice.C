/**
 * Mu2eerdDevice.C
 *
 * This file contains the implementation of the Mu2eerdDevice class.
 *
 * @author jdiamond
 */

#include <syslog.h>

#include "errors.H"
#include "Mu2eerdDevice.H"

using namespace Mu2eER;

Mu2eerdDevice::Mu2eerdDevice( string mqName, string shmName )
  : Device<32>( "Mu2eerdDevice", "mu2eerd Device" ),
    _mqName( mqName ),
    _shmName( shmName )
{
  registerMethod( ATTR_DAEMON_READ, 
                  *this, 
                  &Mu2eerdDevice::daemonRead, 
                  DAEMON_READ_MAX );
}

void Mu2eerdDevice::daemonRead( Array<daemon_read_t>& dest, ReqInfo const* reqinfo )
{
  if( dest.offset.getValue() > static_cast<int>( DAEMON_READ_MAX ) )
    {
      throw Ex_BADOFF;
    }

  if( (dest.offset.getValue() + dest.total.getValue() ) >
      static_cast<int>( DAEMON_READ_MAX ) )
    {
      throw Ex_BADOFLEN;
    }

  try
    {
      SharedMemoryClient shmc( _shmName );
      
      for( unsigned int i = 0; i != dest.total.getValue(); i++ )
        {
          switch( dest.offset.getValue() + i )
            {
            case DAEMON_READ_IDX_UPTIME:
              dest[i] = shmc.uptimeGet(); 
              break;
              
            case DAEMON_READ_IDX_PID:
              dest[i] = shmc.pidGet();
              break;
              
            case DAEMON_READ_IDX_JENKINS_NUM:
              dest[i] = _jenkinsNumberGet();
              break;
              
            default:
              throw Ex_BADOFLEN;
              break;
            }
        }
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in Mu2eerdDevice::daemonRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}

unsigned int Mu2eerdDevice::_jenkinsNumberGet() const
{
  return 0;
}
