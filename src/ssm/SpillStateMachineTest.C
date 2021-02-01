/**
 * SpillStateMachineTest.C
 *
 * This file contains unit tests (CppUTest) for the SpillStateMachine class.
 *
 * @author jdiamond and rtadkins
 */

#include <chrono>
#include <thread>
#include <functional>
#include <cmath>
#include <iostream>
#include "CppUTest/TestHarness.h"

#include "SharedMemoryManager.H"
#include "SpillStateMachine.H"

using namespace Mu2eER;
using namespace std;

/**
 * Use a default configuration for testing
 */
static ConfigurationManager _cm;

/**
 * Shared memory manager for testing
 */
static SharedMemoryManager* _shmm;

/**
 * A global SpillStateMachine object used for all tests
 */
static SpillStateMachine* _ssm;

/**
 * Poll for Spill Cycles
 *
 * Poll the spill counter until it reachs n.
 *
 * @param n Poll until spill counter reaches n
 * @param wait Amount of time (in ms) to wait between polls (default: 5ms)
 * @param timeout Throws a runtime_error if timeout (in ms) is exceeded (default: 1000ms)
 */
void pollForCycles( unsigned int n, unsigned int wait = 5, unsigned int timeout = 1000 )
{
  unsigned int totalWait = 0;

  auto& smb = _shmm->ssmBlockGet();

  while( smb.spillCounterGet() < n )
    {
      this_thread::sleep_for( chrono::milliseconds( wait ) );
      totalWait += wait;

      if( totalWait > timeout )
        {
          throw runtime_error( "pollForCycles timeout" );
        }
    }
}

/**
 * Initialization Group
 *
 * Tests related to initializing and tearing-down the SSM module.
 */
TEST_GROUP( InitGroup )
{
  void setup()
  {
    _shmm = new SharedMemoryManager( "mu2eer_test" );
    _ssm = new SpillStateMachine( _cm, _shmm->ssmBlockGet() );
  }

  void teardown()
  {
    delete _ssm;
    delete _shmm;
  }
};

/**
 * Test Initialization
 *
 * Verify that we can initializae the SSM module with a basic configuration
 */
TEST( InitGroup, Initialize )
{
  auto& smb = _shmm->ssmBlockGet();

  CHECK_EQUAL( SSM_IDLE, smb.currentStateGet() );

  _ssm->initialize();

  CHECK_EQUAL( SSM_BETWEEN_CYCLES, smb.currentStateGet() );

  CHECK_EQUAL( 0, smb.spillCounterGet() );

  CHECK_EQUAL( 0, smb.timeInSpillGet() );

  CHECK_EQUAL( true, smb.ledStateGet() );
}

/**
 * Spill Counter Group
 *
 * Tests related to the operation of the spill counter.
 */
TEST_GROUP( SpillCounterGroup )
{
  void setup()
  {
    _cm.ssmGet().mockSpillsSet( 5 );
    _shmm = new SharedMemoryManager( "mu2eer_test" );
    _ssm = new SpillStateMachine( _cm, _shmm->ssmBlockGet() );
  }

  void teardown()
  {
    delete _ssm;
    delete _shmm;
    _cm.ssmGet().mockSpillsSet( 0 );
  }
};

/**
 * Test Initial Spill Counter
 *
 * Test that the spill counter is 0 after the spill state machine is initialized.
 */
TEST( SpillCounterGroup, InitialValueIsZero )
{
  _ssm->initialize();
  CHECK_EQUAL( 0, _shmm->ssmBlockGet().spillCounterGet() );
}

/**
 * Test Initialize Returns Spill Counter to Zero
 *
 * Test that initialize() will return the spill counter to 0.
 */
TEST( SpillCounterGroup, ResetToZero )
{
  auto& smb = _shmm->ssmBlockGet();

  // Ask for 5 fake spills
  _ssm->initialize();
  CHECK_EQUAL( 0, smb.spillCounterGet() );

  // Run through the fake spills
  _ssm->run();
  pollForCycles( 5 );
  //  CHECK_EQUAL( SSM_FAULT, smb.currentStateGet() );
  CHECK_EQUAL( 5, smb.spillCounterGet() );

  // Re-initialize
  _ssm->initialize();
  CHECK_EQUAL( 0, smb.spillCounterGet() );
}

/**
 * Test Reset
 *
 * Verify that we can reset the SSM module.
 */
TEST( SpillCounterGroup, Reset )
{
  auto& smb = _shmm->ssmBlockGet();

  CHECK_EQUAL( SSM_IDLE, smb.currentStateGet() );

  _ssm->initialize();
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, smb.currentStateGet() );
  CHECK_EQUAL( 0, smb.spillCounterGet() );
  CHECK_EQUAL( 0, smb.timeInSpillGet() );

  _ssm->run();
  pollForCycles( 5 );
  //CHECK_EQUAL( SSM_FAULT, smb.currentStateGet() );
  CHECK_EQUAL( 5, smb.spillCounterGet() );
  CHECK_EQUAL( 107, smb.timeInSpillGet() );

  _ssm->reset();
  CHECK_EQUAL( SSM_IDLE, smb.currentStateGet() );
  CHECK_EQUAL( 0, smb.spillCounterGet() );
  CHECK_EQUAL( 0, smb.timeInSpillGet() );
}

/**
 * Thread Group
 *
 * Tests related to the operation of the SSM thread.
 */
TEST_GROUP( ThreadGroup )
{
  void setup()
  {
    _shmm = new SharedMemoryManager( "mu2eer_test" );
    _ssm = new SpillStateMachine( _cm, _shmm->ssmBlockGet() );
  }

  void teardown()
  {
    delete _ssm;
    delete _shmm;
  }
};

/**
 * Test Initial Thread State
 *
 * Verify that the SSM thread is not running when the SSM is constructed.
 */
TEST( ThreadGroup, TestInitialState )
{
  CHECK_EQUAL( false, _shmm->ssmBlockGet().threadRunningGet() );
}

/**
 * Test Thread Start
 *
 * Verify that the thread is running after calling run().
 */
TEST( ThreadGroup, TestRunning )
{
  auto& smb = _shmm->ssmBlockGet();

  // Verify that the thread is not running
  CHECK_EQUAL( false, smb.threadRunningGet() );

  // Start the SSM thread
  _ssm->run();

  // Verify that the thread is running
  CHECK_EQUAL( true, smb.threadRunningGet() );

  // Wait for the SSM thread to stop
  _ssm->stop();

  // Verify that the thread is not running
  CHECK_EQUAL( false, smb.threadRunningGet() );
}

/**
 * Test Circular Buffer Fill
 *
 * Create thread & verify that the circular buffer is filled.
 */
TEST( ThreadGroup, TestCircularBufferThread )
{
  int capacity, i, degrees = 0;
  double res;

  auto& smb = _shmm->ssmBlockGet();

  std::thread threadObj(&SpillStateMachineSMB::fillCircularBuffer, &smb);
  threadObj.join();

  CircularBuffer<double> circBuff = smb.circularBufferGet();
  capacity = circBuff.capacityGet();

  for ( i = 0; i < capacity; i++ )
  {
    res = ( degrees % 360 ) * 3.14159 / 180;

    DOUBLES_EQUAL( sin(res), circBuff.dataGet(i), 0.01 );

    degrees += 15;
  }
}

/**
 * Test Circular Buffer Fill
 *
 * Verify the thread is running & the circular buffer is filled.
 */
TEST( ThreadGroup, TestCircularBuffer )
{
  int capacity, i, degrees = 0;
  double res;

  auto& smb = _shmm->ssmBlockGet();

  // Verify that the thread is not running
  CHECK_EQUAL( false, smb.threadRunningGet() );

  // Start the SSM thread
  _ssm->run();

  // Verify that the thread is running
  CHECK_EQUAL( true, smb.threadRunningGet() );

  smb.fillCircularBuffer();

  // Wait for the SSM thread to stop
  _ssm->stop();

  // Verify that the thread is not running
  CHECK_EQUAL( false, smb.threadRunningGet() );

  CircularBuffer<double> circBuff = smb.circularBufferGet();
  capacity = circBuff.capacityGet();

  for ( i = 0; i < capacity; i++ )
  {
    res = ( degrees % 360 ) * 3.14159 / 180;

    CHECK_EQUAL( sin(res), circBuff.dataGet(i) );
    
    degrees += 15;
  }
}

/**
 * Operation Group
 *
 * Tests related to the operation of the SSM.
 */
TEST_GROUP( OperationGroup )
{
  void setup()
  {
    _shmm = new SharedMemoryManager( "mu2eer_test" );
    _ssm = new SpillStateMachine( _cm, _shmm->ssmBlockGet() );
  }

  void teardown()
  {
    delete _ssm;
    delete _shmm;
  }
};

/**
 * Test Fault
 *
 * Verify that the SSM goes to the SSM_FAULT state when requested.
 */
TEST( OperationGroup, TestFault )
{
  auto& smb = _shmm->ssmBlockGet();

  _ssm->initialize();

  CHECK_EQUAL( SSM_BETWEEN_CYCLES, smb.currentStateGet() );

  _ssm->fault();

  CHECK_EQUAL( SSM_FAULT, smb.currentStateGet() );
}

/**
 * LED Group
 *
 * Tests related to the operation of the LED.
 */
TEST_GROUP( LEDGroup )
{
  void setup()
  {
    _shmm = new SharedMemoryManager( "mu2eer_test" );
    _ssm = new SpillStateMachine( _cm, _shmm->ssmBlockGet() );
  }

  void teardown()
  {
    delete _ssm;
    delete _shmm;
  }
};

/**
 * Test Initial State
 *
 * Verify that the LED turns on when initialized & off when stopped.
 */
TEST( LEDGroup, TestRunning)
{
  auto& smb = _shmm->ssmBlockGet();

  _ssm->initialize();

  CHECK_EQUAL( true, smb.ledStateGet() );

  _ssm->run();
  _ssm->stop();

  CHECK_EQUAL( false, smb.ledStateGet() );
}

/**
 * Test Fault State
 *
 * Verify that the LED turns off in fault state.
 */
TEST( LEDGroup, TestFault )
{
  auto& smb = _shmm->ssmBlockGet();

  _ssm->initialize();

  CHECK_EQUAL( 1, smb.ledStateGet() );

  _ssm->fault();

  CHECK_EQUAL( 0, smb.ledStateGet() );
}
