/**
 * SpillStateMachine.C
 *
 * This file contains the implementation of the SpillStateMachine class.
 *
 * @author jdiamond
 */

#include <iostream>

#include "SpillStateMachine.H"
#include "SSMDeviceDriverMock.H"

using namespace Mu2eER;
using namespace std;

/**
 * The SSM module was not initialized
 */
ssm_error SSM_NOT_READY( "SSM Not Ready" );

void SpillStateMachine::initialize()
{
  cout << "Initializing Spill Statate Machine...";

  // Create a mock SSM device and program some state changes
  _ssmDev = 
    unique_ptr<ISSMDeviceDriver>( new SSMDeviceDriverMock( { 
          SSM_BETWEEN_CYCLES, 
          SSM_START_CYCLE, 
          SSM_BETWEEN_SPILLS, 
          SSM_RAMP, 
          SSM_FAULT
    } ) );

  // Initialize firmware
  _ssmDev->initialize();

  cout << " done." << endl;
}

ssm_state_t SpillStateMachine::stateGet() const
{
  if( _ssmDev.get() == nullptr )
    {
      throw SSM_NOT_READY;
    }

  return _ssmDev->stateGet();
}
