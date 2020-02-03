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

void SSMDeviceDriverMock::initialize()
{
  _resetSequence();

  _stateNext();
}

void SSMDeviceDriverMock::_resetSequence()
{
  _stateSequence.clear();
  for( auto state : _initialSequence )
    {
      _stateSequence.push_back( state );
    }
}

SSMDeviceDriverMock::SSMDeviceDriverMock()
  : _state( SSM_IDLE )
{
}

SSMDeviceDriverMock::SSMDeviceDriverMock( const vector<ssm_state_t>& sequence )
  : SSMDeviceDriverMock()
{
  stateSequenceSet( sequence );
}

SSMDeviceDriverMock::~SSMDeviceDriverMock()
{
}

ssm_state_t SSMDeviceDriverMock::stateGet() const
{
  return _state;
}

ssm_state_t SSMDeviceDriverMock::_stateNext()
{
  if( _stateSequence.empty() )
    {
      _state = SSM_FAULT;
    }
  else
    {
      _state = _stateSequence[0];
      _stateSequence.pop_front();
    }

  return _state;
}

void SSMDeviceDriverMock::stateSequenceSet( const vector<ssm_state_t>& sequence )
{
  _initialSequence = sequence;
  _resetSequence();
}

ssm_state_t SSMDeviceDriverMock::waitForStateChange()
{
  return _stateNext();
}
