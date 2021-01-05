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


/**
 * Construction Group
 *
 * Tests related to constructing & destructing the CircularBuffer.
 */
TEST_GROUP( ConstructionGroup )
{
  void setup()
  {
    CircularBuffer<int16_t> circular_buffer(BUFFER_SIZE);
  }

  void teardown()
  {
  }
};

TEST( ConstructionGroup, InitialValues )
{
  CircularBuffer<int16_t> circular_buffer(BUFFER_SIZE);

  CHECK_EQUAL(BUFFER_SIZE, circular_buffer.capacityGet());
  CHECK_EQUAL(0, circular_buffer.sizeGet());
  CHECK_EQUAL(0, circular_buffer.headGet());
  CHECK_EQUAL(0, circular_buffer.tailGet());
}
