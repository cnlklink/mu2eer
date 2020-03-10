/**
 * CLI.C
 *
 * This file contains the implementation of the CLI class.
 *
 * @author jdiamond
 */

#include "CLI.H"
#include "PIDCommand.H"
#include "ShutdownCommand.H"

using namespace Mu2eER;
using namespace std;

CLI::CLI( const string cmqName, const string shmcName )
  : _mqc( cmqName ),
    _shmc( shmcName )
{
  _commands["pid"] = unique_ptr<Command>( new PIDCommand( _mqc, _shmc ) );
  _commands["shutdown"] = unique_ptr<Command>( new ShutdownCommand( _mqc, _shmc ) );
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
