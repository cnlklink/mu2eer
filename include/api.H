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
   * mu2eerd control commands
   */
  enum command_t
    {
      COMMAND_SHUTDOWN = 0,
      COMMAND_INVALID = 255
    };

  /**
   * mu2eerd control message
   */
  struct control_msg_t
  {
    /**
     * Control command
     */
    command_t command;
  };

  /**
   * Exception from the API module
   */
  typedef std::runtime_error api_error;
};

#endif
