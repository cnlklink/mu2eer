/**
 * SSMDeviceDriverMock.C
 *
 * This file contains the implementation of the SSMDeviceDriverMock class.
 *
 * @author jdiamond and rtadkins
 */

#include <chrono>
#include <thread>

#include "SSMDeviceDriverMock.H"

using namespace Mu2eER;
using namespace std;

void SSMDeviceDriverMock::configure( const SSMConfig& config )
{
  if( config.mockSpillsGet() > 0 )
    {
      loadSpillSequence( config.mockSpillsGet() );
    }

  delaySet( config.mockDelayGet() );
}

void SSMDeviceDriverMock::delaySet( unsigned int delay )
{
  _delay = delay;
}

void SSMDeviceDriverMock::fault()
{
  _state = SSM_FAULT;
  _stateSequence.clear();
}

void SSMDeviceDriverMock::initialize()
{
  // Reset spill counter
  _spillCount = 0;

  // Reset the time-in-spill register
  _timeInSpill = 0;

  // Reset the state sequence
  _resetSequence();

  // Prime with the first state in the sequence
  _stateNext();
}

void SSMDeviceDriverMock::loadSpillSequence( unsigned int spills )
{
  vector<ssm_state_t> sequence;

  // Load spills
  for( unsigned int i = 0; i != spills; i++ )
    {
      sequence.push_back( SSM_BETWEEN_CYCLES );
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

void SSMDeviceDriverMock::reset()
{
  _delay = 0;
  _spillCount = 0;
  _state = SSM_IDLE;
  _timeInSpill = 0;
  _resetSequence();
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
{
  reset();
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

unsigned int SSMDeviceDriverMock::timeInSpillGet() const
{
  return _timeInSpill;
}

ssm_state_t SSMDeviceDriverMock::waitForStateChange()
{
  this_thread::sleep_for( chrono::milliseconds( _delay ) );

  auto ret = _stateNext();

  // Increment spill counter whenever the SSM_SPILL state is returned
  if( SSM_SPILL == ret )
    {
      ++_spillCount;
    }

  // Set the time-in-spill to 107ms whenever the SSM_AFTER_SPILL state is returned
  if( SSM_AFTER_SPILL == ret )
    {
      _timeInSpill = 107;
    }

  return ret;
}

bool SSMDeviceDriverMock::getLedState()
{
  return _led;
}
