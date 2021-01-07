/**
 * CircularBuffer.H
 *
 * This file contains the definition of the Circular Buffer class.
 *
 * @author rtadkins
 */

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

// Assume a frequency of 1440Hz and a need to capture 7 seconds of data
#define BUFFER_SIZE 9800

using namespace std;

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
    CircularBuffer( T capacity );

    /**
     * Remove an element of the buffer
     *
     * @return The element at the head of the buffer
     */
    T dequeue();

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
    T headGet() const;

    /**
     * Get tail of buffer
     *
     * @return Tail index of buffer
     */
    T tailGet() const;

    /**
     * Get capacity of buffer
     *
     * @return Capacity of buffer
     */
    T capacityGet() const;

    /**
     * Get size of buffer
     *
     * @return Size of buffer
     */
    T sizeGet() const;

  private:
    /**
     * Buffer array to store contents
     */
    T _buffer[BUFFER_SIZE];

    /**
     * Index of the head of buffer
     */
    T _head;

    /**
     * Index of the tail of buffer
     */
    T _tail;

    /**
     * Capacity of buffer
     */
    T _capacity;

    /**
     * Count of elements currently stored in buffer
     */
    T _size;
  };
};

#endif
