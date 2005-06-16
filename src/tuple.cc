#include "tuple.h"
#include "contract.h"

tuple::
tuple()
{
  // Preconditions:

  // Body:

  _d = 0;
  _mem = 0;

  // Postconditions:

  assert(invariant());
  assert(d() == 0);

  // Exit:
}

tuple::
~tuple()
{
  // Preconditions:

  // Body:

  delete [] _mem;

  // Postconditions:

  // Exit:
}

bool
tuple::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  // The state of a tuple consists of its dimension and a possible
  // memory allocation.  There is no relationship between the two:
  // any combination of values is a valid state.  So the invariant
  // is trivially satisfied.

  result = true;

  // Postconditions:

  // Exit:

  return result;
}

tuple::
tuple(const tuple& xother)
{
  // Preconditions:

  // Body:

  _d = xother._d;
  _mem = new hsize_t[_d];
  for (unsigned i = 0; i < _d; ++i)
    _mem[i] = xother._mem[i];

  // Postconditions:

  assert(invariant());
  assert(d() == xother.d());
  assert(unexecutable(foreach 0 <= i < d(); this->[i] == xother[i]));

  // Exit:
}

tuple&
tuple::
operator=(const tuple& xother)
{
  // Preconditions:

  // Body:

  _d = xother._d;

  delete [] _mem;

  _mem = new hsize_t[_d];

  for (unsigned i = 0; i < _d; ++i)
    _mem[i] = xother._mem[i];

  // Postconditions:

  assert(invariant());
  assert(d() == xother.d());
  assert(unexecutable(foreach 0 <= i < d(); this->[i] == xother[i]));

  // Exit:

  return *this;
}

tuple::
tuple(unsigned xdim)
{
  // Preconditions:

  // Body:

  _d = xdim;
  _mem = new hsize_t[xdim];

  // Postconditions:

  assert(invariant());
  assert(d() == xdim);

  // Exit:
}

hsize_t&
tuple::
operator[](unsigned xindex)
{
  hsize_t* ptr_to_result;

  // Preconditions:

  assert(xindex < d());

  // Body:

  ptr_to_result = _mem+xindex;

  // Postconditions:

  assert(invariant());

  // Exit:

  return *ptr_to_result;
}

const hsize_t&
tuple::
operator[](unsigned xindex) const
{
  const hsize_t* ptr_to_result;

  // Preconditions:

  assert(xindex < d());

  // Body:

  ptr_to_result = _mem+xindex;

  // Postconditions:

  // Exit:

  return *ptr_to_result;
}

unsigned
tuple::
d() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _d;

  // Postconditions:

  // Exit:

  return result;
}

void
tuple::
reserve(unsigned xdim)
{
  // Preconditions:

  // Body:

  if (_mem != 0)
  {
    delete [] _mem;
  }

  if (xdim > 0)
  {
    _mem = new hsize_t[xdim];
  }
  else
  {
    // ISSUE:
    // C++ requires that the arg to a new array be strictly positive.
    // However, we can have zero-dimensional spaces, so it seems
    // desirable to allow zero-dimensional tuples.

    _mem = new hsize_t[1];
  }

  _d = xdim;

  // Postconditions:

  assert(invariant());
  assert(d() == xdim);

  // Exit:
}

bool
tuple::
is_positive() const
{
  bool result;

  // Preconditions:

  // Body:

  result = true;
  for (unsigned i = 0; i < _d; ++i)
  {
    if (_mem[i] <= 0)
    {
      result = false;
      break;
    }
  }

  // Postconditions:

  // Exit:

  return result;
}

tuple&
tuple::
operator=(const hsize_t& xval)
{
  tuple* ptr_to_result;

  // Preconditions:

  // Body:

  ptr_to_result = this;

  for (unsigned i = 0; i < _d; ++i)
    _mem[i] = xval;

  // Postconditions:

  assert(unexecutable(all members == xval));

  // Exit:

  return *ptr_to_result;
}

bool
tuple::
operator>=(const tuple& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  result = true;

  for (unsigned i = 0; result && i < _d; ++i)
  {
    if (_mem[i] < xother._mem[i])
    {
      result = false;
      break;
    }
  }

  // Postconditions:

  assert(unexecutable(result ? all members of this >= corresponding members of xother : true));

  // Exit:

  return result;
}

bool
tuple::
operator==(const tuple& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  result = true;

  for (unsigned i = 0; result && i < _d; ++i)
  {
    if (_mem[i] != xother._mem[i])
    {
      result = false;
      break;
    }
  }

  // Postconditions:

  assert(unexecutable(result ? all members of this == corresponding members of xother : true));

  // Exit:

  return result;
}

bool
tuple::
operator>(const tuple& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  result = true;

  for (unsigned i = 0; result && i < _d; ++i)
  {
    if (_mem[i] <= xother._mem[i])
    {
      result = false;
      break;
    }
  }

  // Postconditions:

  assert(unexecutable(result ? all members of this > corresponding members of xother : true));

  // Exit:

  return result;
}

bool
tuple::
operator<=(const tuple& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  result = true;

  for (unsigned i = 0; result && i < _d; ++i)
  {
    if (_mem[i] > xother._mem[i])
    {
      result = false;
      break;
    }
  }

  // Postconditions:

  assert(unexecutable(result ? all members of this <= corresponding members of xother : true));

  // Exit:

  return result;
}

bool
tuple::
operator<(const tuple& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  result = true;

  for (unsigned i = 0; result && i < _d; ++i)
  {
    if (_mem[i] >= xother._mem[i])
    {
      result = false;
      break;
    }
  }

  // Postconditions:

  assert(unexecutable(result ? all members of this < corresponding members of xother : true));

  // Exit:

  return result;
}

ostream&
operator<<(ostream& xos, const tuple& xtuple)
{
  // Preconditions:

  // Body:

  for (unsigned i = 0; i < xtuple._d; ++i)
  {
    xos << *(xtuple._mem+i)
	<< ' ';
  }

  // Postconditions:

  // Exit:

  return xos;
}

