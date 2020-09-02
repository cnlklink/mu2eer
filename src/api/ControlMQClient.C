/**
 * ControlMQClient.C
 *
 * This file contains the implementation of the ControlMQClient class.
 *
 * @author jdiamond
 */

#include "ControlMQClient.H"
#include "errors.H"

using namespace Mu2eER;
using namespace std;

ControlMQClient::ControlMQClient( string mqName )
  : _mqName( mqName )
{
  if( -1 == (_mq = mq_open( _mqName.c_str(), O_WRONLY )) )
    {
      throw API_CMQ_OPENFAIL;
    }
}

ControlMQClient::~ControlMQClient()
{
  mq_close( _mq );
}

void ControlMQClient::fault()
{
  control_msg_t msg;
  msg.command = COMMAND_FAULT;
  _send( msg );
}

void ControlMQClient::reset()
{
  control_msg_t msg;
  msg.command = COMMAND_RESET;
  _send( msg );
}

void ControlMQClient::shutdown()
{
  control_msg_t msg;
  msg.command = COMMAND_SHUTDOWN;
  _send( msg );
}

void ControlMQClient::start()
{
  control_msg_t msg;
  msg.command = COMMAND_START;
  _send( msg );
}

void ControlMQClient::_send( control_msg_t& msg )
{
  if( -1 == mq_send( _mq, (char*)&msg, sizeof( msg ), 0 ) )
    {
      throw API_CMQ_SENDFAIL;
    }
}

void ControlMQClient::ssmInit()
{
  control_msg_t msg;
  msg.command = COMMAND_SSM_INIT;
  _send( msg );
}

void ControlMQClient::testBadCommand()
{
  control_msg_t msg;
  msg.command = COMMAND_INVALID;
  _send( msg );
}
