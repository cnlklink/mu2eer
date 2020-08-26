/**
 * SharedMemoryClient.C
 *
 * This file contains the implementation of the SharedMemoryClient class.
 *
 * @author jdiamond
 */

#include <string>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "SharedMemoryClient.H"

using namespace Mu2eER;
using namespace std;

/**
 * SHM open failure
 */
api_error API_SHMC_OPENFAIL( "failed to open shared memory segment" );

/**
 * SHM mmap failure
 */
api_error API_SHMC_MAPFAIL( "failed to mmap the shared memory segment" );

/**
 * API version mismatch
 */
api_error API_SHMC_BADVERSION( "shared memory version mismatch" );

string SharedMemoryClient::configFileGet() const
{
  return _shmi->configFileGet();
}

mu2eerd_state_t SharedMemoryClient::currentStateGet() const
{
  return _shmi->currentStateGet();
}

pid_t SharedMemoryClient::pidGet() const
{
  return _shmi->pidGet();
}

SharedMemoryClient::SharedMemoryClient( const string& name )
  : _name( name )
{
  // Create shared memory region
  if( -1 == (_fd = shm_open( _name.c_str(), O_RDWR, 0 )) )
    {
      throw API_SHMC_OPENFAIL;
    }

  // Map into our address space
  _size = sizeof( SharedMemoryInterface );
  if( MAP_FAILED == (_ptr = mmap( 0, _size, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0 )) )
    {
      throw API_SHMC_MAPFAIL;
    }

  _shmi = static_cast<SharedMemoryInterface*>( _ptr );

  // Verify the versions agree
  if( 0 != versionGet().compare( API_VERSION ) )
    {
      throw API_SHMC_BADVERSION;
    }
}

SharedMemoryClient::~SharedMemoryClient()
{
  // Unmap shared memory
  if( _ptr != MAP_FAILED )
    munmap( _ptr, _size );

  // Close shared memory file
  close( _fd );
}

const SpillStateMachineSMB& SharedMemoryClient::ssmBlockGet() const
{
  return _shmi->ssmBlockGet();
}

time_t SharedMemoryClient::startTimeGet() const
{
  return _shmi->startTimeGet();
}

string SharedMemoryClient::versionGet() const
{
  return _shmi->versionGet();
}

void SharedMemoryClient::waitForState( mu2eerd_state_t waitForState, 
                                       unsigned int interval, 
                                       unsigned int tries ) const
{
  for( unsigned int i = 0; tries == 0 || i < tries; i++ )
    {
      if( currentStateGet() == waitForState )
        {
          break;
        }
      this_thread::sleep_for( chrono::milliseconds( interval ) );
    }
}

void SharedMemoryClient::waitForSSMState( ssm_state_t waitForState, 
                                          unsigned int interval, 
                                          unsigned int tries ) const
{
  auto& ssm = ssmBlockGet();

  for( unsigned int i = 0; tries == 0 || i < tries; i++ )
    {
      if( ssm.currentStateGet() == waitForState )
        {
          break;
        }
      this_thread::sleep_for( chrono::milliseconds( interval ) );
    }
}
