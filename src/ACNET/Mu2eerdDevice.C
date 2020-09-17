/**
 * Mu2eerdDevice.C
 *
 * This file contains the implementation of the Mu2eerdDevice class.
 *
 * @author jdiamond
 */

#include <dirent.h>
#include <fstream>
#include <syslog.h>

#include "errors.H"
#include "Mu2eerdDevice.H"

using namespace Mu2eER;

/**
 * Get Process ID By Name
 *
 * Returns the PID for the process with the given name.
 * 
 * From:
 *   https://stackoverflow.com/questions/45037193/how-to-check-if-a-process-is-running-in-c
 *
 * @param procName Process name
 * @return Process ID for procName or -1 if no process found
 */
static int getProcIdByName(string procName)
{
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

Mu2eerdDevice::Mu2eerdDevice( string daemonName, string mqName, string shmName )
  : Device<32>( "Mu2eerdDevice", "mu2eerd Device" ),
    _daemonName( daemonName ),
    _mqName( mqName ),
    _shmName( shmName )
{
  registerMethod( ATTR_DAEMON_READ, 
                  *this, 
                  &Mu2eerdDevice::daemonRead, 
                  DAEMON_READ_MAX );

  registerMethod( ATTR_DAEMON_STATUSCTRL,
                  *this,
                  &Mu2eerdDevice::daemonStatus,
                  DAEMON_STATUSCTRL_MAX );
}

void Mu2eerdDevice::daemonRead( Array<daemon_read_t>& dest, ReqInfo const* reqinfo )
{
  if( dest.offset.getValue() > static_cast<int>( DAEMON_READ_MAX ) )
    {
      throw Ex_BADOFF;
    }

  if( (dest.offset.getValue() + dest.total.getValue() ) >
      static_cast<int>( DAEMON_READ_MAX ) )
    {
      throw Ex_BADOFLEN;
    }

  try
    {
      SharedMemoryClient shmc( _shmName );
      
      for( unsigned int i = 0; i != dest.total.getValue(); i++ )
        {
          switch( dest.offset.getValue() + i )
            {
            case DAEMON_READ_IDX_UPTIME:
              dest[i] = shmc.uptimeGet(); 
              break;
              
            case DAEMON_READ_IDX_PID:
              dest[i] = shmc.pidGet();
              break;
              
            case DAEMON_READ_IDX_JENKINS_NUM:
              dest[i] = _jenkinsNumberGet();
              break;
              
            default:
              throw Ex_BADOFLEN;
              break;
            }
        }
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in Mu2eerdDevice::daemonRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}

void Mu2eerdDevice::daemonStatus( Array<daemon_statusctrl_t>& dest, ReqInfo const* reqinfo )
{
  if( dest.offset.getValue() > static_cast<int>( DAEMON_STATUSCTRL_MAX ) )
    {
      throw Ex_BADOFF;
    }

  if( (dest.offset.getValue() + dest.total.getValue() ) >
      static_cast<int>( DAEMON_STATUSCTRL_MAX ) )
    {
      throw Ex_BADOFLEN;
    }

  try
    {
      dest[0] = 0;

      // Daemon process running/not running status
      dest[0] |= _daemonIsRunning() ? DAEMON_STATUS_RUNNING : 0;
    }
  catch( runtime_error e )
    {
      syslog( LOG_ERR, "runtime_error caught in Mu2eerdDevice::daemonRead(..) - %s", e.what() );
      throw Ex_DEVFAILED;
    }
}

bool Mu2eerdDevice::_daemonIsRunning() const
{
  return -1 != getProcIdByName( _daemonName );
}

unsigned int Mu2eerdDevice::_jenkinsNumberGet() const
{
  fstream jnfile( "/etc/jenkins_build_number", ios_base::in );
  unsigned int num;

  jnfile >> num;

  return num;
}
