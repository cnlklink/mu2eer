/**
 * TestSystemController.H
 *
 * This file contains the definition of the TestSystemController class.
 *
 * @author jdiamond
 */

#ifndef TESTSYSTEMCONTROLLER_H
#define TESTSYSTEMCONTROLLER_H

#include "ISystemController.H"

namespace Mu2eER
{
  /**
   * TestSystemController
   *
   * A "fake" ISystemController for testing.  Only pretends to reboot the system by asserting the
   * "is rebooting" flag" when reboot() is called.
   */
  class TestSystemController : public ISystemController
  {
  public:
    // Override
    virtual bool isRebooting() const;

    // Override
    virtual void doReboot();

    // Override
    virtual unsigned int jenkinsNumberGet() const;

    /**
     * Constructor
     */
    TestSystemController();

  private:
    /**
     * Rebooting flag
     */
    bool _rebooting;
  };
};

#endif
