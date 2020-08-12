/**
 * SpillStateMachineSMB.C
 *
 * This file contains the definition of the SpillStateMachineSMB class.
 *
 * @author rtadkins
 */

 #ifndef SPILLSTATEMACHINE_H
 #define SPILLSTATEMACHINE_H

 #include "SpillStateMachineSMB.H"

 using namespace Mu2eER;

 SpillStateMachineSMB::SpillStateMachineSMB()
 {
 }

 void SpillStateMachineSMB::initialize()
 {
   cout << "Initializing Spilll State Machine Shared Memory...";

   std::vector<int>(16000, 0);

   cout << " done." << endl;
 }

 ssm_state_t SpillStateMachine::stateGet() const
 {
   return _currentState;
 }

 std::vector<unsigned int> currentStateDataGet()
 {
    return _dataVector;
 }
