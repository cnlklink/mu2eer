/**
 * testutils.H
 *
 * This file contains macros to be used with CppUTest.
 *
 * @author jdiamond
 */

// Check that exp throws AcnetError ae
#define CHECK_THROWS_ACNETERROR(ae,exp) \
  try                                               \
    {                                               \
      (exp);                                        \
      FAIL( "expected to throw AcnetError" ); \
    }                                                         \
  catch( AcnetError e )                                       \
    {                                                         \
      CHECK_EQUAL( ae.getValue(), e.getValue() );             \
    }                                                         \
  catch( ... )                                                \
    {                                                         \
      FAIL( "expected to throw AcnetError facility, but threw something else" ); \
    }
