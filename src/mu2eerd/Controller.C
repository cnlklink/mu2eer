/**
 * Controller.C
 *
 * This file contains the implementation of the Controller class.
 *
 * @author jdiamond
 */

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>

#include "../tclk/TCLKDecoderDriverMock.H"

#include "Controller.H"

using namespace Mu2eER;
using namespace std;

/**
 * Thrown if mq_receive is interrupted
 */
controller_error CONTROLLER_MQ_INT( "mq_receive was interrupted" );

/**
 * Thrown if mq_recieve fails
 */
controller_error CONTROLLER_MQ_FAILURE( "mq_receive failed" );

/**
 * Thrown if the command is not recognized
 */
controller_error CONTROLLER_INVALID_COMMAND( "command not recognized" );

/**
 * Thrown if the message size is not what is expected
 */
controller_error CONTROLLER_INVALID_MESSAGE( "message is invalid" );

/**
 * Thrown if the control message queue already exists
 */
controller_error CONTROLLER_MQ_EXISTS( "control message queue already exists" );

/**
 * Thrown if there is an invalid parameter to mq_open
 */
controller_error CONTROLLER_MQ_INVPARAM( "invalid parameter to mq_open" );

/**
 * Thrown if there is an unkown error creating the message queue
 */
controller_error CONTROLLER_MQ_OPENFAILURE( "mq_open failure" );

const string Controller::TEST_DAEMON_CMQ_NAME = "/mu2eer_test";

const string Controller::TEST_DAEMON_SHM_NAME = "mu2eer_test";

ConfigurationManager* Controller::_testdCM = nullptr;

Controller* Controller::_testdCtlr = nullptr;

thread* Controller::_testdThread = nullptr;

Controller::Controller( ConfigurationManager& cm, string mqName, string shmName )
  : _cm( cm ),
    _mqName( mqName ),
    _shmm( shmName ),
    _ssm( _cm, _shmm.ssmBlockGet() ),
    _tclkDecoder( TCLKDecoderFactory( "mock", _shmm.tclkBlockGet() ) )
{
  _shmm.configFileSet( _cm.configFileGet() );

  _shmm.currentStateSet( MU2EERD_INITIALIZING );

  _createMQ();
}

Controller::~Controller()
{
  if( _mqId != 0 )
    {
      // Close the command message queue
      mq_unlink( _mqName.c_str() );
      mq_close( _mqId );
    }
}

void Controller::_createMQ()
{
  // Create a POSIX message queue for accepting commands from clients
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = MQ_MAX_MESSAGES;
  attr.mq_msgsize = sizeof( control_msg_t );
  attr.mq_curmsgs = 0;
  if( -1 == (_mqId = mq_open( _mqName.c_str(), O_RDONLY | O_CREAT | O_EXCL, 0660, &attr )) )
    {
      switch( errno )
	{
	case EEXIST:
	  throw CONTROLLER_MQ_EXISTS;

	case EINVAL:
	  throw CONTROLLER_MQ_INVPARAM;

	default:
	  throw CONTROLLER_MQ_OPENFAILURE;
	}
    }

}

void Controller::_handleFault()
{
  _ssm.fault();
}

void Controller::_handleReset()
{
  _ssm.reset();
}

void Controller::_handleShutdown()
{
  // Shutdown tasks...
}

void Controller::_handleSSMInit()
{
  _ssm.initialize();
}

void Controller::_handleStart()
{
  _ssm.run();
}

void Controller::_processMessages()
{
  syslog( LOG_INFO, "mu2eerd running on %s.", ConfigurationManager::hostnameGet().c_str() );

  while( 1 )
    {
      control_msg_t msg;
      ssize_t msgSize;

      try
	{
	  // Wait for messages to arrive
	  if( -1 == (msgSize = mq_receive( _mqId, (char*)&msg, sizeof( msg ), 0 )) )
	    {
	      switch( errno )
		{
		case EINTR:
                  throw CONTROLLER_MQ_INT;
		  return;
		  
		default:
		  throw CONTROLLER_MQ_FAILURE;
		}
	    }

	  // Validate message received
	  if( msgSize != sizeof( msg ) )
	    {
              throw CONTROLLER_INVALID_MESSAGE;
	    }
	  
	  // Handle according to the command
	  switch( msg.command )
	    {
            case COMMAND_FAULT:
              _handleFault();
              break;

	    case COMMAND_SHUTDOWN:
              _handleShutdown();
	      return;

            case COMMAND_SSM_INIT:
              _handleSSMInit();
              break;

            case COMMAND_RESET:
              _handleReset();
              break;

            case COMMAND_START:
              _handleStart();
              break;

	    default:
	      throw CONTROLLER_INVALID_COMMAND;
	    }
	}
      catch( controller_error e )
	{
          syslog( LOG_ERR, "exception caught in control message loop: %s", e.what() );
          throw e;
	}
    }
}

void Controller::_shutdown()
{
  _shmm.currentStateSet( MU2EERD_SHUTDOWN );
}

void Controller::start()
{
  _shmm.startTimeSetToNow();
  
  _shmm.currentStateSet( MU2EERD_STARTUP );

  _shmm.pidSet( getpid() );

  if( _cm.ssmGet().autoInitGet() )
    {
      // Initialize the spill state machine
      _ssm.initialize();
    }

  _shmm.currentStateSet( MU2EERD_RUNNING );

  // Message queue loop
  _processMessages();

  _shutdown();
}

void Controller::testDaemonStart( string cfile )
{
  _testdCM = new ConfigurationManager();
  if( cfile != "" )
    {
      _testdCM->load( cfile );
    }

  _testdCtlr = new Controller( *_testdCM, TEST_DAEMON_CMQ_NAME, TEST_DAEMON_SHM_NAME );

  _testdThread = new thread( []() {
      try
        {
          _testdCtlr->start();
        }
      catch( controller_error e )
        {
          cerr << "test daemon exception: " << e.what() << endl;
        }
  } );
}

void Controller::testDaemonCleanup()
{
  _testdThread->join();
  
  delete _testdThread;
  delete _testdCtlr;
  delete _testdCM;
}
