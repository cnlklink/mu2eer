/**
 * Mu2eerdDevice.C
 *
 * This file contains the implementation of the Mu2eerdDevice class.
 *
 * @author jdiamond
 */

#include <syslog.h>

#include "DaemonController.H"
#include "errors.H"
#include "ISystemController.H"
#include "Mu2eerdDevice.H"

using namespace Mu2eER;

Mu2eerdDevice::Mu2eerdDevice( ISystemController& sysCtrl,
                              DaemonController daemonCtrl, 
                              string mqName, 
                              string shmName )
  : Device<32>( "Mu2eerdDevice", "mu2eerd Device" ),
    _daemonCtlr( daemonCtrl ),
    _mqName( mqName ),
    _shmName( shmName ),
    _sysCtlr( sysCtrl )
{
  registerMethod( ATTR_DAEMON_READ, 
                  *this, 
                  &Mu2eerdDevice::daemonRead, 
                  DAEMON_READ_MAX );

  registerMethods( ATTR_DAEMON_STATUSCTRL,
                   *this,
                   &Mu2eerdDevice::daemonStatus,
                   &Mu2eerdDevice::daemonControl,
                   DAEMON_STATUSCTRL_MAX );
}

void Mu2eerdDevice::daemonControl( Array<const daemon_statusctrl_t>& src, ReqInfo const* reqinfo )
{
  if( src.offset.getValue() != 0 )
    {
      throw Ex_BADOFF;
    }

  if( src.total.getValue() != 1 )
    {
      throw Ex_BADOFLEN;
    }

  try
    {
      switch( src[0] )
        {
        case DAEMON_CONTROL_SOFTREBOOT:
          _reboot();
          break;

        case DAEMON_CONTROL_START:
          _start();
          break;

        case DAEMON_CONTROL_STOP:
          _stop();
          break;
          
        default:
          syslog( LOG_ERR, "bad command in daemonControl(..) - %d", src[0] );
          throw Ex_BADSET;
        }
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in daemonControl(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
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
              dest[i] = _sysCtlr.jenkinsNumberGet();
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

void Mu2eerdDevice::daemonStatus( Array<daemon_statusctrl_t>& dest, ReqInfo const* reqinfo )
{
  if( dest.offset.getValue() > static_cast<int>( DAEMON_STATUSCTRL_MAX ) )
    {
      throw Ex_BADOFF;
    }

  if( (dest.offset.getValue() + dest.total.getValue() ) >
      static_cast<int>( DAEMON_STATUSCTRL_MAX ) )
    {
      throw Ex_BADOFLEN;
    }

  try
    {
      dest[0] = 0;

      // Daemon process running/not running status
      dest[0] |= _daemonCtlr.isRunning() ? DAEMON_STATUS_RUNNING : 0;

      // Shared memory status
      try
        {
          SharedMemoryClient shmc( _shmName );
          dest[0] |= DAEMON_STATUS_SHM_CONNECTED;
        }
      catch( runtime_error e )
        {
          // Not connected
        }

      // Control queue status
      try
        {
          ControlMQClient cmq( _mqName );
          dest[0] |= DAEMON_STATUS_CMQ_CONNECTED;
        }
      catch( runtime_error e )
        {
          // Not connected
        }
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in Mu2eerdDevice::daemonRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}

void Mu2eerdDevice::_reboot()
{
  if( _sysCtlr.isRebooting() )
    {
      syslog( LOG_ERR, "attempt to reboot when we're already rebooting" );
      throw Ex_BADSET;
    }
  
  _sysCtlr.doReboot();
}

void Mu2eerdDevice::_start()
{
  if( _daemonCtlr.isRunning() )
    {
      syslog( LOG_ERR, "attempt to start mu2eerd when mu2eerd is already running" );
      throw Ex_BADSET;
    }
  _daemonCtlr.start();
}

void Mu2eerdDevice::_stop()
{
  if( !_daemonCtlr.isRunning() )
    {
      syslog( LOG_ERR, "attempt to stop mu2eerd when mu2eerd is not running" );
      throw Ex_BADSET;
    }
  _daemonCtlr.stop();
}
