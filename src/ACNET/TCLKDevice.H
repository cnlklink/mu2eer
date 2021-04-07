/**
 * TCLKDevice.H
 *
 * This file contains the definition of the TCLKDevice class.
 *
 * @author jdiamond
 */

#ifndef TCLKDEVICE_H
#define TCLKDEVICE_H

#include <string>

#include "device.h"

#include "SharedMemoryClient.H"

using namespace FFF;
using namespace std;

namespace Mu2eER
{
  /**
   * TCLKDevice
   *
   * Implements an ACSys/cdev device for reporting status and interacting with the TCLK decoder.
   */
  class TCLKDevice : public Device<32>
  {
  public:
    /**
     * Reading property data type
     */
    typedef uint32_t event_counter_read_t;

    /**
     * Event Counter / Reading Property
     *
     * @param dest ACNET response buffer
     * @param reqinfo ACNET request object
     */
    void eventCounterRead( Array<event_counter_read_t>& dest, ReqInfo const* reqinfo );

    /**
     * Constructor
     *
     * @param mqName mu2eerd control message queue name
     * @param shmName mu2eerd shared memory region name
     */
    TCLKDevice( string mqName, string shmName );

  private:
    /**
     * Name of the mu2eerd control message queue
     */
    string _mqName;

    /**
     * Name of the mu2eerd shared memory region
     */
    string _shmName;

  };
};

#endif
