/**
 * SpillStateMachineSMB.C
 *
 * This file contains the implementation of the SpillStateMachineSMB class.
 *
 * @author jdiamond and rtadkins
 */

#include <iostream>
#include <cmath>

#include "SpillStateMachineSMB.H"

using namespace Mu2eER;
using namespace std;

SpillStateMachineSMB::SpillStateMachineSMB()
  : _currentState( SSM_UNKNOWN )
{
  initialize();
}

void SpillStateMachineSMB::initialize()
{
  idealSpillWaveform();
  actualSpillWaveform();
  errorSignalWaveform();
}

CircularBuffer<int16_t> SpillStateMachineSMB::circularBufferGet() const
{
  return _circular_buffer;
}

void SpillStateMachineSMB::currentStateSet( ssm_state_t state )
{
  _currentState = state;
}

ssm_state_t SpillStateMachineSMB::currentStateGet() const
{
  return _currentState;
}

bool SpillStateMachineSMB::ledStateGet() const
{
  return _led;
}

void SpillStateMachineSMB::ledStateSet( bool ledState )
{
  _led = ledState;
}

unsigned int SpillStateMachineSMB::spillCounterGet() const
{
  return _spillCounter;
}

void SpillStateMachineSMB::spillCounterSet( unsigned int sc )
{
  _spillCounter = sc;
}

void SpillStateMachineSMB::threadRunningSet( bool running )
{
  _threadRunning = running;
}

bool SpillStateMachineSMB::threadRunningGet() const
{
  return _threadRunning;
}

float SpillStateMachineSMB::timeInSpillGet() const
{
  return _timeInSpill;
}

void SpillStateMachineSMB::timeInSpillSet( float tis )
{
  _timeInSpill = tis;
}

const int* SpillStateMachineSMB::idealSpillWaveFormGet() const
{
   return _idealSpillWaveFormData;
}

int SpillStateMachineSMB::idealSpillWaveFormSizeGet() const
{
  return _idealSpillWaveFormSize;
}

void SpillStateMachineSMB::idealSpillWaveform()
{
  int i = 0, j = 0;
  for (i = 15999; i >= 0; i--)
  {
    _idealSpillWaveFormData[j++] = i;
  }
}

const int* SpillStateMachineSMB::actualSpillWaveFormGet() const
{
   return _actualSpillWaveFormData;
}

int SpillStateMachineSMB::actualSpillWaveFormSizeGet() const
{
  return _idealSpillWaveFormSize;
}

void SpillStateMachineSMB::actualSpillWaveform()
{
  int i = 0, j = 0;

  for ( i = 15999; i >= 0; i-- )
  {
    if ( i % 1000 == 0 && i > 0)
    {
      i-=100;
      _actualSpillWaveFormData[j++] = i;
    } else
    {
      _actualSpillWaveFormData[j++] = i;
    }
  }
}

const int* SpillStateMachineSMB::errorSignalWaveFormGet() const
{
   return _errorSignalWaveFormData;
}

int SpillStateMachineSMB::errorSignalWaveFormSizeGet() const
{
  return _idealSpillWaveFormSize;
}

void SpillStateMachineSMB::errorSignalWaveform()
{
  int i = 0;

  for ( i = 0; i < _idealSpillWaveFormSize; i++ ) {
    _errorSignalWaveFormData[i] = _idealSpillWaveFormData[i] - _actualSpillWaveFormData[i];
  }
}

void SpillStateMachineSMB::fillCircularBuffer()
{
  int i, degrees;
  double x, result;

  degrees = _circular_buffer.capacityGet();

  printf("Show fillCircularBuffer() capacity %d", degrees);

  for ( i = 0; i < 5; i++ )
  {
    x = ( i * 3.14159 * 90 ) / 180;
    printf("Show x in fillCircularBuffer - x: %d", x);

    result = sin(x);
    printf("Show result in fillCircularBuffer - result: %d", result);
    _circular_buffer.enqueue(result);
  }
  for ( i = 0; i < 5; i++ ) {
    printf("index: %d, data: %f\n", i, _circular_buffer.dataGet(i));
  }
}
