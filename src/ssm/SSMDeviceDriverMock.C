/**
 * SSMDeviceDriverMock.C
 *
 * This file contains the implementation of the SSMDeviceDriverMock class.
 *
 * @author jdiamond
 */

#include "SSMDeviceDriverMock.H"

using namespace Mu2eER;
using namespace std;

SSMDeviceDriverMock::SSMDeviceDriverMock()
  : _state( ISSMDeviceDriver::STATE_IDLE )
{
}

SSMDeviceDriverMock::~SSMDeviceDriverMock()
{
}

ISSMDeviceDriver::state_t SSMDeviceDriverMock::stateGet() const
{
  return _state;
}

void SSMDeviceDriverMock::stateSequenceSet( const vector<ISSMDeviceDriver::state_t>& sequence )
{
  _stateSequence.clear();

  for( auto state : sequence )
    {
      _stateSequence.push_back( state );
    }
}

ISSMDeviceDriver::state_t SSMDeviceDriverMock::waitForStateChange()
{
  if( _stateSequence.empty() )
    {
      _state = ISSMDeviceDriver::STATE_FAULT;
    }
  else
    {
      _state = _stateSequence[0];
      _stateSequence.pop_front();
    }

  return _state;
}
