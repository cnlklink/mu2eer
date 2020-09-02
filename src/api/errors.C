/**
 * errors.C
 *
 * This file contains global error definitions used throughout mu2eer.
 *
 * @author jdiamond
 */

#include "errors.H"

namespace Mu2eER
{
  // API module
  const Error API_CMQ_OPENFAIL( API, -1, "Client mq_open failure" );
  const Error API_CMQ_SENDFAIL( API, -2, "Client mq_send failure" );

  // Command line utility
  const Error MU2EERCLI_ARGUMENT_MISSING( MU2EERCLI, -2, "at least one argument is required");
  const Error MU2EERCLI_COMMAND_NOT_FOUND( MU2EERCLI, -1, "unrecognized command" );
  const Error MU2EERCLI_FAULT_ABORTED( MU2EERCLI, -5, "SSM fault request failed." );
  const Error MU2EERCLI_RESET_ABORTED( MU2EERCLI, -5, "SSM reset request failed." );
  const Error MU2EERCLI_SHUTDOWN_ABORTED( MU2EERCLI, -3, "mu2eerd shutdown request timeout" );
  const Error MU2EERCLI_START_ABORTED( MU2EERCLI, -4, "SSM start request timeout" );
};
