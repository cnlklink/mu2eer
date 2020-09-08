/**
 * CLITests.C
 *
 * This file contains unit tests for the CLI class.
 *
 * @author jdiamond
 */

#include <iostream>

#include "CLI.H"
#include "../mu2eerd/Controller.H"

#include "CppUTest/TestHarness.h"

using namespace Mu2eER;
using namespace std;

/**
 * Global CLI object
 */
static CLI* _cli;

/**
 * Construction Tests
 *
 * Tests related to constructing a CLI object
 */
TEST_GROUP( ConstructionGroup )
{
  void setup()
  {
  }

  void teardown()
  {
  }
};

TEST( ConstructionGroup, DaemonNotPresent )
{
  CHECK_THROWS( Error, CLI( "/does_not_exist", "does_not_exist" ) );
}

/**
 * PID Command Tests
 *
 * Tests related to executing the "pid" command.
 */
TEST_GROUP( PIDGroup )
{
  void setup()
  {
    Controller::testDaemonStart();

    SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
    shmc.waitForState( MU2EERD_RUNNING );

    _cli = new CLI( Controller::TEST_DAEMON_CMQ_NAME, Controller::TEST_DAEMON_SHM_NAME );
  }

  void teardown()
  {
    delete _cli;

    ControlMQClient mqc( Controller::TEST_DAEMON_CMQ_NAME );
    mqc.shutdown();
    Controller::testDaemonCleanup();
  }
};

TEST( PIDGroup, CommandNotFound )
{
  unsigned int argc = 2;
  const char* argv[] = { "mu2eercli", "does_not_exist" };

  CHECK_THROWS( Error, _cli->run( argc, argv ) );
}

TEST( PIDGroup, Run )
{
  unsigned int argc = 2;
  const char *argv[] = { "mu2eercli", "pid" };

  _cli->run( argc, argv );
}

/**
 * Shutdown Command Tests
 *
 * Tests related to executing the "shutdown" command.
 */
TEST_GROUP( ShutdownGroup )
{
  void setup()
  {
    Controller::testDaemonStart();

    SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
    shmc.waitForState( MU2EERD_RUNNING );

    _cli = new CLI( Controller::TEST_DAEMON_CMQ_NAME, Controller::TEST_DAEMON_SHM_NAME );
  }

  void teardown()
  {
    Controller::testDaemonCleanup();
    delete _cli;
  }
};

TEST( ShutdownGroup, Run )
{
  unsigned int argc = 2;
  const char *argv[] = { "mu2eercli", "shutdown" };

  SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );

  // Because we're running in the same process as the test daemon, this should timeout...
  CHECK_THROWS( Error, _cli->run( argc, argv ) );

  // But then we should be in the shutdown state
  CHECK_EQUAL( MU2EERD_SHUTDOWN, shmc.currentStateGet() );
}

/**
 * Show Command Tests
 *
 * Tests related to executing the "show" command.
 */
TEST_GROUP( ShowGroup )
{
  void setup()
  {
    Controller::testDaemonStart();

    SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
    shmc.waitForState( MU2EERD_RUNNING );

    _cli = new CLI( Controller::TEST_DAEMON_CMQ_NAME, Controller::TEST_DAEMON_SHM_NAME );
  }

  void teardown()
  {
    delete _cli;

    ControlMQClient mqc( Controller::TEST_DAEMON_CMQ_NAME );
    mqc.shutdown();
    Controller::testDaemonCleanup();
  }
};

TEST( ShowGroup, Run )
{
  unsigned int argc = 2;
  const char *argv[] = { "mu2eercli", "show" };

  _cli->run( argc, argv );
}

/**
 * Start Command Tests
 *
 * Tests related to executing the "start" command.
 */
TEST_GROUP( StartGroup )
{
  void setup()
  {
    Controller::testDaemonStart();

    SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
    shmc.waitForState( MU2EERD_RUNNING );

    _cli = new CLI( Controller::TEST_DAEMON_CMQ_NAME, Controller::TEST_DAEMON_SHM_NAME );
  }

  void teardown()
  {
    delete _cli;

    ControlMQClient mqc( Controller::TEST_DAEMON_CMQ_NAME );
    mqc.shutdown();
    Controller::testDaemonCleanup();
  }
};

TEST( StartGroup, Run )
{
  unsigned int argc = 2;
  const char *argv[] = { "mu2eercli", "start" };

  _cli->run( argc, argv );
}

/**
 * Dump Command Tests
 *
 * Tests related to executing the "dump" command.
 */
TEST_GROUP( DumpGroup )
{
  void setup()
  {
    Controller::testDaemonStart();

    SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
    shmc.waitForState( MU2EERD_RUNNING );

    _cli = new CLI( Controller::TEST_DAEMON_CMQ_NAME, Controller::TEST_DAEMON_SHM_NAME );
  }

  void teardown()
  {
    delete _cli;

    ControlMQClient mqc( Controller::TEST_DAEMON_CMQ_NAME );
    mqc.shutdown();
    Controller::testDaemonCleanup();
  }
};

TEST( DumpGroup, Run )
{
  unsigned int argc = 2;
  const char *argv[] = { "mu2eercli", "dump" };

  _cli->run( argc, argv );
}

/**
 * Fault Command Tests
 *
 * Tests related to executing the "fault" command.
 */
TEST_GROUP( FaultGroup )
{
  void setup()
  {
    Controller::testDaemonStart();

    SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
    shmc.waitForState( MU2EERD_RUNNING );

    _cli = new CLI( Controller::TEST_DAEMON_CMQ_NAME, Controller::TEST_DAEMON_SHM_NAME );
  }

  void teardown()
  {
    delete _cli;

    ControlMQClient mqc( Controller::TEST_DAEMON_CMQ_NAME );
    mqc.shutdown();
    Controller::testDaemonCleanup();
  }
};

TEST( FaultGroup, Run )
{
  try
    {
      unsigned int argc = 2;
      const char *argv[] = { "mu2eercli", "fault" };
      
      _cli->run( argc, argv );

      SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
      shmc.waitForSSMState( SSM_FAULT, 5, 20 );
      CHECK_EQUAL( SSM_FAULT, shmc.ssmBlockGet().currentStateGet() );
    }
  catch( Error e )
    {
      cout << e.what() << endl;
      FAIL( "Exception occured." );
    }
}

/**
 * Reset Command Tests
 *
 * Tests related to executing the "reset" command.
 */
TEST_GROUP( ResetGroup )
{
  void setup()
  {
    Controller::testDaemonStart();

    SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
    shmc.waitForState( MU2EERD_RUNNING );

    _cli = new CLI( Controller::TEST_DAEMON_CMQ_NAME, Controller::TEST_DAEMON_SHM_NAME );
  }

  void teardown()
  {
    delete _cli;

    ControlMQClient mqc( Controller::TEST_DAEMON_CMQ_NAME );
    mqc.shutdown();
    Controller::testDaemonCleanup();
  }
};

TEST( ResetGroup, Run )
{
  try
    {
      SharedMemoryClient shmc( Controller::TEST_DAEMON_SHM_NAME );
      auto& smb = shmc.ssmBlockGet();

      // Should be in the IDLE state when we start
      CHECK_EQUAL( SSM_IDLE, smb.currentStateGet() );

      // Should be in the FAULT state after running through all of the test cycles
      unsigned int argc = 2;
      const char *argv[] = { "mu2eercli", "start" };
      _cli->run( argc, argv );
      shmc.waitForSSMState( SSM_FAULT, 5, 20 );
      CHECK_EQUAL( SSM_FAULT, smb.currentStateGet() );

      // Should be IDLE again after a reset
      argv[1] = "reset";
      _cli->run( argc, argv );
      shmc.waitForSSMState( SSM_IDLE, 5, 20 );
      CHECK_EQUAL( SSM_IDLE, smb.currentStateGet() );
    }
  catch( Error e )
    {
      cout << e.what() << endl;
      FAIL( "Exception occured." );
    }
}
