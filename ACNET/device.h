/**
 * device.h
 *
 * This file contains a mock template definition for the ACSys/FE cdev device class.
 *
 * @author jdiamond
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <string>

using namespace std;

namespace FFF
{
  template<int N> class Device
    {
      static const int n = N;
      
    public:
      /**
       * Constructor
       *
       * @param name Device name
       * @param description Device description
       */
    Device( string name, string description ) : _name( name ), _description( description )
      {
      }
      
    private:
      /**
       * Name
       *
       * Name of the device
       */
      string _name;
      
      /**
       * Description
       *
       * Description for the device
       */
      string _description;
    };
};

#endif
