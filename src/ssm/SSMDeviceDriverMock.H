/**
 * SSMDeviceDriverMock.H
 *
 * This file contains the definition of the SSMDeviceDriverMock class.
 *
 * @author jdiamond
 */

#ifndef SSMDEVICEDRIVERMOCK_H
#define SSMDEVICEDRIVERMOCK_H

#include "ISSMDeviceDriver.H"

namespace Mu2eER
{
  /**
   * SSMDeviceDriverMock
   *
   * A Mock that can stand in for SSMDeviceDriver during testing.
   */
  class SSMDeviceDriverMock : public ISSMDeviceDriver
  {
  public:
    /**
     * Destructor
     */
    virtual ~SSMDeviceDriverMock()
    {
    }

  private:
  };
};

#endif
