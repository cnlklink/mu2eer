/**
 * MulticastTCLKDecoderDriver.C
 *
 * This file contains the implementation of the MulticastTCLKDecoderDriver.
 *
 * @author jdiamond
 */

#include "MulticastTCLKDecoderDriver.H"

using namespace std;
using namespace Mu2eER;

MulticastTCLKDecoderDriver::MulticastTCLKDecoderDriver()
{
  mcastInit( MCAST_THREAD_PRIORITY );
}

MulticastTCLKDecoderDriver::~MulticastTCLKDecoderDriver()
{
}
