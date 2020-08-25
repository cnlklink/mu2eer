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
  arr.addLinearData();
  std::ofstream fileToCreate("shared_memory.csv");
  fileToCreate << "Shared Memory Data" << endl << endl;
  for (i = 0; i < size; i++) {
    cout << "entry " << i << ": " << arr[i] << endl;
    fileToCreate << "entry " << i << ":" << arr[i] << endl;
  }

  fileToCreate.close();
}
