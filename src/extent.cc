#include "extent.h"
#include "contract.h"

extent::
extent()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());
  assert(d() == 0);

  // Exit:
}

extent::
extent(const extent& xother)
{
  // Preconditions:

  // Body:

  _max_size = xother._max_size;
  _size = xother._size;

  // Postconditions:

  assert(invariant());
  assert(*this == xother);

  // Exit:
}

extent::
extent(unsigned xdim)
{
  // Preconditions:

  // Body:

  if (xdim > 0)
  {
    _max_size.reserve(xdim);
    _size.reserve(xdim);
  }

  // Postconditions:

  assert(invariant());
  assert(d() == xdim);

  // Exit:
}

extent::
~extent()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
extent::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = max_size().d() == d();
  result = result && size().d() == d();

  // Postconditions:

  // Exit:

  return result;
}

unsigned
extent::
d() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _size.d();

  // Postconditions:

  // Exit:

  return result;
}

hsize_t
extent::
size(unsigned xindex) const
{
  // Preconditions:

  assert(xindex < d());

  // Body:

  const hsize_t result = _size[xindex];

  // Postconditions:

  // Exit:

  return result;
}

hsize_t
extent::
max_size(unsigned xindex) const
{
  hsize_t result;

  // Preconditions:

  assert(xindex < d());

  // Body:

  result = _max_size[xindex];

  // Postconditions:

  // Exit:

  return result;
}

const tuple&
extent::
size() const
{
  const tuple* ptr_to_result;

  // Preconditions:

  // Body:

  ptr_to_result = &_size;

  // Postconditions:

  // Exit:

  return *ptr_to_result;
}

tuple&
extent::
size()
{
  tuple* ptr_to_result;

  // Preconditions:

  // Body:

  ptr_to_result = &_size;

  // Postconditions:

  // Exit:

  return *ptr_to_result;
}

const tuple&
extent::
max_size() const
{
  const tuple* ptr_to_result;

  // Preconditions:

  // Body:

  ptr_to_result = &_max_size;

  // Postconditions:

  // Exit:

  return *ptr_to_result;
}

tuple&
extent::
max_size()
{
  tuple* ptr_to_result;

  // Preconditions:

  // Body:

  ptr_to_result = &_max_size;

  // Postconditions:

  // Exit:

  return *ptr_to_result;
}

hsize_t
extent::
npoints() const
{
  hsize_t result;

  // Preconditions:

  // Body:

  int dim = _size.d();

  result = 1;
  for (int i = 0; i < dim; ++i)
    result *= _size[i];

  // ISSUE:
  // One could experience spurious results if the
  // product of the sizes in each dimension overflowed
  // an hsize_t.

  // Postconditions:

  // Exit:

  return result;
}

void
extent::
reserve(unsigned xdim)
{
  // Preconditions:

  // Body:

  _size.reserve(xdim);
  _max_size.reserve(xdim);

  // Postconditions:

  assert(invariant());

  // Exit:
}

bool
extent::
operator==(const extent& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  result = (_size == xother._size) && (_max_size == xother._max_size);
 
  // Postconditions:

  // Exit:

  return result;
}

ostream&
operator<<(ostream& xos, const extent& xextent)
{
  // Preconditions:

  assert(xos.good());

  // Body:

  xos << "size = "
      << xextent._size
      << ", max_size = "
      << xextent._max_size;

  // Postconditions:

  // Exit:

  return xos;
}

extent&
extent::
operator=(const extent& xother)
{
  // Preconditions:

  // Body:

  _size = xother._size;
  _max_size = xother._max_size;

  // Postconditions:

  assert(invariant());
  assert(d() == xother.d());
  // TODO:
  // express the fact that the values of _max_size and _size are identical to those of xother.

  // Exit:

  return *this;
}
