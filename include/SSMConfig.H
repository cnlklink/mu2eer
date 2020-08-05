/**
 * SSMConfig.H
 *
 * This file contains the definition of the SSMConfig class.
 *
 * @author jdiamond
 */

#ifndef SSMCONFIG_H
#define SSMCONFIG_H

#include <libconfig.h++>

using namespace std;

namespace Mu2eER
{
  /**
   * SSMConfig
   *
   * Holds the configuration for the Spill State Machine sub-system.
   */
  class SSMConfig
  {
  public:
    /**
     * Constructor
     *
     * @param cfg Reference to the ssm configuration
     */
    SSMConfig( libconfig::Setting& cfg );

    /**
     * Get Auto-Init Flag
     *
     * @return True if the ssm should be initialized automatically on start-up
     */
    bool autoInitGet() const;

    /**
     * Set Auto-Init Flag
     *
     * @param autoInit True if the ssm should be initialize automaticall on start-up
     */
    void autoInitSet( bool autoInit );

    /**
     * Get Driver
     *
     * @return "mock" or "real"
     */
    string driverGet() const;

    /**
     * Get Mock Spills
     *
     * @return The number of spills to fake ("mock" driver)
     */
    unsigned int mockSpillsGet() const;

  private:
    /**
     * Reference to the "ssm" section in the configuration file.
     */
    libconfig::Setting& _cfg;
  };
};

#endif
