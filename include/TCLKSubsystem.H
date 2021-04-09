/**
 * TCLKSubsystem.H
 *
 * This file contains the definition of the TCLKSubsystem class.
 *
 * @author jdiamond
 */

#ifndef TCLKSUBSYSTEM_H
#define TCLKSUBSYSTEM_H

#include <memory>

#include "ConfigurationManager.H"
#include "ITCLKDecoderDriver.H"

using namespace std;

namespace Mu2eER
{
  /**
   * TCLKSubsystem
   *
   * Interfaces with TCLK using the configured TCLK decoder.  Receives TCLK events and distributes them to the
   * rest of the system.
   */
  class TCLKSubsystem
  {
  public:
    /**
     * Initialize
     *
     * Initialize the TCLK decoder and prepare to receive events.
     */
    void initialize();

    /**
     * Constructor
     *
     * @param cm Reference to the configuration manager
     * @param smb Reference to the TCLK shared memory block
     */
    TCLKSubsystem( const ConfigurationManager& cm, TCLKSMB& smb );

  private:
    /**
     * TCLK decoder
     */
    unique_ptr<ITCLKDecoderDriver> _tclkDecoder;

    /**
     * TCLK Shared Memory Block
     */
    TCLKSMB& _sharedMemoryBlock;

    /**
     * Set the TCLK Decoder
     *
     * @param driverName The name of the TCLK decoder driver to be used
     */
    void _tclkDecoderSet( string driverName );
  };
};

#endif
