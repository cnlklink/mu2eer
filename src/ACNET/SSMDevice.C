/**
 * SSMDevice.C
 *
 * This file contains the implementation of the SSMDevice class.
 *
 * @author jdiamond and rtadkins
 */

#include <syslog.h>
#include <iostream>

#include "errors.H"
#include "SSMDevice.H"

using namespace Mu2eER;
using namespace std;

SSMDevice::SSMDevice( string mqName, string shmName )
  : Device<32>( "SSMDevice", "Spill State Machine Device" ),
    _mqName( mqName ),
    _shmName( shmName )
{

  for( uint32_t i = 0; i != MAX_FTP; i++ )
  {
    _active_ftp[i].acsys_fe_request_id = 0;
  }

  _readfast_error_count = 0;

  registerMethod( ATTR_SPILL_COUNTER_READING,
                  *this,
                  &SSMDevice::spillCounterRead,
                  SPILL_COUNTER_READING_MAX );

  registerMethod( ATTR_STATE_READING,
                  *this,
                  &SSMDevice::stateRead,
                  STATE_READING_MAX );

  registerMethods( ATTR_STATUS_CONTROL,
                   *this,
                   &SSMDevice::statusCtrlRead,
                   &SSMDevice::statusCtrlWrite,
                   1 );

  registerMethod( ATTR_TIS_READING,
                  *this,
                  &SSMDevice::timeInSpillRead,
                  TIS_READING_MAX );

  registerMethod( ATTR_IDEAL_SPILL_READING,
                  *this,
                  &SSMDevice::idealSpillRead,
                  IDEAL_SPILL_READING_MAX );

  registerMethod( ATTR_ACTUAL_SPILL_READING,
                  *this,
                  &SSMDevice::actualSpillRead,
                  IDEAL_SPILL_READING_MAX );

  registerMethod( ATTR_ERROR_SIGNAL_READING,
                  *this,
                  &SSMDevice::errorSignalRead,
                  IDEAL_SPILL_READING_MAX );

  registerMethodFast( ATTR_IDEAL_SPILL_READING,
                  *this,
                  &SSMDevice::readFast,
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

  try
    {
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
          syslog( LOG_ERR, "bad command in SSMDevice::statusCtrlWrite(..) - %d", src[0] );
          throw Ex_BADSET;
        }
    }
  catch( Error e )
    {
      syslog( LOG_ERR, "Error caught in SSMDevice::statusCtrlWrite(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}

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

void SSMDevice::idealSpillRead( Array<SSMDevice::ideal_spill_read_t>& dest,
				ReqInfo const* reqinfo )
{
  syslog( LOG_INFO, "Entered into SSMDevice::idealSpillRead() \n" );

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
      int i = 0, j = 0, upper_bound = 0, lower_bound = dest.offset.getValue(), sample_size = dest.total.getValue();
      SharedMemoryClient shmc( _shmName );

      auto idealSpillData = shmc.ssmBlockGet().idealSpillWaveFormGet();
      upper_bound = lower_bound + sample_size;

       for ( i = lower_bound; i < upper_bound; i++ ) {
	 dest[j] = idealSpillData[i];
	 j++;
       }
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::idealSpillRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }

  syslog( LOG_INFO, "Finished in SSMDevice::idealSpillRead() \n" );
}

void SSMDevice::actualSpillRead( Array<SSMDevice::actual_spill_read_t>& dest,
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
      int i = 0, j = 0, upper_bound = 0, lower_bound = dest.offset.getValue(), sample_size = dest.total.getValue();
      SharedMemoryClient shmc( _shmName );

      auto actualSpillData = shmc.ssmBlockGet().actualSpillWaveFormGet();
      upper_bound = lower_bound + sample_size;

      for ( i = lower_bound; i < upper_bound; i++ ) {
	       dest[j] = actualSpillData[i];
	       j++;
      }
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::actualSpillRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}

void SSMDevice::errorSignalRead( Array<SSMDevice::error_signal_read_t>& dest,
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
      int i = 0, j = 0, upper_bound = 0, lower_bound = dest.offset.getValue(), sample_size = dest.total.getValue();
      SharedMemoryClient shmc( _shmName );

      auto errorSignalData = shmc.ssmBlockGet().errorSignalWaveFormGet();
      upper_bound = lower_bound + sample_size;

      for ( i = lower_bound; i < upper_bound; i++ ) {
	       dest[j] = errorSignalData[i];
	       j++;
      }
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::errorSignalRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}

/**
 * Init Collection Property
 * Code by kmartin and adapted by rtadkins

 * @param reqinfo ACNET request object
 */
void SSMDevice::initCollection( ReqInfo const* reqinfo )
{
  uint32_t evType, evVal;
  uint32_t request_index = 0;
  uint32_t request_id = 0;
  uint32_t i;
  uint32_t dataRate = 0;

  syslog( LOG_INFO, "Entered into SSMDevice::initCollection() \n" );

  reqinfo->get_evTypeVal(&evType, &evVal);

  if ( evType == EV_PERIODIC )
  {
    if ( (evVal >= 66000) && (evVal != 66600) && (evVal != 6670) )
    {
      return;
    }
  }
  else
  {
    if ( evVal >= 66000 )
      return;
    if ( evVal < 694 ) //use 694us instead
      return;
  }

  _readfast_error_count = 0;

  for ( i = 0; i < MAX_FTP; i++ )
  {
    if ( _active_ftp[i].acsys_fe_request_id == 0 )
    {
      request_id = reqinfo->get_requestid();
      break;
    }
  }

  if ( request_id == 0 )
  {
    syslog( LOG_INFO, "SSMDevice::initCollection(): ERROR - All ftp_requests are in use." );
    throw Ex_FTPLIMIT;
  }

  request_index = i;

  dataRate = (uint32_t)( 1000000.0 / ((float)evVal) );

  _active_ftp[request_index].acsys_fe_request_id = reqinfo->get_requestid();
  _active_ftp[request_index].dataRate = dataRate;

  syslog( LOG_INFO, "Exiting SSMDevice::initCollection() \n" );
}

/**
 * Fast Reading Property
 * Code by kmartin and adapted by rtadkins
 *
 * @param dest return buffer
 * @param reqinfo ACNET request object
 */
void SSMDevice::readFast( Array<SafeFloat>& dest, ReqInfo const* reqinfo )
{
  uint32_t request_index = 0;
  uint32_t request_id = reqinfo->get_requestid();
  //uint32_t num_read_pts = 16000;
  uint32_t num_read_pts = 2666; 
  uint32_t i;

  syslog( LOG_INFO, "Entered into SSMDevice::readFast() \n");

  for ( i = 0; i < MAX_FTP; i++ )
  {
    if ( _active_ftp[i].acsys_fe_request_id == request_id )
    {
      break;
    }
  }

  if ( i == MAX_FTP )
  {
    _readfast_error_count++;
    if ( _readfast_error_count <= 8 )
    {
      syslog( LOG_INFO, "SSMDevice::readFast(): request_id(%d) not found,\n", request_id );
    }
    throw Ex_FTPLIMIT;
    return;
  }

  request_index = i;

  syslog( LOG_INFO, "Show length %d and offset %d\n", dest.total, dest.offset);
  syslog( LOG_INFO, "Show length of num_read_pts %d\n", num_read_pts);
  try {
    SharedMemoryClient shmc( _shmName );
    auto spil = shmc.ssmBlockGet();
    int val = 0;
    spil.fillCircularBuffer();

    for ( i = 0; i < num_read_pts; i++ )
      {
	syslog (LOG_INFO, "data is: %f\n", spil.circularBufferGet().dataGet(i));
	val = (spil.circularBufferGet().dataGet(i) + 1) * 100;
	syslog (LOG_INFO, "val is %d\n", val);
	dest[i] = val;
	syslog (LOG_INFO, "dest[i] = %d\n", dest[i]);
      }
  }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in SSMDevice::fastRead(..) - %s", e.what() );
      cleanupCollection (reqinfo);
      throw Ex_DEVFAILED;
    }

  dest[0] = (uint32_t)(num_read_pts * sizeof(SafeFloat)); // write the number of bytes being returned in the buffer into the first 2 bytes of the first element

  syslog( LOG_INFO, "Finished in SSMDevice::readFast() \n" );
}

/**
 * Cleanup Collection Property
 * Code by kmartin and adapted by rtadkins

 * @param reqinfo ACNET request object
 */
void SSMDevice::cleanupCollection( ReqInfo const* reqinfo )
{
  uint32_t request_index = 0;
  uint32_t i;
  uint32_t request_id = reqinfo->get_requestid();

  syslog( LOG_INFO, "Entered into SSMDevice::cleanupCollection() \n");

  for ( i = 0; i < MAX_FTP; i++ )
  {
    if ( _active_ftp[i].acsys_fe_request_id == request_id )
    {
      break;
    }
  }

  if ( i == MAX_FTP )
  {
    syslog( LOG_INFO, "SSMDevice::cleanupCollection(): request_id(%d) not found,\n", request_id );
    return;
  }

  request_index = i;
  _active_ftp[request_id].acsys_fe_request_id = 0;

  syslog( LOG_INFO, "Finished SSMDevice::cleanupCollection() \n");
}
