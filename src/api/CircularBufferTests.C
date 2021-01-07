/**
 * CircularBufferTests.C
 *
 * This file contains unit tests for the CircularBuffer class.
 *
 * @author rtadkins
 */

#include "CppUTest/TestHarness.h"

#include "CircularBuffer.H"

#define BUFFER_SIZE 9800

using namespace Mu2eER;
using namespace std;

/**
 * Construction Group
 *
 * Tests related to constructing & destructing the CircularBuffer.
 */
TEST_GROUP( CircularBuffConstruction )
{
  void setup()
  {
    CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );
  }

  void teardown()
  {
  }
};

TEST( CircularBuffConstruction, InitialValues )
{
  CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );

  CHECK_EQUAL( BUFFER_SIZE, circular_buffer.capacityGet() );
  CHECK_EQUAL( 0, circular_buffer.sizeGet() );
  CHECK_EQUAL( 0, circular_buffer.headGet() );
  CHECK_EQUAL( 0, circular_buffer.tailGet() );
}
