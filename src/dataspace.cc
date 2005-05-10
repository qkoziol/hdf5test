#include "dataspace.h"
#include "contract.h"

dataspace::
dataspace()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:
}

dataspace::
dataspace(const dataspace& xother)
{
  not_implemented;
}

dataspace::
dataspace(const tuple& xsize, const tuple& xmax_size)
{
  // Preconditions:

  assert(xsize.d() == xmax_size.d());
  assert(xsize.is_positive());
  assert(xmax_size >= xsize);

  // Body:

  _hid = H5Screate_simple(xsize.d(), &xsize[0], &xmax_size[0]);

  // Postconditions:

  assert(invariant());
  assert(is_attached());
  assert(d() == xsize.d());

  // Exit:
}

dataspace::
~dataspace()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
dataspace::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = hdf5_handle::invariant();
  if (result && is_attached())
  {
    result = (d() >= 0);
  }

  // Postconditions:

  // Exit:

  return result;
}

extent&
dataspace::
get_extent() const
{
  extent* ptr_to_result;

  // Preconditions:

  assert(is_attached());

  // Body:

  int dim = H5Sget_simple_extent_ndims(_hid);

  ptr_to_result = new extent(dim);

  if (dim > 0)
  {
    H5Sget_simple_extent_dims(_hid, &ptr_to_result->size()[0], &ptr_to_result->max_size()[0]);
  }

  // Postconditions:

  // Exit:

  return *ptr_to_result;
}

int
dataspace::
d() const
{
  int result;

  // Preconditions:

  assert(is_attached());

  // Body:

  result = H5Sget_simple_extent_ndims(_hid);

  // Postconditions:

  assert(result >= 0);

  // Exit:

  return result;
}

void
dataspace::
select(const hyperslab& xsel)
{
  not_implemented;
}

void
dataspace::
select(dataspace::selection xsel)
{
  not_implemented;
}

dataspace::
dataspace(hid_t xother)
{
  // Precondition:

  assert(H5Iget_type(xother) == H5I_DATASPACE);

  // Body:

  int dim = H5Sget_simple_extent_ndims(xother);

  if (dim > 0)
  {
    extent e(dim);

    H5Sget_simple_extent_dims(xother, &e.size()[0], &e.max_size()[0]);

    _hid = H5Screate_simple(dim, &e.size()[0], &e.max_size()[0]);
  }
  else
  {
    _hid = H5Screate(H5S_SCALAR);
  }

  // Postconditions:

  assert(invariant());
  assert(d() == H5Sget_simple_extent_ndims(xother));

  // Exit:
}
