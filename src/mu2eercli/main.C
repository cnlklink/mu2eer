/**
 * main.C
 *
 * This file contains the entry point for the mu2eercli command.
 *
 * @author jdiamond
 */

#include <iostream>
#include <stdexcept>

#include "CLI.H"
#include "errors.H"

namespace Mu2eER
{
  /**
   * mu2eercli return codes
   */
  enum cli_return_code_t
    {
      RC_ERROR = 1,
      RC_SUCCESS = 0
    };
};

using namespace Mu2eER;
using namespace std;

/**
 * Main
 *
 * mu2eercli entry point
 *
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return 0 for success, any other value indicates error (see cli_return_code_t)
 */
int main( int argc, const char* argv[] )
{
  try 
    {
      if( argc < 2 ) 
        {
          throw MU2EERCLI_ARGUMENT_MISSING;
        }

      CLI cli( MU2EERD_CMQ_NAME, MU2EERD_SHM_NAME );

      cli.run( argc, argv );
    }
  catch( Error e )
    {
      if( e == API_CMQ_OPENFAIL )
        {
          cout << "mu2eerd is not running." << endl;
        }
      else
        {
          cout << e.what() << endl;
        }

      return RC_ERROR;
    }

  return RC_SUCCESS;
}
