/**
 * main.C
 *
 * This file contains the entry point for the mu2eerd daemon.
 *
 * @author jdiamond
 */

#include <iostream>

#include "SpillStateMachine.H"

using namespace Mu2eER;
using namespace std;

int main( int argc, char* argv[] )
{
  SpillStateMachine ssm;
  ssm.initialize();

  cout << "mu2eerd started." << endl;
  return 0;
}
