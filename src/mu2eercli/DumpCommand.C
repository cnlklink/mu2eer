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
  int size = _shmc.ssmBlockGet().idealSpillWaveFormSizeGet();
  auto ideal = _shmc.ssmBlockGet().idealSpillWaveFormGet();
  auto actual = _shmc.ssmBlockGet().actualSpillWaveFormGet();
  auto error = _shmc.ssmBlockGet().errorSignalWaveFormGet();

  std::ofstream fileToCreate("spill_data.csv");
  fileToCreate << "Entry, Ideal Spill Data, Actual Spill Data, Error Signal Data" << endl << endl;
  for (i = 0; i < size; i++) {
    cout << i << ", " << ideal[i] << ", " << actual[i] << ", " << error[i] << endl;
    fileToCreate << i << ", " << ideal[i] << ", " << actual[i] << ", " << error[i] << endl;
  }

  fileToCreate.close();
}
