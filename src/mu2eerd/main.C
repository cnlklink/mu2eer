/**
 * main.C
 *
 * This file contains the entry point for the mu2eerd daemon.
 *
 * @author jdiamond
 */

#include <iostream>

#include "SharedMemoryManager.H"
#include "SpillStateMachine.H"

using namespace Mu2eER;
using namespace std;

int main( int argc, char* argv[] )
{
  ConfigurationManager cm;

  SharedMemoryManager shmm( "mu2eer" );

  SpillStateMachine ssm( cm, shmm.ssmBlockGet() );
  ssm.initialize();

  cout << "mu2eerd started." << endl;
  return 0;
}
