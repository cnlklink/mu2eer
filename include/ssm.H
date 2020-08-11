/**
 * ssm.H
 *
 * This file contains the public interface to the Spill State Machine (ssm) module.
 *
 * @author jdiamond
 */

#ifndef SSM_H
#define SSM_H

#include "SSMConfig.H"

#include <stdexcept>

namespace Mu2eER
{
  /**
   * Spill State Machine State Definitions
   */
  enum ssm_state_t
    {
      SSM_IDLE = 0,
      SSM_INIT = 1,
      SSM_BETWEEN_CYCLES = 2,
      SSM_START_CYCLE = 3,
      SSM_BETWEEN_SPILLS = 4,
      SSM_READ_IBEAM = 5,
      SSM_RAMP = 6,
      SSM_SPILL = 7,
      SSM_START_EOS_TIMER = 8,
      SSM_SPILL_PARABOLA = 21,
      SSM_START_CLEANUP_TIMER = 19,
      SSM_AFTER_SPILL = 20,
      SSM_END_CYCLE = 22,
      SSM_LEARNING = 14,
      SSM_ABORT = 23,
      SSM_FAULT = 254,
      SSM_UNKNOWN = 255,
    };
  
  /**
   * Exception from the SSM module
   */
  typedef std::runtime_error ssm_error;
   
  /**
   * Stream Insertion Operator
   *
   * Converts state to a string and insert it into the given stream.
   *
   * @param os Output stream
   * @param state State to convert
   * @return Output stream
   */
  ostream& operator<<( ostream& os, const ssm_state_t state );
}
#endif
