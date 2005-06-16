#include "contract.h"
#include "matrix_writer.h"

#include "matrix.h"

matrix_writer::
matrix_writer(const temp_file& xfile, const matrix& xmat, hid_t xtype, hid_t xcreate_plist) :
  _mat(xmat)
{
  // Preconditions:

  assert(xfile.is_attached());
  assert(H5Iget_type(xtype) == H5I_DATATYPE);
  assert(xcreate_plist == H5P_DEFAULT || H5Pget_class(xcreate_plist) == H5P_DATASET_CREATE);

  // Body:

  extent e(2);

  xmat.get_extent(e);

  // If we're writing to a chunked dataset, make it extendible by specifying
  // an unlimited maximum size in each dimension of the dataspace.

  if (xcreate_plist != H5P_DEFAULT)
  {
    if (H5Pget_layout(xcreate_plist) == H5D_CHUNKED)
    {
      e.max_size()[0] = H5S_UNLIMITED;
      e.max_size()[1] = H5S_UNLIMITED;
    }
  }

  _file_space = H5Screate_simple(2, &e.size()[0], &e.max_size()[0]);

  // The memory space is meant to represent the first _ct columns or rows (depending
  // on the value of _access) stored contiguously.  Hence the memory space is 1 dimensional.
  // At this point we don't know the size of the space, since clients may call
  // set_access().  So we make it unlimited in size.

  hsize_t dims     = 1;
  hsize_t max_dims = H5S_UNLIMITED;

  _mem_space = H5Screate_simple(1, &dims, &max_dims); 

  _dataset = H5Dcreate(xfile.hid(),
		       "tmp",
		       xtype,
		       _file_space,
		       xcreate_plist);

  // Default access pattern.  It can be modified by a subsequent call to
  // set_access().

  _ct = 1;
  _access = BY_ROWS;

  // Postconditions:

  assert(invariant());

  // Exit:
}

matrix_writer::
~matrix_writer()
{
  // Preconditions:

  // Body:

  H5Idec_ref(_dataset);
  H5Idec_ref(_file_space);
  H5Idec_ref(_mem_space);

  // Postconditions:

  // Exit:
}

bool
matrix_writer::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:
  
  result = partial::invariant();
  result = result && (H5Iget_type(_dataset) == H5I_DATASET);
  result = result && (_ct > 0);
  result = result && (_access == BY_ROWS ? _ct <= _mat.row_ct() : _ct <= _mat.col_ct());
  result = result && (H5Sis_simple(_mem_space));
  result = result && (H5Sget_simple_extent_ndims(_mem_space) == 1);
  result = result && (H5Sis_simple(_file_space));
  result = result && (H5Sget_simple_extent_ndims(_file_space) == 2);

  // Postconditions:

  // Exit:

  return result;
}

matrix_writer::
matrix_writer(const matrix_writer& xother) :
  _mat(xother._mat)
{
  not_implemented;
}

matrix_writer&
matrix_writer::
operator=(const matrix_writer& xother)
{
  not_implemented;
  return *this;
}

void
matrix_writer::
start()
{
  // Preconditions:

  // Body:

  hyperslab h(2);

  if (_access == BY_ROWS)
  {
    _mat.select_rows(0, _ct, h);
  }
  else
  {
    _mat.select_cols(0, _ct, h);
  }

  // The file space selection is just the first _ct rows or columns.

  H5Sselect_hyperslab(_file_space,
		      H5S_SELECT_SET,
		      &h.origin()[0],
		      &h.stride()[0],
		      &h.ct()[0],
		      &h.block_size()[0]);

  // The memory space selection is the same size as the file space
  // selection, but it's guaranteed to be a contiguous 1-d space

  hsize_t dims = H5Sget_select_npoints(_file_space);
  hsize_t max_dims = H5S_UNLIMITED;

  H5Sset_extent_simple(_mem_space, 1, &dims, &max_dims);
  H5Sselect_all(_mem_space);

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
matrix_writer::
next()
{
  not_implemented;
}

bool
matrix_writer::
is_done() const
{
  bool result;

  not_implemented;

  return result;
}

double::
matrix_writer::
do_partial_io()
{
  double result;

  not_implemented;

  return result;
}

void
matrix_writer::
set_access(unsigned xct, access xaccess)
{
  // Preconditions:

  assert(xct > 0);
  assert(xaccess == BY_ROWS ? xct <= _mat.row_ct() : xct <= _mat.col_ct());

  // Body:

  _ct = xct;
  _access = xaccess;

  // Postconditions:

  assert(invariant());

  // Exit:
}
