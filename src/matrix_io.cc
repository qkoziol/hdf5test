#include "contract.h"
#include "matrix_io.h"

#include "config.h"
#include <iomanip>
#include "matrix.h"
#include <ostream>

#define MILLI_PER_MICRO ((double)1000)

matrix_io::
matrix_io()
{
  // Preconditions:

  // Body:

  // Default access pattern.  It can be modified by a subsequent call to
  // set_access().

  _max_ct   = 1;
  _cur_ct   = 1;
  _by_rows  = true;
  _accum_ct = 0;
  _mat      = 0;
  _dset     = 0;

  _size.reserve(2);
  _size = 0;

  _max_size.reserve(2);
  _max_size = H5S_UNLIMITED;

  // Postconditions:

  assert(invariant());
  assert(ct() == 0);
  assert(per_transfer_ct() == 1);
  assert(access_is_by_rows());

  // Exit:
}

matrix_io::
~matrix_io()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
matrix_io::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:
  
  result = partial::invariant();
  result = result && (is_done() ? _accum_ct > 0 : true);
  result = result && (_max_ct >= _cur_ct);
  result = result && (_cur_ct > 0);
  result = result && (_size.d() == 2);

  // Postconditions:

  // Exit:

  return result;
}

matrix_io::
matrix_io(const matrix_io& xother)
{
  not_implemented;
}

matrix_io&
matrix_io::
operator=(const matrix_io& xother)
{
  not_implemented;
  return *this;
}

void
matrix_io::
start()
{
  // Preconditions:

  // Body:

  // Write out a header.  Descendant classes implement this.

  write_col_heading();

  // We haven't transferred any rows/cols yet.

  _accum_ct = 0;

  // Make sure the timer is in reset mode so we can time all the
  // individual transfers.  Make sure the time is zeroed.

  _timer.put_mode(timer::RESET);

  _timer.reset();

  // Select the first _cur_ct rows or columns of
  // the dataset's dataspace to participate in the data transfer.

  _timer.start();
 
  select_extend();

  _timer.stop();

  _perf.extend = _timer.elapsed();

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
matrix_io::
next()
{
  // Preconditions:

  // Body:

  // Protecting all the machinery of next() with this if
  // statement might seem a little strange, but there's a
  // method to this madness.  After the last piece of the
  // matrix is transferred, but before next() is called, is_done()
  // is true.  It makes no sense whatever to try to advance the
  // matrix selection beyond its boundaries, yet the standard
  // C/C++ loop structure in partial::run_test() requires next()
  // to be called even though is_done() is true.  So in that
  // case we make next() a no-op.

  if (! is_done())
  {
    _timer.start();

    select_extend();

    _timer.stop();

    _perf.extend = _timer.elapsed();
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

bool
matrix_io::
is_done() const
{
  bool result;

  // Preconditions:

  // Body:

  unsigned n;

  if (_mat == 0)
  {
    // is_done() can only be true if we're in the midst of run_test(..)
    // and that's the only place where _mat != 0;

    result = false;
  }
  else
  {
    if (_by_rows)
    {
      n = _mat->row_ct();
    }
    else
    {
      n = _mat->col_ct();
    }

    if (_accum_ct == n)
    {
      result = true;
    }
    else
    {
      result = false;
    }
  }

  // Postconditions:

  // Exit:

  return result;
}

bool::
matrix_io::
do_partial_io()
{
  bool result;

  // Preconditions:

  assert(! is_done());
  assert(_dset->is_attached());
  assert(H5Sget_select_npoints(_dset->get_space().hid()) == H5Sget_select_npoints(_mem->get_space().hid()));

  // Body:

  unsigned old_ct = ct();

  _timer.start();

  herr_t err;

  // TODO:
  // I think we have the ability to transfer between memory and pcontainer built into
  // some class, which should eliminate the need for this HDF5 code.  Upgrade to that
  // ability.

  if (_is_writer)
  {
    err = H5Dwrite(_dset->hid(),
		   _dset->get_type(),
		   _mem->get_space().hid(),
		   _dset->get_space().hid(),
		   H5P_DEFAULT,
		   _mem->mem());
  }
  else
  {
    err = H5Dread(_dset->hid(),
		  _mem->get_type(),
		  _mem->get_space().hid(),
		  _dset->get_space().hid(),
		  H5P_DEFAULT,
		  _mem->mem());
  }

  _timer.stop();

  if (err >= 0)
  {
    _perf.bytes   = H5Sget_select_npoints(_mem->get_space().hid())*H5Tget_size(_dset->get_type());

    _perf.elapsed = _timer.elapsed();

    _accum_ct += _cur_ct;

    _status = SUCCESS;

    result = true;
  }
  else
  {
    _status = FAILURE;

    result = false;
  }

  // Postconditions:

  assert(result ? ct() == old_ct + _cur_ct : ct() == old_ct);
  assert(status() != NOT_RUN);
  assert(result ? status() == SUCCESS : status() == FAILURE);

  // Exit:

  return result;
}

void
matrix_io::
set_access(unsigned xper_transfer_ct, bool xby_rows)
{
  // Preconditions:

  assert(xper_transfer_ct > 0);

  // Body:

  _max_ct  = xper_transfer_ct;
  _cur_ct  = xper_transfer_ct;
  _by_rows = xby_rows;

  // Postconditions:

  assert(invariant());
  assert(access_is_by_rows() == xby_rows);
  assert(per_transfer_ct() == xper_transfer_ct);

  // Exit:
}

unsigned
matrix_io::
ct() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _accum_ct;

  // Postconditions:

  // Exit:

  return result;
}

unsigned
matrix_io::
per_transfer_ct() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _max_ct;

  // Postconditions:

  // Exit:

  return result;
}

bool
matrix_io::
run_test(matrix& xmat, memory& xsrc, dataset& xdest)
{
  bool result;

  // Preconditions:

  assert(access_is_by_rows() ? xmat.row_ct() >= per_transfer_ct() : xmat.col_ct() >= per_transfer_ct());
  assert(xdest.is_attached());
  assert(xdest.get_space().d() == 2);
  // ISSUE:
  // Is any condition necessary for mem?  We'll resize it as necessary.
  //assert(xsrc.is_attached() ? xsrc.get_space().get_select_npoints() == per_transfer_npoints() : true);
  // HACK: commented out - too strict and memory dataspace is 1-d, anyway
  //assert(xsrc.is_attached() ? xsrc.get_space().is_subset_congruent(xmat, per_transfer_ct(), access_is_by_rows()) : true);

  // Body:

  _mat       = &xmat;
  _mem       = &xsrc;
  _dset      = &xdest;
  _is_writer = true;

  if (_by_rows)
  {
    _max_size[0] = H5S_UNLIMITED;
    _max_size[1] = xmat.col_ct();
  }
  else
  {
    _max_size[0] = xmat.row_ct();
    _max_size[1] = H5S_UNLIMITED;
  }

  result = run_test();

  _mat  = 0;
  _mem  = 0;
  _dset = 0;

  // Postconditions:

  assert(invariant());

  // Exit:

  return result;
}

bool
matrix_io::
run_test(matrix& xmat, dataset& xsrc, memory& xdest)
{
  bool result;

  // Preconditions:

  assert(access_is_by_rows() ? xmat.row_ct() >= per_transfer_ct() : xmat.col_ct() >= per_transfer_ct());
  assert(xdest.is_attached());
  assert(xsrc.get_space().d() == 2);
  // ISSUE:
  // Is any condition necessary for mem?  We'll resize it as necessary.
  //assert(xsrc.is_attached() ? xsrc.get_space().get_select_npoints() == per_transfer_npoints() : true);
  // HACK: commented out - too strict and memory dataspace is 1-d, anyway
  //assert(xsrc.is_attached() ? xsrc.get_space().is_subset_congruent(xmat, per_transfer_ct(), access_is_by_rows()) : true);

  // Body:

  // ISSUE:
  // This implementation is almost identical to the "write" version
  // of this function.  I'm bothered by the redundancy.  Does it indicate
  // a design problem?

  _mat       = &xmat;
  _mem       = &xdest;
  _dset      = &xsrc;
  _is_writer = false;

  if (_by_rows)
  {
    _max_size[0] = H5S_UNLIMITED;
    _max_size[1] = xmat.col_ct();
  }
  else
  {
    _max_size[0] = xmat.row_ct();
    _max_size[1] = H5S_UNLIMITED;
  }

  result = run_test();

  _mat  = 0;
  _mem  = 0;
  _dset = 0;

  // Postconditions:

  assert(invariant());

  // Exit:

  return result;
}

bool
matrix_io::
access_is_by_rows() const
{
  bool result;

  // Preconditions:

  // Body:

  result = _by_rows;

  // Postconditions:

  // Exit:

  return result;
}

unsigned
matrix_io::
per_transfer_npoints() const
{
  unsigned result;

  // Preconditions:

  // Body:

  if (_mat == 0)
  {
    result = 0;
  }
  else
  {
    if (_by_rows)
    {
      result = _cur_ct*_mat->col_ct();
    }
    else
    {
      result = _cur_ct*_mat->row_ct();
    }
  }

  // Postconditions:

  // Exit:

  return result;
}

void
matrix_io::
select_extend()
{
  // Preconditions:

  // Body:

  // Determine the number of rows/columns to transfer next time.

  // Compute the minimum of the number of not-yet-transferred
  // rows/columns and the standard number of rows/columns
  // to be transferred.  Only on the last iteration, and only
  // if the number of rows/columns is not evenly divisible
  // by the standard row/column transfer count should the
  // size of the selection change.

  unsigned not_transferred;

  if (_by_rows)
  {
    not_transferred = _mat->row_ct()-_accum_ct;
  }
  else
  {
    not_transferred = _mat->col_ct()-_accum_ct;
  }

  _cur_ct = _max_ct;

  if (_cur_ct > not_transferred)
  {
    _cur_ct = not_transferred;
  }


  // Now ensure that the dataset's extent encompasses the upcoming transfer.

  if (_dset->is_chunked())
  {
    // Define the size of the matrix on disk that will have been transferred after
    // the upcoming partial i/o operation.

    if (_by_rows)
    {
      _size[0] = _accum_ct + _cur_ct;
      _size[1] = _mat->col_ct();
    }
    else
    {
      _size[0] = _mat->row_ct();
      _size[1] = _accum_ct + _cur_ct;
    }

    // Ask HDF5 to extend the dataset's dataspace to the desired size.

    herr_t status = H5Sset_extent_simple(_dset->get_space().hid(), 2, &_size[0], &_max_size[0]);

    assert(status >= 0);

    status = H5Dextend(_dset->hid(), &_size[0]);

    assert(status >= 0);
  }

  // Select the next group of rows/columns in the dataset's dataspace.

  hyperslab h(2);

  if (_by_rows)
  {
    _mat->select_rows(_accum_ct, _cur_ct, h);
  }
  else
  {
    _mat->select_cols(_accum_ct, _cur_ct, h);
  }

  _dset->get_space().select(h);

  // Make sure the memory buffer is big enough.

  _mem->reserve(*_dset);

  _mem->get_space().select(h.npoints());

  // Postconditions:

  // Exit:

}
