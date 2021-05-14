/**
 * MulticastTCLKDecoderDriver.C
 *
 * This file contains the implementation of the MulticastTCLKDecoderDriver.
 *
 * @author jdiamond
 */

#include <chrono>
#include <syslog.h>
#include <thread>

#include "MulticastTCLKDecoderDriver.H"

using namespace std;
using namespace Mu2eER;

void MulticastTCLKDecoderDriver::eventListAdd( ITCLKDecoderDriver::tclk_event_t event )
{
  syslog( LOG_INFO, "adding TCLK event: %02x", event );
}

vector<ITCLKDecoderDriver::tclk_event_t> MulticastTCLKDecoderDriver::eventListGet() const
{
  vector<ITCLKDecoderDriver::tclk_event_t> ret;

  return ret;
}

unsigned int MulticastTCLKDecoderDriver::eventListMaxGet() const
{
  return 256;
}

MulticastTCLKDecoderDriver::MulticastTCLKDecoderDriver()
{
  mcastInit( MCAST_THREAD_PRIORITY );
}

MulticastTCLKDecoderDriver::~MulticastTCLKDecoderDriver()
{
}

string MulticastTCLKDecoderDriver::nameGet() const
{
  return "multicast";
}

ITCLKDecoderDriver::tclk_event_t MulticastTCLKDecoderDriver::waitForEvents()
{
  this_thread::sleep_for( chrono::seconds( 1 ) );

  return 0xfe;
}
