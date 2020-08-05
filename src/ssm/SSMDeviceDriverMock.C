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

void SSMDeviceDriverMock::configure( const SSMConfig& config )
{
}

void SSMDeviceDriverMock::initialize()
{
  _spillCount = 0;

  _resetSequence();

  _stateNext();
}

void SSMDeviceDriverMock::loadSpillSequence( unsigned int spills )
{
  vector<ssm_state_t> sequence( { SSM_IDLE, SSM_INIT, SSM_BETWEEN_CYCLES } );

  // Load spills
  for( unsigned int i = 0; i != spills; i++ )
    {
      sequence.push_back( SSM_READ_IBEAM );
      sequence.push_back( SSM_RAMP );
      sequence.push_back( SSM_SPILL );
      sequence.push_back( SSM_AFTER_SPILL );
      sequence.push_back( SSM_END_CYCLE );
      sequence.push_back( SSM_LEARNING );
      sequence.push_back( SSM_BETWEEN_CYCLES );
    }

  stateSequenceSet( sequence );
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
  : _spillCount( 0 ),
    _state( SSM_IDLE )
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

unsigned int SSMDeviceDriverMock::spillCounterGet() const
{
  return _spillCount;
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
  auto ret = _stateNext();
  
  // Increment spill counter whenever the SSM_SPILL state is returned
  if( SSM_SPILL == ret )
    {
      ++_spillCount;
    }

  return ret;
}
