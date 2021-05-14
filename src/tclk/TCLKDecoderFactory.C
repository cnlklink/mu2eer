/**
 * TCLKDecoderFactory.C
 *
 * This file contains the implementatin of the TCLKDecoderFactory.
 *
 * @author jdiamond
 */

#include "tclk.H"

#include "MulticastTCLKDecoderDriver.H"
#include "TCLKDecoderDriverMock.H"

namespace Mu2eER
{
  unique_ptr<ITCLKDecoderDriver> TCLKDecoderFactory( string driverName )
  {
    if( 0 == driverName.compare( "mock" ) )
      {
        auto mock = new TCLKDecoderDriverMock();
        
        mock->eventSequenceSet( { 0x02 } );

        return unique_ptr<ITCLKDecoderDriver>( mock );
      }
    else if( 0 == driverName.compare( "multicast" ) )
      {
        return unique_ptr<ITCLKDecoderDriver>( new MulticastTCLKDecoderDriver() );
      }
    else if( 0 == driverName.compare( "onboard" ) )
      {
        throw runtime_error( "Not implemented yet" );
      }

    throw runtime_error( "Unrecognized TCLK driver requested." );
  }
};
