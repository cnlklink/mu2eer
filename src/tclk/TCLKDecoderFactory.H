/**
 * TCLKDecoderFactory.H
 *
 * This file contains the definition of the TCLKDecoderFactory.
 *
 * @author jdiamond
 */

#ifndef TCLKDECODERFACTORY_H
#define TCLKDECODERFACTORY_H

#include <memory>
#include <string>

#include "ITCLKDecoderDriver.H"

using namespace std;
using namespace Mu2eER;

namespace Mu2eER
{
  /**
   * TCLKDecoderFactory
   *
   * @param driverName The name of a driver to instantiate a TCLK decoder from
   *
   * @return TCLK decoder
   */
  unique_ptr<ITCLKDecoderDriver> TCLKDecoderFactory( string driverName );
}

#endif
