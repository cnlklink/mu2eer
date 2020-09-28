/**
 * ISystemController.H
 *
 * This file contains the definition of the ISystemController interface.
 *
 * @author jdiamond
 */

#ifndef ISYSTEMCONTROLLER_H
#define ISYSTEMCONTROLLER_H

namespace Mu2eER
{
  /**
   * ISystemController
   *
   * Interface to a SystemController.
   */
  class ISystemController
  {
  public:
    /**
     * Is Rebooting
     *
     * @return bool True if the system is being rebooted.
     */
    virtual bool isRebooting() const = 0;

    /**
     * Reboot
     *
     * Sets the "is rebooting" flag and reboots the system.
     */
    virtual void doReboot() = 0;

    /**
     * Get Jenkins Build Number
     *
     * Returns the Jenkin's build number for the OS Buildroot image.  On a Buildroot system that
     * is built by Jenkins this is stored in /etc/jenkins_build_number.
     *
     * @return Jenkins build number
     */
    virtual unsigned int jenkinsNumberGet() const = 0;
  };
};

#endif
