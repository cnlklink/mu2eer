/**
 * ConfigurationManager.H
 *
 * This file contains the definition of the ConfigurationManager class.
 *
 * @author jdiamond
 */

#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "config.H"

#include <libconfig.h++>
#include <string>

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
     * Default constructor
     */
    ConfigurationManager();

    /**
     * Get Hostname
     *
     * @return Hostname
     */
    string hostnameGet() const;

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
     * Set Spill State Machine Driver
     *
     * @return "mock" or "real"
     */
    string ssmDriverGet() const;

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
