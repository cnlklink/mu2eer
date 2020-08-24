/**
 * config.H
 *
 * This file contains the public interface to the Configuration (config) module.
 *
 * @author jdiamond
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "Error.H"

#include <stdexcept>

namespace Mu2eER
{
  /**
   * Configuration file I/O error
   */
  extern const Error CONFIG_FILE_ERROR;

  /**
   * Configuration file parse error
   */
  extern const Error CONFIG_FILE_PARSE_ERROR;

  /**
   * Missing TCLK configuration error
   */
  extern const Error CONFIG_TCLK_MISSING;

  /**
   * Missing tclk.driver error
   */
  extern const Error CONFIG_TCLK_DRIVER_MISSING;
};

#endif
