/**
 * SharedMemoryManager.C
 *
 * This file contains the implementation of the SharedMemoryManager class.
 *
 * @author jdiamond
 */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "SharedMemoryInterface.H"
#include "SharedMemoryManager.H"

using namespace Mu2eER;
using namespace std;

/**
 * Shared memory permission denied
 */
api_error API_SHM_PERM( "Permission denied when creating shared memory region (EACCES)" );

/**
 * Shared memory segment already exists
 */
api_error API_SHM_EXISTS( "shared memory region already exists (EEXIST)" );

/**
 * Invalid shared memory name
 */
api_error API_SHM_BADNAME( "invalid name for shared memory region (EINVAL or ENAMETOOLONG)" );

/**
 * Not enough memory
 */
api_error API_SHM_OUTOFMEM( "not enough memory for shared memory region (ENOSPC)" );

/**
 * Unknown shared memory error
 */
api_error API_SHM_GENERIC( "failed to create shared memory segment" );

/**
 * Resize failure
 */
api_error API_SHM_TRUNCFAIL( "failed to set the appropriate size for the shared memory segment" );

/**
 * Mapping failure
 */
api_error API_SHM_MAPFAIL( "failed to mmap the shared memory segment" );

/**
 * Lock failure
 */
api_error API_SHM_LOCKFAIL( "failed to lock the shared memory segment" );

void SharedMemoryManager::currentStateSet( mu2eerd_state_t state )
{
  _shmPtr->currentStateSet( state );
}

void SharedMemoryManager::pidSet( pid_t pid )
{
  _shmPtr->pidSet( pid );
}

SharedMemoryManager::SharedMemoryManager( const string& name )
  : _name( name ),
    _ptr( nullptr ),
    _size( sizeof( SharedMemoryInterface ) )
{
  // Create shared memory region
  if( -1 == (_fd = shm_open( _name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR )) )
    {
      switch( errno )
	{
	case EACCES:
          throw API_SHM_PERM;
          break;

	case EEXIST:
          throw API_SHM_EXISTS;
	  break;

	case EINVAL:
	case ENAMETOOLONG:
          throw API_SHM_BADNAME;
	  break;

	case ENOSPC:
          throw API_SHM_OUTOFMEM;
	  break;

	default:
          throw API_SHM_GENERIC;
	  break;
	}
    }  

  // Resize accordingly
  if( -1 == ftruncate( _fd, _size ) )
    {
      throw API_SHM_TRUNCFAIL;
    }

  // Map into our address space
  if( MAP_FAILED == (_ptr = mmap( 0, _size, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0 )) )
    {
      throw API_SHM_MAPFAIL;
    }

  // Lock the shared memory into physical address space (necessary for transfering DAQ data via DMA)
  if( mlock( _ptr, _size ) )
    {
      throw API_SHM_LOCKFAIL;
    }

  // Initialize
  _shmPtr = new (_ptr) SharedMemoryInterface();
}

SharedMemoryManager::~SharedMemoryManager()
{
  if( _ptr != nullptr )
    {
      // Un-map the shared memory region
      munmap( _ptr, _size );

      // Delete the shared memory region
      shm_unlink( _name.c_str() );
    }
}

SpillStateMachineSMB& SharedMemoryManager::ssmBlockGet()
{
  return _shmPtr->ssmBlockGet();
}

string SharedMemoryManager::versionGet() const
{
  return _shmPtr->versionGet();
}
