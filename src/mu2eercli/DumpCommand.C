/**
 * DumpCommand.C
 *
 * This file contains the implementation of the DumpCommand class.
 *
 * @author rtadkins
 */

 #include <iostream>
 #include <fstream>

 #include "DumpCommand.H"

 using namespace Mu2eER;
 using namespace std;

DumpCommand::DumpCommand( ControlMQClient& mqc, SharedMemoryClient& shmc )
  : Command( mqc, shmc )
{
}

void DumpCommand::run( unsigned int argc, const char* argv[] )
{
  int i = 0;
  int size = _shmc.ssmBlockGet().dataSizeGet();
  const int* arr = _shmc.ssmBlockGet().dataGet();

  std::ofstream fileToCreate("ideal_spill_data.csv");
  fileToCreate << "Entry, Ideal Spill Data" << endl << endl;
  for (i = 0; i < size; i++) {
    cout << i << ": " << arr[i] << endl;
    fileToCreate << i << ", " << arr[i] << endl;
  }

  fileToCreate.close();
}
