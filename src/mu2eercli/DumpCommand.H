/**
 * DumpCommand.H
 *
 * This file contains the definition of the DumpCommand class.
 *
 * @author rtadkins
 */

#ifndef DUMPCOMMAND_H
#define DUMPCOMMAND_H

#include "Command.H"

namespace Mu2eER
{
   /**
    * DumpCommand
    *
    * Implements a command for exporting all data from the shared memory
    * interface into a .csv file.
    */
  class DumpCommand : public Command
  {
  public:
   /**
    * Constructor
    *
    * @param mqc Control message queue client reference
    * @param shmc Shared memory client reference
    */
   DumpCommand( ControlMQClient& mqc, SharedMemoryClient& shmc );

   // Override
   virtual void run( unsigned int argc, const char* argv[] );
 };
};

#endif
