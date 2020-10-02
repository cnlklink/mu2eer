/**
 * DaemonController.C
 *
 * This file contains the implementation of the DaemonController class.
 *
 * @author jdiamond
 */

#include <array>
#include <dirent.h>
#include <fstream>
#include <syslog.h>

#include "DaemonController.H"

using namespace Mu2eER;
using namespace std;

DaemonController::DaemonController( string name, 
                                    string startCmdProd, 
                                    string startCmdTest, 
                                    string stopCmd )
  : _processName( name ),
    _startCmdProd( startCmdProd ),
    _startCmdTest( startCmdTest ),
    _stopCmd( stopCmd )
{
}

DaemonController::DaemonController( string name, string startCmd, string stopCmd )
  : DaemonController( name, startCmd, startCmd, stopCmd )
{
}

void DaemonController::_exec( string cmd ) const
{
  // From: https://stackoverflow.com/questions/52164723/how-to-execute-a-command-and-get-return-code-stdout-and-stderr-of-command-in-c

  array<char, 128> buffer;
  string result;

  // Open pipe, execute cmd
  auto pipe = popen( cmd.c_str(), "r" );
  if( !pipe )
    {
      throw runtime_error( "failed to exec daemon command, popen failure" );
    }

  // Wait to receive output from `cmd`
  while( !feof( pipe ) )
    {
      if( fgets( buffer.data(), 128, pipe ) != nullptr )
        {
          result += buffer.data();
        }
    }

  // Wait for `cmd` to finish, check return code for success
  auto rc = pclose( pipe );
  if( rc != EXIT_SUCCESS )
    {
      syslog( LOG_ERR, "  command was - %s", cmd.c_str() );
      syslog( LOG_ERR, "  output was - %s", result.c_str() );
      throw runtime_error( "failed to exec daemon command, command failure" );
    }
}

int DaemonController::_getProcIdByName( string procName ) const
{
  // From: https://stackoverflow.com/questions/45037193/how-to-check-if-a-process-is-running-in-c

  int pid = -1;
  
  // Open the /proc directory
  DIR *dp = opendir("/proc");
  if (dp != NULL)
    {
      // Enumerate all entries in directory until process found
      struct dirent *dirp;
      while (pid < 0 && (dirp = readdir(dp)))
        {
          // Skip non-numeric entries
          int id = atoi(dirp->d_name);
          if (id > 0)
            {
              // Read contents of virtual /proc/{pid}/cmdline file
              string cmdPath = string("/proc/") + dirp->d_name + "/cmdline";
              ifstream cmdFile(cmdPath.c_str());
              string cmdLine;
              getline(cmdFile, cmdLine);
              if (!cmdLine.empty())
                {
                  // Keep first cmdline item which contains the program path
                  size_t pos = cmdLine.find('\0');
                  if (pos != string::npos)
                    cmdLine = cmdLine.substr(0, pos);
                  // Keep program name only, removing the path
                  pos = cmdLine.rfind('/');
                  if (pos != string::npos)
                    cmdLine = cmdLine.substr(pos + 1);
                  // Compare against requested process name
                  if (procName == cmdLine)
                    pid = id;
                }
            }
        }
    }
  
  closedir(dp);
  
  return pid;
}

bool DaemonController::isRunning() const
{
  return -1 != _getProcIdByName( _processName );
}

void DaemonController::start( DaemonController::config_t config ) const
{
  switch( config )
    {
    case CONFIG_PRODUCTION: 
      _exec( _startCmdProd ); 
      return;

    case CONFIG_TEST: 
      _exec( _startCmdTest ); 
      return;

    default: 
      throw runtime_error( "bad configuration option" );
    }
}

void DaemonController::stop() const
{
  _exec( _stopCmd );
}
