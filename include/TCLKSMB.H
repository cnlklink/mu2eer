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
     */
    string driverNameGet() const;

    /**
     * Set TCLK Decoder Driver Name
     */
    void driverNameSet( string driverName );

  private:
    /**
     * TCLK decoder driver name
     */
    char _driverName[64];
  };
};

#endif
