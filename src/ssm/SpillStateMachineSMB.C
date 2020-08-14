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
 using namespace std;

 SpillStateMachineSMB::SpillStateMachineSMB( ssm_state_t _currentState, std::vector<unsigned int> _dataVector )
  : _currentState( SSM_IDLE ),
    _dataVector( 16000, 0 )
 {
 }

 void SpillStateMachineSMB::initialize()
 {
   cout << "Initializing Spilll State Machine Shared Memory...";

   std::vector<int>(16000, 0);

   cout << " done." << endl;
 }

 void currentStateSet( ssm_state_t state )
 {
   _currentState = state;
 }

 ssm_state_t SpillStateMachineSMB::currentStateGet() const
 {
   return _currentState;
 }

 std::vector<unsigned int> SpillStateMachineSMB::currentStateDataGet() const
 {
    return _dataVector;
 }
