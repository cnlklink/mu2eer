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

TEST( CircularBuffEnqueue, EnqueueOneElement )
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

  CHECK_EQUAL( 20, circular_buffer.dataGet( head ) );
  CHECK_EQUAL( 20, circular_buffer.dataGet( tail ) );
  CHECK_EQUAL( 1, circular_buffer.sizeGet() );
}

TEST( CircularBuffEnqueue, EnqueueEntireBuffer )
{
  int capacity, i;
  CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );

  capacity = circular_buffer.capacityGet();
  for (i = 0; i < capacity; i++) {
    circular_buffer.enqueue(i);
  }

  CHECK_EQUAL( 0, circular_buffer.headGet() );
  CHECK_EQUAL( 9799, circular_buffer.tailGet() );
  CHECK_EQUAL( 9800, circular_buffer.sizeGet() );

  for (i = 0; i < capacity; i++) {
    CHECK_EQUAL(i, circular_buffer.dataGet(i));
  }
}

TEST( CircularBuffEnqueue, EnqueueWrapAround )
{
  int capacity, i;
  CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );

  capacity = circular_buffer.capacityGet();
  for (i = 0; i < capacity; i++) {
    circular_buffer.enqueue(i);
  }

  circular_buffer.enqueue(9800);
  circular_buffer.enqueue(9801);
  circular_buffer.enqueue(9802);

  CHECK_EQUAL( 0, circular_buffer.headGet() );
  CHECK_EQUAL( 2, circular_buffer.tailGet() );
  CHECK_EQUAL( 9800, circular_buffer.sizeGet() );

  // check the first three elements
  CHECK_EQUAL( 9800, circular_buffer.dataGet(0) );
  CHECK_EQUAL( 9801, circular_buffer.dataGet(1) );
  CHECK_EQUAL( 9802, circular_buffer.dataGet(2) );

  for (i = 3; i < capacity; i++) {
    CHECK_EQUAL(i, circular_buffer.dataGet(i));
  }
}

TEST( CircularBuffEnqueue, EnqueueEntireWrapAround )
{
  int capacity, i;
  CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );

  capacity = circular_buffer.capacityGet();
  for (i = 0; i < capacity; i++) {
    circular_buffer.enqueue(i);
  }

  circular_buffer.enqueue(9800);
  circular_buffer.enqueue(9801);
  circular_buffer.enqueue(9802);

  for (i = 0; i < capacity - 3; i++) {
    circular_buffer.enqueue(i);
  }

  CHECK_EQUAL( 0, circular_buffer.headGet() );
  CHECK_EQUAL( 9799, circular_buffer.tailGet() );
  CHECK_EQUAL( 9800, circular_buffer.sizeGet() );

  // check the first three elements
  CHECK_EQUAL( 9800, circular_buffer.dataGet(0) );
  CHECK_EQUAL( 9801, circular_buffer.dataGet(1) );
  CHECK_EQUAL( 9802, circular_buffer.dataGet(2) );

  for (i = 3; i < capacity - 3; i++) {
    CHECK_EQUAL((i - 3), circular_buffer.dataGet(i));
  }
}

/**
 * CircularBuffDequeue Group
 *
 * Tests related to the CircularBuffer dequeue function.
 */
TEST_GROUP( CircularBuffDequeue )
{
  void setup()
  {
    CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );
  }

  void teardown()
  {
  }
};

TEST( CircularBuffDequeue, DequeueSimple )
{
  CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );

  circular_buffer.enqueue(10);
  circular_buffer.enqueue(11);
  circular_buffer.enqueue(12);

  CHECK_EQUAL( 3, circular_buffer.sizeGet() );

  circular_buffer.dequeue();

  CHECK_EQUAL( 2, circular_buffer.sizeGet() );
}

TEST( CircularBuffDequeue, DequeueAllSimple )
{
  CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );

  circular_buffer.enqueue(10);
  circular_buffer.enqueue(11);
  circular_buffer.enqueue(12);

  CHECK_EQUAL( 3, circular_buffer.sizeGet() );

  circular_buffer.dequeue();

  CHECK_EQUAL( 2, circular_buffer.sizeGet() );

  circular_buffer.dequeue();

  CHECK_EQUAL( 1, circular_buffer.sizeGet() );

  circular_buffer.dequeue();

  CHECK_EQUAL( 1, circular_buffer.empty() );
}

TEST( CircularBuffDequeue, DequeueAndEnqueueAllSimple )
{
  int capacity, i;
  CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );

  capacity = circular_buffer.capacityGet();
  for (i = 0; i < capacity; i++) {
    circular_buffer.enqueue(i);
  }

  for (i = 0; i < capacity; i++) {
    circular_buffer.dequeue();
  }
}

TEST( CircularBuffDequeue, DequeueEmpty )
{
  int capacity, i;
  CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );

  capacity = circular_buffer.capacityGet();
  for (i = 0; i < capacity; i++) {
    circular_buffer.enqueue(i);
  }

  for (i = 0; i < capacity; i++) {
    circular_buffer.dequeue();
  }

  circular_buffer.dequeue();
}

TEST( CircularBuffDequeue, DequeueSingle )
{
  CircularBuffer<int16_t> circular_buffer( BUFFER_SIZE );

  circular_buffer.enqueue(100);

  circular_buffer.dequeue();
}
