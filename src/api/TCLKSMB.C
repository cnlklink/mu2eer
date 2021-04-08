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
  : _eventCounter( 0 )
{
  driverNameSet( "none" );
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

unsigned int TCLKSMB::eventCounterGet() const
{
  return _eventCounter;
}
