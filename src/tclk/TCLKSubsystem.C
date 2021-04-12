/**
 * TCLKSubsystem.C
 *
 * This file contains the implementation of the TCLKSubsystem class.
 *
 * @author jdiamond
 */

#include "TCLKDecoderFactory.H"
#include "TCLKSMB.H"
#include "TCLKSubsystem.H"

using namespace std;
using namespace Mu2eER;

TCLKSubsystem::TCLKSubsystem( const ConfigurationManager& cm, TCLKSMB& smb )
  : _configuration( cm ),
    _tclkDecoder( nullptr ),
    _sharedMemoryBlock( smb )
{
  _sharedMemoryBlock.driverNameSet( "none" );
}

TCLKSubsystem::~TCLKSubsystem()
{
  if( nullptr != _tclkDecoder.get() )
    {
      _stopEventReceiverThread();
    }
}

void TCLKSubsystem::initialize()
{
  _tclkDecoderSet( _configuration.tclkDriverGet() );

  _registerTCLKEventsToReceive();
  
  _startEventReceiverThread();
}

void TCLKSubsystem::_registerTCLKEventsToReceive()
{
  _tclkDecoder->eventListAdd( ITCLKDecoderDriver::TCLK_FTP_RESET_EVENT );
}

void TCLKSubsystem::_startEventReceiverThread()
{
  _eventReceiverThread = unique_ptr<thread>( new thread( [this]() {
        while( auto event = _tclkDecoder->waitForEvents() )
          {
            if( event == ITCLKDecoderDriver::TCLK_NEVER_EVENT )
              {
                // TODO - this is an error condition, need some way to trap it
                break;
              }

            _sharedMemoryBlock.eventCounterIncrement( event );
          }
      } ) );
}

void TCLKSubsystem::_stopEventReceiverThread()
{
  _eventReceiverThread->join();
}

void TCLKSubsystem::_tclkDecoderSet( string driverName )
{
  _tclkDecoder = TCLKDecoderFactory( driverName );
  _sharedMemoryBlock.driverNameSet( driverName );
}
