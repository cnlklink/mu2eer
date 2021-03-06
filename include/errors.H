/**
 * errors.H
 *
 * This file contains error definitions used between modules.
 *
 * @author jdiamond
 */

#ifndef ERRORS_H
#define ERRORS_H

#include "api.H"
#include "Error.H"

namespace Mu2eER
{
  /**
   * Message queue open failure
   */
  extern const Error API_CMQ_OPENFAIL;
  
  /**
   * Message queue send failure
   */
  extern const Error API_CMQ_SENDFAIL;

  /**
   * Command not found error
   */
  extern const Error MU2EERCLI_COMMAND_NOT_FOUND;

  /**
   * Insufficient arguments provided to mu2eercli
   */
  extern const Error MU2EERCLI_ARGUMENT_MISSING;

  /**
   * Fault command failed.
   */
  extern const Error MU2EERCLI_FAULT_ABORTED;

  /**
   * Reset command failed.
   */
  extern const Error MU2EERCLI_RESET_ABORTED;

  /**
   * Shutdown command did not complete within the timeout period
   */
  extern const Error MU2EERCLI_SHUTDOWN_ABORTED;

  /**
   * Start command did not complete within the timeout period
   */
  extern const Error MU2EERCLI_START_ABORTED;

  /**
   * Timeout occurred while waiting for mu2eerd
   */
  extern const Error MU2EERCLI_TIMEOUT;
};

#endif
