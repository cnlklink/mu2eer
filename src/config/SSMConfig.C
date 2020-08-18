/**
 * SSMConfig.C
 *
 * This file contains the implementation of the SSMConfig class.
 *
 * @author jdiamond
 */

#include <stdexcept>
#include <string>
#include <unistd.h>

#include "SSMConfig.H"

using namespace Mu2eER;
using namespace std;

SSMConfig::SSMConfig( libconfig::Setting& cfg )
  : _cfg( cfg )
{
}

bool SSMConfig::autoInitGet() const
{
  return _cfg["auto_init"];
}

void SSMConfig::autoInitSet( bool autoInit )
{
  _cfg["auto_init"] = autoInit;
}

string SSMConfig::driverGet() const
{
  return _cfg["driver"];
}

void SSMConfig::mockDelaySet( unsigned int delay )
{
  _cfg["mock_delay"] = static_cast<int>( delay );
}

unsigned int SSMConfig::mockDelayGet() const
{
  return _cfg["mock_delay"];
}

void SSMConfig::mockSpillsSet( unsigned int spills )
{
  _cfg["mock_spills"] = static_cast<int>( spills );
}

unsigned int SSMConfig::mockSpillsGet() const
{
  return _cfg["mock_spills"];
}

