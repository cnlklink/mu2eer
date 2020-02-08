/**
 * SpillStateMachineSMB.C
 *
 * This file contains the implementation of the SpillStateMachineSMB class.
 *
 * @author jdiamond
 */

#include "SpillStateMachineSMB.H"

using namespace Mu2eER;
using namespace std;

SpillStateMachineSMB::SpillStateMachineSMB()
  : _currentState( SSM_UNKNOWN )
{
}

void SpillStateMachineSMB::currentStateSet( ssm_state_t state )
{
  _currentState = state;
}

ssm_state_t SpillStateMachineSMB::currentStateGet() const
{
  return _currentState;
}
