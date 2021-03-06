/**
 * CLI.C
 *
 * This file contains the implementation of the CLI class.
 *
 * @author jdiamond
 */

#include "CLI.H"
#include "FaultCommand.H"
#include "PIDCommand.H"
#include "ResetCommand.H"
#include "ShowCommand.H"
#include "ShutdownCommand.H"
#include "StartCommand.H"
#include "DumpCommand.H"

using namespace Mu2eER;
using namespace std;

CLI::CLI( const string cmqName, const string shmcName )
  : _mqc( cmqName ),
    _shmc( shmcName )
{
  _commands["fault"] = unique_ptr<Command>( new FaultCommand( _mqc, _shmc ) );
  _commands["pid"] = unique_ptr<Command>( new PIDCommand( _mqc, _shmc ) );
  _commands["reset"] = unique_ptr<Command>( new ResetCommand( _mqc, _shmc ) );
  _commands["show"] = unique_ptr<Command>( new ShowCommand( _mqc, _shmc ) );
  _commands["shutdown"] = unique_ptr<Command>( new ShutdownCommand( _mqc, _shmc ) );
  _commands["start"] = unique_ptr<Command>( new StartCommand( _mqc, _shmc ) );
  _commands["dump"] = unique_ptr<Command>( new DumpCommand( _mqc, _shmc ) );
}

void CLI::run( unsigned int argc, const char* argv[] )
{
  try
    {
      _commands.at( argv[1] )->run( argc, argv );
    }
  catch( out_of_range e )
    {
      throw MU2EERCLI_COMMAND_NOT_FOUND;
    }
}
