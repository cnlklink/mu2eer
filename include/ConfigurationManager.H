/**
 * ConfigurationManager.H
 *
 * This file contains the definition of the ConfigurationManager class.
 *
 * @author jdiamond
 */

#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "Error.H"
#include "SSMConfig.H"

#include <libconfig.h++>
#include <string>

#define DEFAULT_CONFIG_FILE_PATH "none (default configuration loaded)"

using namespace std;

namespace Mu2eER
{
  /**
   * ConfigurationManager
   *
   * This class manages the Mu2eER configuration.  The user provides the configuration via a config
   * file (typically /etc/mu2eer.d/<node-name>.conf) with the -c option to mu2eerd.  The
   * ConfigurationManager handles loading and parsing this file and provides an API for accessing
   * the configuration sections directly.  If no configuration file is loaded then a default
   * configuration is provided.
   *
   * This class also provides access to parameters that come from the environment (e.g. hostname).
   */
  class ConfigurationManager
  {
  public:
    /**
     * Get Configuration File
     *
     * @return Full path to configuration file or DEFAULT_CONFIG_FILE_PATH if none was loaded
     */
    string configFileGet() const;

    /**
     * Default constructor
     */
    ConfigurationManager();

    /**
     * Get Host Configuration File
     *
     * Returns a path to the configuration file for this node.  The path is in the form:
     * /etc/mu2eer.d/hostname-mu2eerd.conf
     *
     * @return Path to the configuration file for this node
     */
    static string hostConfigFileGet();

    /**
     * Get Hostname
     *
     * @return Hostname (short version) that we're running on
     */
    static string hostnameGet();

    /**
     * Load
     *
     * Load the given configuration file.
     *
     * @param path Path to configuration file.
     */
    void load( string path );

    /**
     * Reload
     *
     * Re-load configuration file from previous call to load(..).
     */
    void reload();

    /**
     * Get Spill State Machine Configuration
     *
     * @return Spill state machine configuration object
     */
    SSMConfig ssmGet() const;

    /**
     * Get TCLK Driver
     *
     * @return "mock" or "real"
     */
    string tclkDriverGet() const;

    /**
     * Set TCLK Driver
     *
     * @param driver TCLK driver
     */
    void tclkDriverSet( const string& driver );

  private:
    /**
     * Parsed libconfig configuration data
     */
    libconfig::Config _cfg;

    /**
     * Path to configuration file
     */
    string _path;

    /**
     * Initialize _cfg with defaults
     */
    void _defaultInit();
  };
};

#endif
