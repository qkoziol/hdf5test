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

  result = _d >= 0;
  result = result && _d > 0 ? _mem != 0 : _mem == 0;

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
  for (int i = 0; i < _d; ++i)
    _mem[i] = xother._mem[i];

  // Postconditions:

  assert(invariant());
  assert(d() == xother.d());
  assert(unexecutable(foreach 0 <= i < d(); this->[i] == xother[i]));

  // Exit:
}
tuple::
tuple(int xdim)
{
  // Preconditions:

  assert(xdim >= 0);

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
operator[](int xindex)
{
  hsize_t* ptr_to_result;

  // Preconditions:

  assert(xindex >= 0);
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
operator[](int xindex) const
{
  const hsize_t* ptr_to_result;

  // Preconditions:

  assert(xindex >= 0);
  assert(xindex < d());

  // Body:

  ptr_to_result = _mem+xindex;

  // Postconditions:

  // Exit:

  return *ptr_to_result;
}

int
tuple::
d() const
{
  int result;

  // Preconditions:

  // Body:

  result = _d;

  // Postconditions:

  // Exit:

  return result;
}

void
tuple::
reserve(int xdim)
{
  // Preconditions:

  assert(xdim > 0);

  // Body:

  if (_mem != 0)
  {
    delete [] _mem;
  }

  _mem = new hsize_t[xdim];

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
  for (int i = 0; i < _d; ++i)
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

  for (int i = 0; i < _d; ++i)
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

  for (int i = 0; result && i < _d; ++i)
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

  for (int i = 0; result && i < _d; ++i)
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

  for (int i = 0; result && i < _d; ++i)
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

  for (int i = 0; result && i < _d; ++i)
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

  for (int i = 0; result && i < _d; ++i)
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
