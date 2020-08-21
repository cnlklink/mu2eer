/**
 * CLI.C
 *
 * This file contains the implementation of the CLI class.
 *
 * @author jdiamond
 */

#include "CLI.H"
#include "PIDCommand.H"
#include "ShowCommand.H"
#include "ShutdownCommand.H"
#include "StartCommand.H"

using namespace Mu2eER;
using namespace std;

CLI::CLI( const string cmqName, const string shmcName )
  : _mqc( cmqName ),
    _shmc( shmcName )
{
  _commands["pid"] = unique_ptr<Command>( new PIDCommand( _mqc, _shmc ) );
  _commands["show"] = unique_ptr<Command>( new ShowCommand( _mqc, _shmc ) );
  _commands["shutdown"] = unique_ptr<Command>( new ShutdownCommand( _mqc, _shmc ) );
<<<<<<< HEAD
  _commands["start"] = unique_ptr<Command>( new StartCommand( _mqc, _shmc ) );
=======
  _commands["dump"] = unique_ptr<Command>( new DumpCommand( _mqc, _shmc ) );
>>>>>>> 53a2aa4d24a200a018b4911a8f72f693b49114cf
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
