/**
 * ISSMDeviceDriverTest.C
 *
 * This file contains unit tests for the ISSMDeviceDriver class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "ISSMDeviceDriver.H"
#include "SSMDeviceDriverMock.H"

using namespace Mu2eER;
using namespace std;

/**
 * Factory Group
 *
 * Tests for the device driver factory.
 */
TEST_GROUP( FactoryGroup )
{
  void setup()
  {
  }

  void teardown()
  {
  }
};

/**
 * Test instantiating a SSMDeviceDriverMock
 */
TEST( FactoryGroup, InstantiateMock )
{
  auto dev = ISSMDeviceDriver::factory( "mock" );

  CHECK_EQUAL( SSM_IDLE, dev->stateGet() );

  dev->initialize();
  CHECK_EQUAL( SSM_BETWEEN_CYCLES, dev->stateGet() );

  CHECK_EQUAL( SSM_START_CYCLE, dev->waitForStateChange() );
  CHECK_EQUAL( SSM_BETWEEN_SPILLS, dev->waitForStateChange() );
  CHECK_EQUAL( SSM_RAMP, dev->waitForStateChange() );
  CHECK_EQUAL( SSM_FAULT, dev->waitForStateChange() );
}

/**
 * Tests a bad device driver ID
 */
TEST( FactoryGroup, InstantiateUnknown )
{
  auto dev = ISSMDeviceDriver::factory( "unknown" );

  CHECK_EQUAL( SSM_IDLE, dev->stateGet() );

  dev->initialize();
  CHECK_EQUAL( SSM_FAULT, dev->stateGet() );
}
