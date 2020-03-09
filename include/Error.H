/**
 * Mu2eERError.H
 *
 * This file contains the definition of the Error class.
 *
 * @author jdiamond
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>

#include "api.H"

using namespace std;

namespace Mu2eER
{
  /**
   * Error
   *
   * Standard error exception for Mu2eER.
   */
  class Error : public runtime_error
  {
  public:
    /**
     * Constructor
     *
     * @param module The module that raised this error
     * @param errCode The error code
     * @param message Error message
     */
    Error( module_t module, int errCode, string message );

    friend bool operator==( const Error& lhs, const Error& rhs );
    friend bool operator!=( const Error& lhs, const Error& rhs );

  private:
    /**
     * Error code
     */
    int _errCode;

    /**
     * Mu2eER module that generated this error
     */
    module_t _module;
  };

  /**
   * Equality Operator
   *
   * @param lhs Left-hand Error
   * @param rhs Right-hand Error
   * @return True if lhs == rhs
   */
  bool operator==( const Error& lhs, const Error& rhs );

  /**
   * Non-equality Operator
   *
   * @param lhs Left-hand Error
   * @param rhs Right-hand Error
   * @return True if lhs != rhs
   */
  bool operator!=( const Error& lhs, const Error& rhs );
};

#endif
