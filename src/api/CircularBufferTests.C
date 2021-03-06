/**
 * CircularBufferTests.C
 *
 * This file contains unit tests for the CircularBuffer class.
 *
 * @author rtadkins
 */

#include <ctime>
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
    CircularBuffer<double> circular_buffer( BUFFER_SIZE );
  }

  void teardown()
  {
  }
};

/**
 * Test Initial Values
 *
 * Verify the initial values of the elements of the circular buffer
 */
TEST( CircularBuffConstruction, InitialValues )
{
  CircularBuffer<double> circular_buffer( BUFFER_SIZE );

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
    CircularBuffer<double> circular_buffer( BUFFER_SIZE );
  }

  void teardown()
  {
  }
};

/**
 * Test Circular Buffer Enqueue
 *                                                                                                                    
 * Test that the circular buffer can properly enqueue one element.
 */
TEST( CircularBuffEnqueue, EnqueueOneElement )
{
  int head, tail;
  CircularBuffer<double> circular_buffer( BUFFER_SIZE );

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

/**
 * Test Circular Buffer Enqueue
 *
 * Test that the circular buffer can properly enqueue the number of elements equal to the capacity.
 */
TEST( CircularBuffEnqueue, EnqueueEntireBuffer )
{
  int capacity, i;
  CircularBuffer<double> circular_buffer( BUFFER_SIZE );

  capacity = circular_buffer.capacityGet();
  for ( i = 0; i < capacity; i++ )
  {
    circular_buffer.enqueue(i);
  }

  CHECK_EQUAL( 0, circular_buffer.headGet() );
  CHECK_EQUAL( 9799, circular_buffer.tailGet() );
  CHECK_EQUAL( 9800, circular_buffer.sizeGet() );

  for (i = 0; i < capacity; i++)
  {
    CHECK_EQUAL( i, circular_buffer.dataGet(i) );
  }
}

/**
 * Test Circular Buffer Enqueue Wrap Around
 *
 * Test that elements will properly wrap around
 */
TEST( CircularBuffEnqueue, EnqueueWrapAround )
{
  int capacity, i;
  CircularBuffer<double> circular_buffer( BUFFER_SIZE );

  capacity = circular_buffer.capacityGet();
  for ( i = 0; i < capacity; i++ )
  {
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

  for ( i = 3; i < capacity; i++ )
  {
    CHECK_EQUAL(i, circular_buffer.dataGet(i));
  }
}

/**
 * Test Circular Buffer Enqueue Entire Wrap Around
 *
 * Test the continous adding of elements correctly wrap around.
 */
TEST( CircularBuffEnqueue, EnqueueEntireWrapAround )
{
  int capacity, i;
  CircularBuffer<double> circular_buffer( BUFFER_SIZE );

  capacity = circular_buffer.capacityGet();
  for ( i = 0; i < capacity; i++ )
  {
    circular_buffer.enqueue(i);
  }

  circular_buffer.enqueue(9800);
  circular_buffer.enqueue(9801);
  circular_buffer.enqueue(9802);

  for ( i = 0; i < capacity - 3; i++ )
  {
    circular_buffer.enqueue(i);
  }

  CHECK_EQUAL( 0, circular_buffer.headGet() );
  CHECK_EQUAL( 9799, circular_buffer.tailGet() );
  CHECK_EQUAL( 9800, circular_buffer.sizeGet() );

  // check the first three elements
  CHECK_EQUAL( 9800, circular_buffer.dataGet(0) );
  CHECK_EQUAL( 9801, circular_buffer.dataGet(1) );
  CHECK_EQUAL( 9802, circular_buffer.dataGet(2) );

  for ( i = 3; i < capacity - 3; i++ )
  {
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
    CircularBuffer<double> circular_buffer( BUFFER_SIZE );
  }

  void teardown()
  {
  }
};

/**
 * Test Dequeue Simple
 *
 * Test enqueuing and dequeuing one element
 */
TEST( CircularBuffDequeue, DequeueSimple )
{
  CircularBuffer<double> circular_buffer( BUFFER_SIZE );

  circular_buffer.enqueue(10);
  circular_buffer.enqueue(11);
  circular_buffer.enqueue(12);

  CHECK_EQUAL( 3, circular_buffer.sizeGet() );

  circular_buffer.dequeue();

  CHECK_EQUAL( 2, circular_buffer.sizeGet() );
}

/**
 * Test Dequeue Three Elements
 *
 * Test enqueuing and dequeuing three elements
 */
TEST( CircularBuffDequeue, DequeueAllSimple )
{
  CircularBuffer<double> circular_buffer( BUFFER_SIZE );

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

/**
 * Test Enqueue and Dequeue All Simple
 *
 * Test enqueing and dequeing the entire capacity of elements
 */
TEST( CircularBuffDequeue, DequeueAndEnqueueAllSimple )
{
  int capacity, i;
  CircularBuffer<double> circular_buffer( BUFFER_SIZE );

  capacity = circular_buffer.capacityGet();
  for ( i = 0; i < capacity; i++ )
  {
    circular_buffer.enqueue(i);
  }

  for ( i = 0; i < capacity; i++ )
  {
    circular_buffer.dequeue();
  }
}

/**
 * Test Dequeue Empty
 *
 * Test dequeuing from an empty circular buffer
 */
TEST( CircularBuffDequeue, DequeueEmpty )
{
  int capacity, i;
  CircularBuffer<double> circular_buffer( BUFFER_SIZE );

  capacity = circular_buffer.capacityGet();
  for ( i = 0; i < capacity; i++ )
  {
    circular_buffer.enqueue(i);
  }

  for ( i = 0; i < capacity; i++ )
  {
    circular_buffer.dequeue();
  }

  circular_buffer.dequeue();
}

/**
 * Test Dequeue Single
 *
 * Test dequeing a single element
 */
TEST( CircularBuffDequeue, DequeueSingle )
{
  CircularBuffer<double> circular_buffer( BUFFER_SIZE );

  circular_buffer.enqueue(100);

  circular_buffer.dequeue();
}


/*
TEST_GROUP( CircularBuffStructInitialization )
{
  void setup()
  {
    CircularBuffer<CircBuffer> circular_buffer( BUFFER_SIZE );
  }

  void teardown()
  {
  }
};

TEST( CircularBuffStructInitialization, DequeueSingle )
{
  CircularBuffer<CircBuffer> circular_buffer( BUFFER_SIZE );
  std::time_t t = std::time(nullptr);
  CircBuffer c1 = { 10 , t };

  circular_buffer.enqueue(c1);

  CHECK_EQUAL( 1, circular_buffer.sizeGet() );
  c1 = circular_buffer.dataGet(0); 
  circular_buffer.dequeue();

  CHECK_EQUAL( 0, circular_buffer.sizeGet() );
}

TEST( CircularBuffStructInitialization, DequeueMultiple )
{
  CircularBuffer<CircBuffer> circular_buffer( BUFFER_SIZE );
  std::time_t t = std::time(nullptr);
  CircBuffer c1 = { 10 , t };
  CircBuffer c2 = { 20 , t };
  CircBuffer c3 = { 30 , t };

  circular_buffer.enqueue(c1);
  circular_buffer.enqueue(c2);
  circular_buffer.enqueue(c3);

  CHECK_EQUAL( 3, circular_buffer.sizeGet() );
  c1 = circular_buffer.dataGet(0);
  CHECK_EQUAL( 10, c1.data );

  c2 = circular_buffer.dataGet(1);
  c3 = circular_buffer.dataGet(2);
  circular_buffer.dequeue();
  CHECK_EQUAL( 2, circular_buffer.sizeGet() );
  circular_buffer.dequeue();
  CHECK_EQUAL( 1, circular_buffer.sizeGet() );
  circular_buffer.dequeue();
  CHECK_EQUAL( 0, circular_buffer.sizeGet() );
}

TEST( CircularBuffStructInitialization, EnqueueAll )
{
  double i = 0, capacity;
  CircularBuffer<CircBuffer> circular_buffer( BUFFER_SIZE );
  std::time_t t = std::time(nullptr);

  capacity = circular_buffer.capacityGet();
  for( i = 0; i < capacity; i++) {
    circular_buffer.enqueue( CircBuffer { i, t } );
  }

  CHECK_EQUAL( 9800, circular_buffer.sizeGet() );
}

TEST( CircularBuffStructInitialization, EnqueueAllDequeueAll )
{
  double i = 0, capacity;
  CircularBuffer<CircBuffer> circular_buffer( BUFFER_SIZE );
  std::time_t t = std::time(nullptr);

  capacity = circular_buffer.capacityGet();
  for ( i = 0; i < capacity; i++) {
    circular_buffer.enqueue( CircBuffer { i, t } );
  }

  CHECK_EQUAL( 9800, circular_buffer.sizeGet() );

  for ( i = 0; i < capacity; i++ ) {
    circular_buffer.dequeue();
  }

  CHECK_EQUAL( 0, circular_buffer.sizeGet() );
}
*/
