/**
 * mu2eerd_fef_init.C
 *
 * This file contains the Acsys C++ driver entry point for acnet_mu2eerd.
 *
 * @author jdiamond
 */

#include <syslog.h>

#include "Mu2eerdDevice.H"

using namespace Mu2eER;

extern void register_dev( DeviceBase* );

/**
 * Acsys Initialization Routine
 *
 * Registers our devices with the local Acsys process.
 *
 * @param int argc Argument count
 * @param char** argv Argument vector
 * @return int Return status
 */
int fef_init( int argc, char* argv[] )
{
  try
    {
      // Register devices
      register_dev( new Mu2eerdDevice( MU2EERD_CMQ_NAME, MU2EERD_SHM_NAME ) );
      
      syslog( LOG_INFO, "acnet_mu2eerd started" );
    }
  catch( const runtime_error& e )
    {
      syslog( LOG_ERR, "runtime_error caught in acnet_mu2eerd: %s", e.what() );
      return -1;
    }

  return 0;
}
