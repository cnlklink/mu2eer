/**
 * CircularBuffer.C
 *
 * This file contains the implementation of the CircularBuffer class.
 *
 * @author rtadkins
 */

#include <cstddef>
#include <cstdint>

#include "CircularBuffer.H"

using namespace Mu2eER;
using namespace std;

template <class T>
CircularBuffer<T>::CircularBuffer( T capacity )
  : _head( 0 ),
    _tail( -1 ),
    _capacity( capacity ),
    _size( 0 )
{
}

template <class T>
void CircularBuffer<T>::dequeue()
{
  if ( !empty() )
  {
    //increment _head
    _head = ((int)_head + 1 ) % (int)_capacity;
    //decrease size
    _size--;
  }
}

template <class T>
void CircularBuffer<T>::enqueue( T element )
{
  //designate index for element
  _tail = ( (int)_tail + 1 ) % (int)_capacity;
  //set element to index
  _buffer[(int)_tail] = element;
  //increment size
  if ( !full() )
  {
    _size++;
  }
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
T CircularBuffer<T>::headGet() const
{
  return _head;
}

template <class T>
T CircularBuffer<T>::tailGet() const
{
  return _tail;
}

template <class T>
T CircularBuffer<T>::capacityGet() const
{
  return BUFFER_SIZE;
}

template <class T>
T CircularBuffer<T>::sizeGet() const
{
  return _size;
}

template <class T>
T CircularBuffer<T>::dataGet( T index ) const
{
  return _buffer[(int) index];
}

template class CircularBuffer<double>;
