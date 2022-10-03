/**
 * TCLKFactoryTest.C
 *
 * This file contains tests for the TCLKFactory class.
 *
 * @author jdiamond
 */

#include <memory>

#include "CppUTest/TestHarness.h"

#include "TCLKDecoderFactory.H"
#include "TCLKSMB.H"

using namespace std;
using namespace Mu2eER;

TEST_GROUP( TCLKFactoryCoreGroup )
{
  void setup()
  {
  }

  void teardown()
  {
  }
};

TEST( TCLKFactoryCoreGroup, ThrowsUnknown )
{
  CHECK_THROWS( runtime_error, TCLKDecoderFactory( "unknown" ) );
}

TEST( TCLKFactoryCoreGroup, ReturnsMock )
{
  unique_ptr<ITCLKDecoderDriver> tclkDecoder = TCLKDecoderFactory( "mock" );

  CHECK( nullptr != tclkDecoder.get() );

  STRCMP_EQUAL( "mock", tclkDecoder->nameGet().c_str() );
}
