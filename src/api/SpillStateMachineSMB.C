/**
 * SpillStateMachineSMB.C
 *
 * This file contains the implementation of the SpillStateMachineSMB class.
 *
 * @author jdiamond and rtadkins
 */

#include <iostream>

#include "SpillStateMachineSMB.H"

using namespace Mu2eER;
using namespace std;

SpillStateMachineSMB::SpillStateMachineSMB()
  : _currentState( SSM_UNKNOWN )
{
}

void SpillStateMachineSMB::initialize()
{
  cout << "Initializing Spill State Machine Shared Memory...";
  cout << " done." << endl;
}

void SpillStateMachineSMB::currentStateSet( ssm_state_t state )
{
  _currentState = state;
}

ssm_state_t SpillStateMachineSMB::currentStateGet() const
{
  return _currentState;
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

const int* SpillStateMachineSMB::dataGet() const
{
   return _data;
}

int SpillStateMachineSMB::dataSizeGet() const
{
  return _dataSize;
}

void SpillStateMachineSMB::idealSpillWaveform()
{
  int i = 0, j = 0;
  for (i = 15999; i >= 0; i--)
  {
      _data[j++] = i;
  }
}
