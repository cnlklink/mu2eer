/**
 * AllTests.C
 *
 * This file contains the entry point for the CppUTest test harness.
 *
 * @author jdiamond
 */

#include "CppUTest/CommandLineTestRunner.h"

int main( int ac, char** av )
{
  return CommandLineTestRunner::RunAllTests( ac, av );
}
