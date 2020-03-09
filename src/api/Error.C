/**
 * Error.C
 *
 * This file contains the implementation of the Error class.
 *
 * @author jdiamond
 */

#include "Error.H"

using namespace Mu2eER;
using namespace std;

Error::Error( module_t module, int errCode, string message )
  : runtime_error( message ),
    _errCode( errCode ),
    _module( module )
{
}

bool Mu2eER::operator==( const Error& lhs, const Error& rhs )
{
  return lhs._errCode == rhs._errCode && lhs._module == rhs._module;
}

bool Mu2eER::operator!=( const Error& lhs, const Error& rhs )
{
  return !(lhs == rhs);
}
