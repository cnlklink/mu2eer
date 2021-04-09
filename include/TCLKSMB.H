/**
 * TCLKSMB.H
 *
 * This file contains the definition of the TCLKSMB class.
 *
 * @author jdiamond
 */

#ifndef TCLKSMB_H
#define TCLKSMB_H

#include <string>

using namespace std;

namespace Mu2eER
{
  /**
   * TCLKSMB
   *
   * Shared memory block for the TCLK decoder.
   */
  class TCLKSMB
  {
  public:
    /**
     * Constructor
     */
    TCLKSMB();

    /**
     * Get TCLK Decoder Drive Name
     *
     * @return TCLK decoder driver name
     */
    string driverNameGet() const;

    /**
     * Set TCLK Decoder Driver Name
     *
     * @param driverName TCLK decoder driver name
     */
    void driverNameSet( string driverName );

    /**
     * Get Event Counter
     *
     * @param event TCLK event
     * @return A count for the given TCLK event
     */
    unsigned int eventCounterGet( uint8_t event ) const;

  private:
    /**
     * TCLK decoder driver name
     */
    char _driverName[64];

    /**
     * TCLK event counter
     */
    unsigned int _eventCounter[256];

    /**
     * Reset Event Counters
     */
    void _resetEventCounters();
  };
};

#endif
