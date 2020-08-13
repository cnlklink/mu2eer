/**
 * SpillStateMachineSMB.C
 *
 * This file contains the definition of the SpillStateMachineSMB class.
 *
 * @author rtadkins
 */

 #include <iostream>

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

 ssm_state_t SpillStateMachineSMB::stateGet()
 {
   return _currentState;
 }

 std::vector<unsigned int> SpillStateMachineSMB::currentStateDataGet()
 {
    return _dataVector;
 }
