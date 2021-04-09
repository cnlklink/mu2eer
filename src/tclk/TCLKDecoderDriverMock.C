/**
 * TCLKDecoderDriverMock.C
 *
 * This file contains the implementation of the TCLKDecoderDriverMock class.
 *
 * @author jdiamond
 */

#include <algorithm>
#include <stdexcept>

#include "TCLKDecoderDriverMock.H"

using namespace Mu2eER;
using namespace std;

TCLKDecoderDriverMock::TCLKDecoderDriverMock()
{
}

TCLKDecoderDriverMock::~TCLKDecoderDriverMock()
{
  // Do nothing
}

void TCLKDecoderDriverMock::eventListAdd( ITCLKDecoderDriver::tclk_event_t event )
{
  if( _eventList.size() == EVENT_LIST_MAX )
    {
      throw runtime_error( "TCLK Event List is full" );
    }

  if( _eventList.end() != find( _eventList.begin(), _eventList.end(), event ) )
    {
      // Silently ignore requests to listen for duplicate events
      return;
    }

  _eventList.push_back( event );
}

vector<ITCLKDecoderDriver::tclk_event_t> TCLKDecoderDriverMock::eventListGet() const
{
  return _eventList;
}

unsigned int TCLKDecoderDriverMock::eventListMaxGet() const
{
  return EVENT_LIST_MAX;
}

void 
TCLKDecoderDriverMock::eventSequenceSet( const vector<ITCLKDecoderDriver::tclk_event_t>& sequence )
{
  _eventSequence.clear();

  for( auto event : sequence )
    {
      _eventSequence.push_back( event );
    }
}

string TCLKDecoderDriverMock::nameGet() const
{
  return "mock";
}

ITCLKDecoderDriver::tclk_event_t TCLKDecoderDriverMock::waitForEvents()
{
  if( _eventSequence.empty() )
    {
      return ITCLKDecoderDriver::TCLK_NEVER_EVENT;
    }
    
  auto ret = _eventSequence[0];
  _eventSequence.pop_front();

  return ret;
}
