/**
 * TestSystemController.C
 *
 * This file contains the implementation of the TestSystemController class.
 *
 * @author jdiamond
 */

#include "TestSystemController.H"

using namespace std;
using namespace Mu2eER;

bool TestSystemController::isRebooting() const
{
  return _rebooting;
}

void TestSystemController::doReboot()
{
  _rebooting = true;
}

unsigned int TestSystemController::jenkinsNumberGet() const
{
  return 99;
}

TestSystemController::TestSystemController()
  : _rebooting( false )
{
}
