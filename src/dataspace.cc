#include "dataspace.h"
#include "contract.h"

#include "hyperslab.h"
#include "matrix.h"

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

  assert(_hid >= 0);

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
    result = (_ext.d() == d());
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
  assert(d() > 0);

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

  herr_t status = H5Sselect_hyperslab(_hid,
		                      xop,
		                      &xsubset.start()[0],
		                      &xsubset.stride()[0],
		                      &xsubset.ct()[0],
		                      &xsubset.block_size()[0]);

  assert(status >= 0);

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

  herr_t status = H5Sselect_all(_hid);

  assert(status >= 0);

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

  herr_t status = H5Sselect_none(_hid);

  assert(status >= 0);

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

  assert(dim >= 0);

  if (dim > 0)
  {
    _ext.reserve(dim);

    int status = H5Sget_simple_extent_dims(xother, &_ext.size()[0], &_ext.max_size()[0]);

    assert(status >= 0);

    _hid = H5Screate_simple(dim, &_ext.size()[0], &_ext.max_size()[0]);

    assert(_hid >= 0);

    set_extent();
  }
  else
  {
    _hid = H5Screate(H5S_SCALAR);

    assert(_hid >= 0);
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

  assert(dim >= 0);

  _ext.reserve(dim);

  if (dim > 0)
  {
    int status = H5Sget_simple_extent_dims(_hid, &_ext.size()[0], &_ext.max_size()[0]);

    assert(status >= 0);
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

  if (is_attached())
    detach();

  hdf5_handle::attach(xhid);

  set_extent();

  // Postconditions:

  assert(invariant());
  assert(is_attached());

  // Exit:
}

std::ostream&
operator<<(std::ostream& xos, const dataspace& xspace)
{
  // Preconditions:

  assert(xos.good());

  // Body:

  unsigned d = xspace.d();

  xos << "d = "
      << d;

  if (d > 0)
  {
    xos << " extent = {"
	<< xspace.get_extent()
	<< '}';
  }

  // Postconditions:

  // Exit:

  return xos;
}

void
dataspace::
select(hsize_t xnpts)
{
  // Preconditions:

  assert(is_attached());
  assert(d() == 1);

  // Body:

  hyperslab h(1);

  h.ct()[0] = xnpts;

  select(h);

  // Postconditions:

  // Exit:

}

dataspace&
dataspace::
operator=(const dataspace& xother)
{
  // Preconditions:

  // Body:

  _ext = xother._ext;
  _hid = H5Scopy(xother._hid);

  // Postconditions:

  assert(invariant());

  // Exit:

  return *this;
}

bool
dataspace::
is_congruent(const matrix& xmat) const
{
  bool result;

  // Preconditions:

  // Body:

  not_implemented;

  // HACK:
  // The following just keeps the compiler from complaining
  // that result is uninitialized.

  result = false;

  // Postconditions:

  // Exit:

  return result;
}

bool
dataspace::
is_subset_congruent(const matrix& xmat, unsigned xct, bool xby_rows) const
{
  bool result;

  // Preconditions:

  // Body:

  // We're trying to determine whether a dataspace matches the 1st xct
  // rows or columns of a matrix.  It does if the dataspace is 2 dimensional,
  // and if the length of a row (if xby_rows) or a column (if !xby_rows)
  // is the same length as the corresponding row/col of the matrix and
  // if there are at least xct rows/cols in the matrix and dataspace.

  result = false;

  if (is_attached())
  {
    const extent& e = get_extent();

    if (e.d() == 2)
    {
      // Then the dataspace is 2-d, same as a matrix,

      if (xby_rows)
      {
	if (xmat.row_ct() >= xct)
	{
	  // and the matrix has at least xct rows,

	  if (e.size(1) == xmat.col_ct())
	  {
	    // and the length of the dataspace's row matches the length of the matrix row.

	    result = true;
	  }
	}
      }
      else
      {
	if (xmat.col_ct() >= xct)
	{
	  // and the matrix has at least xct columns,

	  if (e.size(0) == xmat.row_ct())
	  {
	    // and the length of the dataspace's column matches the length of the matrix column.

	    result = true;
	  }
	}
      }
    }
  }

  // Postconditions:

  // Exit:

  return result;
}

hsize_t
dataspace::
get_select_npoints() const
{
  hsize_t result;

  // Preconditions:

  assert(is_attached());

  // Body:

  result = H5Sget_select_npoints(_hid);

  // Postconditions:

  // Exit:

  return result;
}
