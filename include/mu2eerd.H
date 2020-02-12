/**
 * mu2eerd.H
 *
 * This file contains the public interface to mu2eerd.
 *
 * @author jdiamond
 */

#ifndef MU2EERD_H
#define MU2EERD_H

namespace Mu2eER
{
  /**
   * State of the mu2eerd daemon
   */
  enum mu2eerd_state_t
    {
      MU2EERD_INITIALIZING,
      MU2EERD_STARTUP,
      MU2EERD_RUNNING,
      MU2EERD_SHUTDOWN,
      MU2EERD_FAULT = 255
    };
};

#endif
