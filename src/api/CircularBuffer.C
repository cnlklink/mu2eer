/**
 * CircularBuffer.C
 *
 * This file contains the implementation of the CircularBuffer class.
 *
 * @author rtadkins
 */

#include <cstddef>
#include <cstdint>
#include <mutex>

#include "CircularBuffer.H"

using namespace Mu2eER;
using namespace std;

std::mutex _circular_buffer_lock;

template <class T>
CircularBuffer<T>::CircularBuffer( uint16_t capacity )
  : _head( 0 ),
    _tail( -1 ),
    _capacity( capacity ),
    _size( 0 )
{
}

template <class T>
void CircularBuffer<T>::dequeue()
{
  _circular_buffer_lock.lock();

  if ( !empty() )
  {
    //increment _head
    _head = (_head + 1 ) % _capacity;
  
    //decrease size
    _size--;
  }

  _circular_buffer_lock.unlock();
}

template <class T>
void CircularBuffer<T>::enqueue( T element )
{
  _circular_buffer_lock.lock();

  //designate index for element
  _tail = ( _tail + 1 ) % _capacity;

  //set element to index
  _buffer[_tail] = element;
  
  //increment size
  if ( !full() )
  {
    _size++;
  }

  _circular_buffer_lock.unlock();
}

template <class T>
bool CircularBuffer<T>::empty() const
{
  return ( _size == 0 ) ? true : false;
}

template <class T>
bool CircularBuffer<T>::full() const
{
  return ( _size == _capacity ) ? true : false;
}

template <class T>
uint16_t CircularBuffer<T>::headGet() const
{
  return _head;
}

template <class T>
int16_t CircularBuffer<T>::tailGet() const
{
  return _tail;
}

template <class T>
uint16_t CircularBuffer<T>::capacityGet() const
{
  return _capacity;
}

template <class T>
uint16_t CircularBuffer<T>::sizeGet() const
{
  return _size;
}

template <class T>
T CircularBuffer<T>::dataGet( uint16_t index ) const
{
  _circular_buffer_lock.lock();

  T data = _buffer[(int) index];

  _circular_buffer_lock.unlock();

  return data;
}

template class CircularBuffer<double>;
template class CircularBuffer<struct_test>;
