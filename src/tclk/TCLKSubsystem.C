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
  : _tclkDecoder( nullptr ),
    _sharedMemoryBlock( smb )
{
  _sharedMemoryBlock.driverNameSet( "none" );
}

void TCLKSubsystem::initialize()
{
  _tclkDecoderSet( "mock" );
}

void TCLKSubsystem::_tclkDecoderSet( string driverName )
{
  _tclkDecoder = TCLKDecoderFactory( driverName );
  _sharedMemoryBlock.driverNameSet( driverName );
}
