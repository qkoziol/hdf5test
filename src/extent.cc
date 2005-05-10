#include "extent.h"
#include "contract.h"

extent::
extent()
{
  not_implemented;
}

extent::
extent(const extent& xother)
{
  not_implemented;
}

extent::
extent(int xdim)
{
  // Preconditions:

  assert(xdim >= 0);

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

  result = d() >= 0;
  result = result && max_size().d() == d();
  result = result && size().d() == d();

  // Postconditions:

  // Exit:

  return result;
}

int
extent::
d() const
{
  int result;

  // Preconditions:

  // Body:

  result = _size.d();

  // Postconditions:

  assert(result >= 0);

  // Exit:

  return result;
}

hsize_t
extent::
size(int xindex) const
{
  // Preconditions:

  assert(xindex >= 0);
  assert(xindex < d());

  // Body:

  const hsize_t result = _size[xindex];

  // Postconditions:

  // Exit:

  return result;
}

hsize_t
extent::
max_size(int xindex) const
{
  hsize_t result;

  // Preconditions:

  assert(xindex >= 0);
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
