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
};

#endif
