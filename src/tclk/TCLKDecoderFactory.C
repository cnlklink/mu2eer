/**
 * TCLKDecoderFactory.C
 *
 * This file contains the implementatin of the TCLKDecoderFactory.
 *
 * @author jdiamond
 */

#include "tclk.H"
#include "TCLKDecoderDriverMock.H"

namespace Mu2eER
{
  unique_ptr<ITCLKDecoderDriver> TCLKDecoderFactory( string driverName, TCLKSMB& tclkBlock )
  {
    if( 0 == driverName.compare( "mock" ) )
      {
        return unique_ptr<ITCLKDecoderDriver>( new TCLKDecoderDriverMock( tclkBlock ) );
      }
    else if( 0 == driverName.compare( "multicast" ) )
      {
        throw runtime_error( "Not implemented yet" );
      }
    else if( 0 == driverName.compare( "onboard" ) )
      {
        throw runtime_error( "Not implemented yet" );
      }

    throw runtime_error( "Unrecognized TCLK driver requested." );
  }
};
