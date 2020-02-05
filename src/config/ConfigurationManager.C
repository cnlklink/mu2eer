/**
 * ConfigurationManager.C
 *
 * This file contains the implementation of the ConfigurationManager class.
 *
 * @author jdiamond
 */

#include <stdexcept>
#include <string>
#include <unistd.h>

#include "ConfigurationManager.H"

using namespace Mu2eER;
using namespace std;

/**
 * Thrown if the configuration file is not accessible
 */
config_error CONFIG_FILE_ERR( "Failed to open configuration file" );

/**
 * Thrown if the configuration file can not be parsed
 */
config_error CONFIG_PARSE_ERR( "Failed to parse configuration file" );

/**
 * Missing tclk section
 */
config_error CONFIG_TCLK_MISSING( "Missing tclk configuration" );

/**
 * Missing tclk.driver parameter
 */
config_error CONFIG_TCLK_DRIVER_MISSING( "Missing tclk.driver parameter" );

ConfigurationManager::ConfigurationManager()
{
  _defaultInit();
}

void ConfigurationManager::_defaultInit()
{
  auto& root = _cfg.getRoot();

  // Spill state machine defaults
  root.add( "ssm", libconfig::Setting::TypeGroup );
  auto& ssm = root["ssm"];

  ssm.add( "driver", libconfig::Setting::TypeString );
  ssm["driver"] = "mock";

  // TCLK defaults
  root.add( "tclk", libconfig::Setting::TypeGroup );
  auto& tclk = root["tclk"];
  
  tclk.add( "driver", libconfig::Setting::TypeString );
  tclk["driver"] = "mock";
}

string ConfigurationManager::hostnameGet() const
{
  // Retrieve the hostname
  char cstr[255];
  if( gethostname( cstr, sizeof( cstr ) ) )
    {
      throw runtime_error( "gethostname(..) failure" );
    }

  // Remove the domain name if necessary
  string fqdn( cstr );
  auto dotpos = fqdn.find_first_of( "." );
  return dotpos == string::npos ? fqdn : fqdn.substr( 0, dotpos );
}

void ConfigurationManager::load( string path )
{
  _path = path;
  reload();
}

void ConfigurationManager::reload()
{
  try
    {
      _cfg.readFile( _path.c_str() );
    }
  catch( libconfig::FileIOException e )
    {
      throw CONFIG_FILE_ERR;
    }
  catch( libconfig::ParseException e )
    {
      throw CONFIG_PARSE_ERR;
    }
}

string ConfigurationManager::ssmDriverGet() const
{
  const auto& root = _cfg.getRoot();
  return root["ssm"]["driver"];
}

string ConfigurationManager::tclkDriverGet() const
{
  const auto& root = _cfg.getRoot();
  return root["tclk"]["driver"];
}

void ConfigurationManager::tclkDriverSet( const string& driver )
{
  const auto& root = _cfg.getRoot();
  root["tclk"]["driver"] = driver;
}
