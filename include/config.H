/**
 * config.H
 *
 * This file contains the public interface to the Configuration (config) module.
 *
 * @author jdiamond
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdexcept>

namespace Mu2eER
{
  /**
   * Exception from the Config module
   */
  typedef std::runtime_error config_error;
};

#endif
