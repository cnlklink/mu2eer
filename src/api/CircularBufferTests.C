/**
 * CircularBufferTests.C
 *
 * This file contains unit tests for the CircularBuffer class.
 *
 * @author rtadkins
 */

#include <iostream>
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
  CHECK_EQUAL( -1, circular_buffer.tailGet() );
}

/**
 * CircularBuffEnqueue Group
 *
 * Tests related to the CircularBuffer enqueue function.
 */
TEST_GROUP( CircularBuffEnqueue )
{
  void setup()
  {
    CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );
  }

  void teardown()
  {
  }
};

TEST( CircularBuffEnqueue, Enqueue )
{
  int head, tail;
  CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );

  head = circular_buffer.headGet();
  tail = circular_buffer.tailGet();
  CHECK_EQUAL( BUFFER_SIZE, circular_buffer.capacityGet() );
  CHECK_EQUAL( 0, circular_buffer.sizeGet() );
  CHECK_EQUAL( 0, head );
  CHECK_EQUAL( -1, tail );

  circular_buffer.enqueue(20);

  head = circular_buffer.headGet();
  tail = circular_buffer.tailGet();
  printf("_head is %d and tail is %d", head, tail);


  CHECK_EQUAL( 20, circular_buffer.dataGet( head ) );
  CHECK_EQUAL( 20, circular_buffer.dataGet( tail ) );
  CHECK_EQUAL( 1, circular_buffer.sizeGet() );
}
