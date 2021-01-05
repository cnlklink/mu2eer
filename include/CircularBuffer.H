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

    T dequeue();
    void enqueue(T item);
    bool empty() const;
    bool full() const;
    T headGet() const;
    T tailGet() const;
    T capacityGet() const;
    T sizeGet() const;

  private:

    T _buffer[BUFFER_SIZE];
    T _capacity;

    T _head;
    T _tail;

    T _capacity;
    T _size;

  };
};

#endif
