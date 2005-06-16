#include "dataspace.h"
#include "contract.h"

#include "hyperslab.h"

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
  set_extent();

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
    result = result && (_ext.d() == d());
  }

  // Postconditions:

  // Exit:

  return result;
}

const extent&
dataspace::
get_extent() const
{
  // Preconditions:

  assert(is_attached());

  // Body:

  // Postconditions:

  // Exit:

  return _ext;
}

unsigned
dataspace::
d() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _ext.d();

  // Postconditions:

  // Exit:

  return result;
}

void
dataspace::
select(const hyperslab& xsubset, H5S_seloper_t xop)
{
  // Preconditions:

  assert(xsubset.d() == d());
  assert(is_attached());

  // Body:

  H5Sselect_hyperslab(_hid,
		      xop,
		      &xsubset.origin()[0],
		      &xsubset.stride()[0],
		      &xsubset.ct()[0],
		      &xsubset.block_size()[0]);

  // Postconditions:

  assert(invariant());

  // Exit:

}

void
dataspace::
select_all()
{
  // Preconditions:

  assert(is_attached());

  // Body:

  H5Sselect_all(_hid);

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
dataspace::
select_none()
{
  // Preconditions:

  assert(is_attached());

  // Body:

  H5Sselect_none(_hid);

  // Postconditions:

  assert(invariant());

  // Exit:
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
    _ext.reserve(dim);

    H5Sget_simple_extent_dims(xother, &_ext.size()[0], &_ext.max_size()[0]);

    _hid = H5Screate_simple(dim, &_ext.size()[0], &_ext.max_size()[0]);

    set_extent();
  }
  else
  {
    _hid = H5Screate(H5S_SCALAR);
  }

  // Postconditions:

  assert(invariant());
  assert(d() == static_cast<unsigned>(H5Sget_simple_extent_ndims(xother)));

  // Exit:
}

void
dataspace::
set_extent()
{
  // Preconditions:

  assert(is_attached());

  // Body:

  int dim = H5Sget_simple_extent_ndims(_hid);

  _ext.reserve(dim);

  if (dim > 0)
  {
    H5Sget_simple_extent_dims(_hid, &_ext.size()[0], &_ext.max_size()[0]);
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
dataspace::
attach(hid_t xhid)
{
  // Preconditions:

  assert(H5Iget_type(xhid) == H5I_DATASPACE);

  // Body:

  hdf5_handle::attach(xhid);

  set_extent();

  // Postconditions:

  assert(invariant());
  assert(is_attached());

  // Exit:
}
