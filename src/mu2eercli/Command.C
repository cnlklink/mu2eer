/**
 * Command.C
 *
 * This file contains the implementation of the Command class.
 *
 * @author jdiamond
 */

#include "Command.H"

using namespace Mu2eER;
using namespace std;

Command::Command( ControlMQClient& mqc, SharedMemoryClient& shmc )
  : _mqc( mqc ),
    _shmc( shmc )
{
}
