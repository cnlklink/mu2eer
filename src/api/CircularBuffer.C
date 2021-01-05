/**
 * CircularBuffer.C
 *
 * This file contains the implementation of the CircularBuffer class.
 *
 * @author rtadkins
 */

#include "CircularBuffer.H"

using namespace Mu2eER;
using namespace std;

template <class T>
CircularBuffer<T>::CircularBuffer( T capacity )
  : _capacity( capacity ),
    _head( 0 ),
    _size( 0 ),
    _tail( 0 )
{
}

template <class T>
T CircularBuffer<T>::dequeue()
{
  int element;

  if ( _buffer == NULL ) {
    return -1;
  }

  element = _buffer[_head];

  _head = ( _head + 1 ) % _capacity;

  _size--;

  return element;
}

template <class T>
T CircularBuffer<T>::enqueue( T element )
{
  int element_index;

  if ( _buffer == NULL ) {
    return -1;
  }

  element_index = ( _head + 1 ) % _capacity;

  _buffer[element_index] = element;

  _size++;
}

template <class T>
bool CircularBuffer<T>::empty()
{
  return ( _size == 0 ) ? true : false;
}

template <class T>
bool CircularBuffer<T>::full()
{
  return ( _size == _capacity ) ? true : false;
}

template <class T>
T CircularBuffer<T>::headGet()
{
  return _head;
}

template <class T>
T CircularBuffer<T>::tailGet()
{
  return _tail;
}

template <class T>
T CircularBuffer<T>::capacityGet()
{
  return _capacity;
}

template <class T>
T CircularBuffer<T>::sizeGet()
{
  return _size;
}
