/**
 * tclk.H
 *
 * This file contains the public interface to the TCLK module.
 *
 * @author jdiamond
 */

#ifndef TCLK_H
#define TCLK_H

#include <memory>
#include <string>

#include "ITCLKDecoderDriver.H"

using namespace std;

namespace Mu2eER
{
  /**
   * TCLKDecoderFactory
   *
   * @param driverName The name of a driver to instantiate a TCLK decoder from
   * @param tclkBlock Shared memory block that the TCLK decoder will write status to
   *
   * @return TCLK decoder
   */
  unique_ptr<ITCLKDecoderDriver> TCLKDecoderFactory( string driverName, TCLKSMB& tclkBlock );
}

#endif
