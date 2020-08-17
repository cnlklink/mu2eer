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

 SpillStateMachineSMB::SpillStateMachineSMB()
 {
 }

 int* SpillStateMachineSMB::dataGet() const
 {
    return _data;
 }

 int SpillStateMachineSMB::dataSizeGet() const
 {
   return _dataSize;
 }


 void SpillStateMachineSMB::initialize()
 {
   cout << "Initializing Spilll State Machine Shared Memory...";
   cout << " done." << endl;
 }

 void SpillStateMachineSMB::addLinearData()
 {
   int i = 0, j = 0;
   for (i = 15999; i >= 0; i--)
   {
     _data[j++] = i;
   }
 }
