/**
 * SharedMemoryManager.H
 *
 * This file contains the definition of the SharedMemoryManager class.
 *
 * @author jdiamond
 */

#ifndef SHAREDMEMORYMANAGER_H
#define SHAREDMEMORYMANAGER_H

#include <string>

#include "api.H"
#include "SharedMemoryInterface.H"

using namespace std;

namespace Mu2eER
{
  /**
   * SharedMemoryManager
   *
   * Manages the shared memory segment for the shared memory API.  Responsible for constructing
   * and destructing the POSIX shared memory segment.
   */
  class SharedMemoryManager
  {
  public:
    /**
     * Set Config File Path
     *
     * @param cfgFilePath Path to the active configuration file
     */
    void configFileSet( string cfgFilePath );

    /**
     * Set Current State
     *
     * @param state State of mu2eerd daemon
     */
    void currentStateSet( mu2eerd_state_t state );

    /**
     * Set PID
     *
     * @param pid PID of mu2eerd daemon
     */
    void pidSet( pid_t pid );

    /**
     * Constructor
     *
     * @param name A name for the shared memory segment
     */
    SharedMemoryManager( const string& name );

    /**
     * Destructor
     */
    ~SharedMemoryManager();

    /**
     * Get SSM Block
     *
     * @return Reference to the Spill State Machine block.
     */
    SpillStateMachineSMB& ssmBlockGet();

    /**
     * Set Start Time
     *
     * Sets the start time to right now.
     */
    void startTimeSetToNow();

    /**
     * Get TCLK Block
     *
     * @return Reference to the TCLK block.
     */
    TCLKSMB& tclkBlockGet();

    /**
     * Get Version
     *
     * @return Shared memory API version
     */
    string versionGet() const;

  private:
    /**
     * Shared memory file descriptor
     */
    int _fd;

    /**
     * Name of the shared memory segment
     */
    string _name;

    /**
     * Pointer to the shared memory segment
     */
    void* _ptr;

    /**
     * Pointer to the shared memory interface allocated in the shared memory segment
     */
    SharedMemoryInterface* _shmPtr;

    /**
     * Size of the shared memory segment
     */
    ssize_t _size;
  };
};

#endif
