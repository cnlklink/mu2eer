/**
 * main.C
 *
 * This file contains the entry point for the mu2eerd daemon.
 *
 * @author jdiamond
 */

#include <iostream>

#include "Controller.H"

using namespace Mu2eER;
using namespace std;

int main( int argc, char* argv[] )
{
  ConfigurationManager cm;
  Controller ctlr( cm, "/mu2eer", "mu2eer" );

  ctlr.start();

  cout << "mu2eerd started." << endl;
  return 0;
}
