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
     * Set Mock Delay
     *
     * @param delay Delay between mock state transitions (ms)
     */
    void mockDelaySet( unsigned int spills );

    /**
     * Get Mock Delay
     *
     * @return Delay between mock state transitions (ms)
     */
    unsigned int mockDelayGet() const;

    /**
     * Set Mock Spills
     *
     * @param spills The number of spills to fake ("mock" driver only)
     */
    void mockSpillsSet( unsigned int spills );

    /**
     * Get Mock Spills
     *
     * @return The number of spills to fake ("mock" driver only)
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
