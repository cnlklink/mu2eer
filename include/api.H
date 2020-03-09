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

/**
 * Name of the mu2eerd control message queue
 */
#define MU2EERD_CMQ_NAME "/mu2eer"

/**
 * Name of the mu2eerd shared memory region
 */
#define MU2EERD_SHM_NAME "mu2eer"

namespace Mu2eER
{
  /**
   * mu2eerd control commands
   */
  enum command_t
    {
      COMMAND_SHUTDOWN = 0,
      COMMAND_SSM_INIT,
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
   * mu2eer modules
   */
  enum module_t
    {
      ACNET,
      ADC,
      API,
      CONFIG,
      MU2EERCLI,
      MU2EERD,
      SSM,
      TCLK
    };

  /**
   * Deprecated api_error type
   */
  typedef std::runtime_error api_error;
};

#endif
