/**
 * CircularBuffer.H
 *
 * This file contains the definition of the Circular Buffer class.
 *
 * @author rtadkins
 */

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <ctime>

// Assume a frequency of 1440Hz and a need to capture 7 seconds of data
#define BUFFER_SIZE 9800.0

using namespace std;

/*
struct struct_test
{
  double data;
  std::time_t timestamp;
} testing;
*/

namespace Mu2eER
{

  /**
   * CircularBuffer
   *
   * Implements the circular buffer within the shared memory.
   */
  template <class T>
  class CircularBuffer {

  public:

    /**
     * Constructor
     *
     * @param capacity The capacity of the circular buffer
     */
    CircularBuffer( uint16_t capacity );

    /**
     * Remove an element of the buffer
     */
    void dequeue();

    /**
     * Add an element to the buffer
     *
     * @param item The element to be added to the buffer
     */
    void enqueue( T item );

    /**
     * Checks if buffer is empty
     *
     * @return Boolean value designating if buffer is full or empty
     */
    bool empty() const;

    /**
     * Checks if buffer is full
     *
     * @return Boolean value designating if buffer is full or empty
     */
    bool full() const;

    /**
     * Get head of buffer
     *
     * @return Head index of buffer
     */
    uint16_t headGet() const;

    /**
     * Get tail of buffer
     *
     * @return Tail index of buffer
     */
    uint16_t tailGet() const;

    /**
     * Get capacity of buffer
     *
     * @return Capacity of buffer
     */
    uint16_t capacityGet() const;

    /**
     * Get size of buffer
     *
     * @return Size of buffer
     */
    uint16_t sizeGet() const;

    /**
     * Get data at index
     *
     * @return Element at index
     */
    T dataGet( uint16_t index ) const;

    /**
     * Set data at index
     *
     * @param index to set data
     * @param data to set at index
     */
    void dataSet( uint16_t index, T data ) const;

  private:
    /**
     * Buffer array to store contents
     */
    T _buffer[(int) BUFFER_SIZE];

    /**
     * Index of the head of buffer
     */
    uint16_t _head;

    /**
     * Index of the tail of buffer
     */
    uint16_t _tail;

    /**
     * Capacity of buffer
     */
    uint16_t _capacity = BUFFER_SIZE;

    /**
     * Count of elements currently stored in buffer
     */
    uint16_t _size;
  };
};

#endif
