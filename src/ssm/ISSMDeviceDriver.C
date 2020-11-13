/**
 * ISSMDeviceDriver.C
 *
 * This file contains the implementation of the ISSMDeviceDriver class.
 *
 * @author jdiamond and rtadkins
 */

#include "ISSMDeviceDriver.H"
#include "SSMDeviceDriverMock.H"

using namespace Mu2eER;
using namespace std;

ISSMDeviceDriver::ISSMDeviceDriver()
{
}

ISSMDeviceDriver::~ISSMDeviceDriver()
{
}

unique_ptr<ISSMDeviceDriver> ISSMDeviceDriver::factory( string driverId )
{
  if( 0 == driverId.compare( "mock" ) )
    {
      return unique_ptr<ISSMDeviceDriver>( new SSMDeviceDriverMock( {
          SSM_BETWEEN_CYCLES,
          SSM_START_CYCLE,
          SSM_BETWEEN_SPILLS,
          SSM_RAMP,
          SSM_FAULT
      } ) );
    }

  return unique_ptr<ISSMDeviceDriver>( new SSMDeviceDriverMock( { SSM_FAULT } ) );
}

void ledOn() {
  _led = true;
}

void ledOff() {
  _led = false;
}
