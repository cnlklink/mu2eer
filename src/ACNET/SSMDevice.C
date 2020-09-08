/**
 * SSMDevice.C
 *
 * This file contains the implementation of the SSMDevice class.
 *
 * @author jdiamond and rtadkins
 */

#include <syslog.h>
#include <iostream>
#include "SSMDevice.H"

using namespace Mu2eER;
using namespace std;
SSMDevice::SSMDevice( string mqName, string shmName )
  : Device<32>( "SSMDevice", "Spill State Machine Device" ),
    _mqName( mqName ),
    _shmName( shmName )
{
  registerMethod( ATTR_STATE_READING,
                  *this,
                  &SSMDevice::stateRead,
                  STATE_READING_MAX );

  registerMethod( ATTR_SPILL_COUNTER_READING,
                  *this,
                  &SSMDevice::spillCounterRead,
                  SPILL_COUNTER_READING_MAX );

  registerMethods( ATTR_STATUS_CONTROL,
                   *this,
                   &SSMDevice::statusCtrlRead,
                   &SSMDevice::statusCtrlWrite,
                   1 );

  registerMethod( ATTR_IDEAL_SPILL_READING,
                   *this,
                   &SSMDevice::idealSpillRead,
                   IDEAL_SPILL_READING_MAX );
}

void SSMDevice::spillCounterRead( Array<SSMDevice::spill_counter_read_t>& dest,
                                  ReqInfo const* reqinfo )
{
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
      dest[0] = 0;
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::spillCounterRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
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
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::stateRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}

void SSMDevice::statusCtrlRead( Array<status_t>& dest, ReqInfo const* reqinfo )
{
  throw Ex_DEVFAILED;
}

void SSMDevice::statusCtrlWrite( Array<const control_t>& src, ReqInfo const* reqinfo )
{
  if( src.offset.getValue() != 0 )
    {
      throw Ex_BADOFF;
    }

  if( src.total.getValue() != 1 )
    {
      throw Ex_BADOFLEN;
    }

  ControlMQClient cmq( _mqName );

  switch( src[0] )
    {
    case CONTROL_RESET:
      cmq.reset();
      return;

    case CONTROL_START:
      cmq.start();
      return;

    default:
      syslog( LOG_ERR, "bad command in statusCtrlWrite(..) - %d", src[0] );
      throw Ex_BADSET;
    }
}

void SSMDevice::idealSpillRead( Array<SSMDevice::ideal_spill_read_t>& dest,
                                  ReqInfo const* reqinfo )
{
  if( dest.offset.getValue() > static_cast<int>( IDEAL_SPILL_READING_MAX ) )
    {
      throw Ex_BADOFF;
    }

  if( (dest.offset.getValue() + dest.total.getValue()) >
      static_cast<int>( IDEAL_SPILL_READING_MAX ) )
    {
      throw Ex_BADOFLEN;
    }

  try
    {
      int i = 0, size = 0;
      const int* idealSpillData;
      SharedMemoryClient shmc( _shmName );
      SpillStateMachineSMB smb = SpillStateMachineSMB();
      
      cout << " Here in SSMDevice class" << endl;
      
      smb.initialize();
      idealSpillData = smb.idealSpillWaveFormGet();
      size = smb.idealSpillWaveFormSizeGet();
      for ( i = 0; i < size; i++ ) {
	dest[i] = idealSpillData[i];
      }
      cout << "Finished transferring data" << endl;
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::idealSpillRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}
