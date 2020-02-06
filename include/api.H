/**
 * api.H
 *
 * This file contains the public interface to the API module.
 *
 * @author jdiamond
 */

#ifndef API_H
#define API_H

#include <stdexcept>
#include <string>

/**
 * API Version
 */
#define API_VERSION "1.0"

namespace Mu2eER
{
  /**
   * Exception from the API module
   */
  typedef std::runtime_error api_error;
};

#endif
