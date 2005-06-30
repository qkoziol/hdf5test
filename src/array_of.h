#ifndef ARRAY_OF_H
#define ARRAY_OF_H

#include "contract.h"
#include "std_cstddef.h"
#include "std_ostream.h"

/*! @class array_of
    @brief A contiguous resizable array of any C++ type.
*/

template <class t>
class array_of
{
 public:

  // Canonical members:

  // Default constructor.

  array_of();


  // Copy constructor.

  array_of(const array_of<t>& xother);

  // Destructor.

  ~array_of();

  // Assignment operator.

  array_of& operator=(const array_of<t>& xother);

  // Class invariant.

  bool invariant() const;


  // Additional features:


  // Make an array with capacity xub.

  array_of(size_t xub);

  // Capacity of this array.

  size_t ub() const;

  // Reference to an array element.

  t& operator[](size_t xindex);

  const t& operator[](size_t xindex) const;

  // Copy of an array element.

  t operator()(size_t xindex) const;

  // Resize array, preserving contents where that makes
  // sense.

  void reserve(size_t xub);

 protected:

 private:

  t* _array;    //  Storage for array elements.

  size_t _ub;  // Capacity of array.
};

template <class t>
array_of<t>::
array_of()
{
  // Preconditions:

  // Body:

  _array = 0;
  _ub    = 0;

  // Postconditions:

  assert(invariant());

  // Exit:
}

template <class t>
array_of<t>::
array_of(size_t xub)
{
  // Preconditions:

  assert(xub > 0);

  // Body:

  _array = new t[xub];
  _ub = xub;

  // Postconditions:

  assert(invariant());

  // Exit:
}

template <class t>
array_of<t>::
array_of(const array_of<t>& xother)
{
  // Preconditions:

  // Body:

  delete [] _array;

  _ub = xother.ub();
  _array = new t[_ub];

  for (size_t i = 0; i < _ub; ++i)
    _array[i] = xother._array[i];

  // Postconditions:

  assert(invariant());
  assert(ub() == xother.ub());
  assert(unexecutable(this[i] == xother[i] for 0 <= i < ub()));

  // Exit:
}

template <class t>
array_of<t>&
array_of<t>::
operator=(const array_of<t>& xother)
{
  // Preconditions:

  // Body:

  if (_array != 0)
  {
    delete [] _array;
    _array = 0;
  }

  _ub = xother.ub();

  if (_ub != 0)
    _array = new t[_ub];

  for (size_t i = 0; i < _ub; ++i)
    _array[i] = xother[i];

  // Postconditions:

  assert(invariant());
  assert(ub() == xother.ub());
  assert(unexecutable(this[i] == xother[i] for 0 <= i < ub()));

  // Exit:

  return *this;
}

template <class t>
array_of<t>::
~array_of()
{
  // Preconditions:

  // none.

  // Body:

  if (_array != 0)
    delete [] _array;

  // Postconditions:

  // Exit:
}

template <class t>
bool
array_of<t>::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  if (_array == 0)
  {
    result = _ub == 0;
  }
  else
  {
    result = _ub > 0;
  }

  // Postconditions:

  // Exit:

  return result;
}

template <class t>
size_t
array_of<t>::
ub() const
{
  // Preconditions:

  // none.

  // Body:

  // Postconditions:

  // Exit:

  return _ub;
}

template <class t>
t&
array_of<t>::
operator[](size_t xindex)
{
  // Preconditions:

  assert(xindex < ub());

  // Body:

  t& result = _array[xindex];

  // Postconditions:

  assert(invariant());

  // Exit:

  return result;
}

template <class t>
const t&
array_of<t>::
operator[](size_t xindex) const
{
  // Preconditions:

  assert(xindex < ub());

  // Body:

  t& result = _array[xindex];

  // Postconditions:

  assert(invariant());

  // Exit:

  return result;
}

template <class t>
t
array_of<t>::
operator()(size_t xindex) const
{
  // Preconditions:

  assert(xindex < ub());

  // Body:

  t result = _array[xindex];

  // Postconditions:

  // Exit:

  return result;
}

template <class t>
void
array_of<t>::
reserve(size_t xub)
{
  // Preconditions:

  assert(xub > 0);

  // Body:

  if (xub > _ub)
  {
    t* tmp = new t[xub];

    for (size_t i = 0; i < _ub; ++i)
      tmp[i] = _array[i];

    if (_array != 0)
      delete [] _array;

    _array = tmp;

    _ub = xub;
  }

  // Postconditions:

  assert(invariant());
  assert(ub() >= xub);
  assert(unexecutable(contents of array are unchanged up to the lesser of xub and old ub()));

  // Exit:

}

template<class t>
ostream&
operator<<(ostream& xos, const array_of<t>& x)
{
  xos << x(0);

  for(size_t i = 1; i < x.ub(); ++i)
    xos << ' ' << x(i);

  return xos;
}

#endif

