/**
 * SSMDevice.C
 *
 * This file contains the implementation of the SSMDevice class.
 *
 * @author jdiamond and rtadkins
 */

#include <syslog.h>

#include "SSMDevice.H"

using namespace Mu2eER;

SSMDevice::SSMDevice( string mqName, string shmName )
  : Device<32>( "SSMDevice", "Spill State Machine Device" ),
    _mqName( mqName ),
    _shmName( shmName )
{
<<<<<<< HEAD
<<<<<<< HEAD
  registerMethod( ATTR_STATE_READING,
                  *this,
                  &SSMDevice::stateRead,
                  STATE_READING_MAX );

=======
>>>>>>> d088ddc005dd1cd5da57d0ec9a66e8d2e2724f70
  registerMethod( ATTR_SPILL_COUNTER_READING,
                  *this,
                  &SSMDevice::spillCounterRead,
=======
  registerMethod( ATTR_SPILL_COUNTER_READING, 
                  *this, 
                  &SSMDevice::spillCounterRead, 
>>>>>>> master
                  SPILL_COUNTER_READING_MAX );

<<<<<<< HEAD
  registerMethod( ATTR_STATE_READING, 
                  *this, 
                  &SSMDevice::stateRead, 
=======
  registerMethod( ATTR_STATE_READING,
                  *this,
                  &SSMDevice::stateRead,
>>>>>>> d088ddc005dd1cd5da57d0ec9a66e8d2e2724f70
                  STATE_READING_MAX );

  registerMethods( ATTR_STATUS_CONTROL,
                   *this,
                   &SSMDevice::statusCtrlRead,
                   &SSMDevice::statusCtrlWrite,
                   1 );

<<<<<<< HEAD
<<<<<<< HEAD
  registerMethod( ATTR_IDEAL_SPILL_READING,
                   *this,
                   &SSMDevice::idealSpillRead,
                   IDEAL_SPILL_READING_MAX );
=======
  registerMethod( ATTR_TIS_READING,
                  *this,
                  &SSMDevice::timeInSpillRead,
                  TIS_READING_MAX );
>>>>>>> master
=======
  registerMethod( ATTR_TIS_READING,
                  *this,
                  &SSMDevice::timeInSpillRead,
                  TIS_READING_MAX );

  registerMethod( ATTR_IDEAL_SPILL_READING,
                  *this,
                  &SSMDevice::idealSpillRead,
                  IDEAL_SPILL_READING_MAX );
>>>>>>> d088ddc005dd1cd5da57d0ec9a66e8d2e2724f70
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
      dest[0] = shmc.ssmBlockGet().spillCounterGet();
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

    case CONTROL_FAULT:
      cmq.fault();
      return;

    default:
      syslog( LOG_ERR, "bad command in statusCtrlWrite(..) - %d", src[0] );
      throw Ex_BADSET;
    }
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
void SSMDevice::timeInSpillRead( Array<SSMDevice::tis_read_t>& dest,
                                 ReqInfo const* reqinfo )
{
  if( dest.offset.getValue() > static_cast<int>( TIS_READING_MAX ) )
    {
      throw Ex_BADOFF;
    }

  if( (dest.offset.getValue() + dest.total.getValue()) >
      static_cast<int>( TIS_READING_MAX ) )
    {
      throw Ex_BADOFLEN;
    }

  try
    {
      SharedMemoryClient shmc( _shmName );
      dest[0] = shmc.ssmBlockGet().timeInSpillGet();
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::timeInSpillRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}

>>>>>>> d088ddc005dd1cd5da57d0ec9a66e8d2e2724f70
void SSMDevice::idealSpillRead( Array<SSMDevice::ideal_spill_read_t>& dest,
                                  ReqInfo const* reqinfo )
{
  if( dest.offset.getValue() > static_cast<int>( IDEAL_SPILL_READING_MAX ) )
=======
void SSMDevice::timeInSpillRead( Array<SSMDevice::tis_read_t>& dest, 
                                 ReqInfo const* reqinfo )
{
  if( dest.offset.getValue() > static_cast<int>( TIS_READING_MAX ) )
>>>>>>> master
    {
      throw Ex_BADOFF;
    }

<<<<<<< HEAD
  if( (dest.offset.getValue() + dest.total.getValue()) >
      static_cast<int>( IDEAL_SPILL_READING_MAX ) )
=======
  if( (dest.offset.getValue() + dest.total.getValue()) > 
      static_cast<int>( TIS_READING_MAX ) )
>>>>>>> master
    {
      throw Ex_BADOFLEN;
    }

  try
    {
<<<<<<< HEAD
      int i = 0, size = 0;
      const int* idealSpillData;
      SharedMemoryClient shmc( _shmName );
      SpillStateMachineSMB smb = SpillStateMachineSMB();

      smb.initialize();
      idealSpillData = smb.idealSpillWaveFormGet();
      size = smb.idealSpillWaveFormSizeGet();

      for ( i = 0; i < size; i++ ) {
	       dest[i] = idealSpillData[i];
      }
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::idealSpillRead(..) - %s", e.what() );
=======
      SharedMemoryClient shmc( _shmName );
      dest[0] = shmc.ssmBlockGet().timeInSpillGet();
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::timeInSpillRead(..) - %s", e.what() );
>>>>>>> master
      throw Ex_DEVFAILED;
    }
}
