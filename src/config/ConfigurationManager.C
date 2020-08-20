/**
 * ConfigurationManager.C
 *
 * This file contains the implementation of the ConfigurationManager class.
 *
 * @author jdiamond
 */

#include <stdexcept>
#include <string>
#include <syslog.h>
#include <unistd.h>

#include "ConfigurationManager.H"

using namespace Mu2eER;
using namespace std;

// Configuration module
const Error CONFIG_FILE_ERROR( CONFIG, -1, "The configuration file specified with -c option was not found or could not be accessed." );

const Error CONFIG_FILE_PARSE_ERROR( CONFIG, -2, "Failed to parse configuration file." );

const Error CONFIG_TCLK_MISSING( CONFIG, -3, "Missing tclk configuration section." );

const Error CONFIG_TCLK_DRIVER_MISSING( CONFIG, -4, "Missing tclk.driver parameter." );

string ConfigurationManager::configFileGet() const
{
  return _path;
}

ConfigurationManager::ConfigurationManager()
  : _path( DEFAULT_CONFIG_FILE_PATH )
{
  _defaultInit();
}

void ConfigurationManager::_defaultInit()
{
  // The default configuration should put the system in the least-operable state.

  auto& root = _cfg.getRoot();

  // Spill state machine defaults
  root.add( "ssm", libconfig::Setting::TypeGroup );
  auto& ssm = root["ssm"];

  ssm.add( "driver", libconfig::Setting::TypeString );
  ssm["driver"] = "mock";

  ssm.add( "auto_init", libconfig::Setting::TypeBoolean );
  ssm["auto_init"] = false;  

  ssm.add( "mock_spills", libconfig::Setting::TypeInt );
  ssm["mock_spills"] = 0;

  ssm.add( "mock_delay", libconfig::Setting::TypeInt );
  ssm["mock_delay"] = 0;

  // TCLK defaults
  root.add( "tclk", libconfig::Setting::TypeGroup );
  auto& tclk = root["tclk"];
  
  tclk.add( "driver", libconfig::Setting::TypeString );
  tclk["driver"] = "mock";
}

string ConfigurationManager::hostConfigFileGet()
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
  string hostname( dotpos == string::npos ? fqdn : fqdn.substr( 0, dotpos ) );

  return "/etc/mu2eer.d/" + hostname + "-mu2eerd.conf";
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
      throw CONFIG_FILE_ERROR;
    }
  catch( libconfig::ParseException e )
    {
      throw CONFIG_FILE_PARSE_ERROR;
    }

  syslog( LOG_INFO, "Configuration loaded from %s", _path.c_str() );
}

SSMConfig ConfigurationManager::ssmGet() const
{
  return SSMConfig( _cfg.getRoot()["ssm"] );
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
