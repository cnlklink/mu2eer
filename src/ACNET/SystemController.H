/**
 * SystemController.H
 *
 * This file contains the definition of the SystemController class.
 *
 * @author jdiamond
 */

#ifndef SYSTEMCONTROLLER_H
#define SYSTEMCONTROLLER_H

#include "ISystemController.H"

namespace Mu2eER
{
  /**
   * SystemController
   *
   * Real system implementation of ISystemController.
   *   (e.g. SystemController::doReboot() actually reboots the system)
   */
  class SystemController : public ISystemController
  {
  public:
    // Override
    virtual void doReboot();

    // Override
    virtual bool isRebooting() const;

    // Override
    virtual unsigned int jenkinsNumberGet() const;

    /**
     * Constructor
     */
    SystemController();

  private:
    /**
     * Rebooting flag
     */
    bool _rebooting;
  };
};

#endif
