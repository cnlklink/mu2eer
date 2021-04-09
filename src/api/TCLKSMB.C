/**
 * TCLKSMB.C
 *
 * This file contains the implementation of the TCLKSMB class.
 *
 * @author jdiamond
 */
#include <syslog.h>
#include <cmath>
#include "TCLKSMB.H"

using namespace Mu2eER;
using namespace std;

TCLKSMB::TCLKSMB()
{
  driverNameSet( "none" );

  _resetEventCounters();
}

string TCLKSMB::driverNameGet() const
{
  return _driverName;
}

void TCLKSMB::driverNameSet( string driverName )
{
  driverName.copy( _driverName, driverName.length(), 0 );
  _driverName[driverName.length()] = 0;
}

unsigned int TCLKSMB::eventCounterGet( uint8_t event ) const
{
  return _eventCounter[event];
}

void TCLKSMB::_resetEventCounters()
{
  for( uint8_t i = 0; i < 255; i++ )
    {
      _eventCounter[i] = 0;
    }
}

