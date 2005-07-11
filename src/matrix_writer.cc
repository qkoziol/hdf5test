#include "contract.h"
#include "matrix_writer.h"

#include "config.h"
#include "matrix.h"
#include "std_iomanip.h"
#include "std_iostream.h"

#define MILLI_PER_MICRO ((double)1000)

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

  e.size()[0] = xmat.row_ct();
  e.size()[1] = xmat.col_ct();

  // If we're writing to a chunked dataset, make it extendable by specifying
  // an unlimited maximum size in each dimension of the dataspace.

  if (xcreate_plist != H5P_DEFAULT)
  {
    if (H5Pget_layout(xcreate_plist) == H5D_CHUNKED)
    {
      e.max_size()[0] = H5S_UNLIMITED;
      e.max_size()[1] = H5S_UNLIMITED;
    }
  }
  else
  {
    e.max_size()[0] = e.size()[0];
    e.max_size()[1] = e.size()[1]; 
  }

  hid_t file_space = H5Screate_simple(2, &e.size()[0], &e.max_size()[0]);

  assert(file_space >= 0);

  hid_t id = H5Dcreate(xfile.hid(),
		       "matrix_writer",
		       xtype,
		       file_space,
		       xcreate_plist);

  assert(id >= 0);

  _dataset.attach(id);

  H5Sclose(file_space);

  // Default access pattern.  It can be modified by a subsequent call to
  // set_access().

  _max_write_ct = 1;
  _cur_write_ct = 1;
  _access       = BY_ROWS;
  _accum_ct     = 0;
  _type         = H5Tcopy(xtype);

  assert(_type >= 0);

  // Postconditions:

  assert(invariant());
  assert(ct() == 0);
  assert(per_write_ct() == 1);

  // Exit:
}

matrix_writer::
~matrix_writer()
{
  // Preconditions:

  // Body:

  _dataset.detach();
  _mem.detach();
  H5Tclose(_type);

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
  result = result && (is_done() ? _accum_ct > 0 : true);
  result = result && (_access == BY_ROWS ? _max_write_ct <= _mat.row_ct() : _max_write_ct <= _mat.col_ct());
  result = result && (_max_write_ct >= _cur_write_ct);
  result = result && (_cur_write_ct > 0);

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

  // Select the first _cur_write_ct rows or columns of
  // the dataset's dataspace as the write destination.
 
  hyperslab h(2);

  _accum_ct = 0;
  _cur_write_ct = _max_write_ct;

  if (_access == BY_ROWS)
  {
    _mat.select_rows(0, _cur_write_ct, h);
  }
  else
  {
    _mat.select_cols(0, _cur_write_ct, h);
  }

  _dataset.get_space().select(h);

  // Reserve sufficient memory to hold the first _cur_write_ct
  // rows or columns as the write source.  Make sure the entire
  // memory space is selected.

  _mem.reserve(_dataset);

  herr_t status = H5Sselect_all(_mem.get_space().hid());

  assert(status >= 0);

  // Make sure the timer is in reset mode so we can time all the
  // individual writes.  Make sure the time is zeroed.

  _timer.put_mode(timer::RESET);

  _timer.reset();

  // Write out a header.

  cout << _dataset
       << '\n'
       << setw(11)
       << "status   ";

  if (_access == BY_ROWS)
  {
    cout << setw(12)
	 << "rows    ";
  }
  else
  {
    cout << setw(12)
	 << "columns   ";
  }
  cout << setw(20)
       << "bytes written (kb)"
       << setw(19)
       << "elapsed time (ms)"
       << setw(16)
       << "io rate (mb/s)\n";

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
matrix_writer::
next()
{
  // Preconditions:

  // Body:

  // Protecting all the machinery of next() with this if
  // statement might seem a little strange, but there's a
  // method to this madness.  After the last piece of the
  // matrix is written, but before next() is called, is_done()
  // is true.  It makes no sense whatever to try to advance the
  // matrix selection beyond its boundaries, yet the standard
  // C/C++ loop structure in partial::run_test() requires next()
  // to be called even though is_done() is true.  So in that
  // case we make next() a no-op.

  if (! is_done())
  {
    // Write the minimum of the number of not-yet-written
    // rows/columns and the standard number of rows/columns
    // to be written.  Only on the last iteration, and only
    // if the number of rows/columns is not evenly divisible
    // by the standard row/column write count should the
    // size of the selection change.

    unsigned unwritten;

    if (_access == BY_ROWS)
    {
      unwritten = _mat.row_ct()-_accum_ct;
    }
    else
    {
      unwritten = _mat.col_ct()-_accum_ct;
    }

    _cur_write_ct = _max_write_ct;

    if (_cur_write_ct > unwritten)
    {
      _cur_write_ct = unwritten;
    }

    hyperslab h(2);

    if (_access == BY_ROWS)
    {
      _mat.select_rows(_accum_ct, _cur_write_ct, h);
    }
    else
    {
      _mat.select_cols(_accum_ct, _cur_write_ct, h);
    }

    _dataset.get_space().select(h);
    _mem.get_space().select(h.npoints());
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

bool
matrix_writer::
is_done() const
{
  bool result;

  // Preconditions:

  // Body:

  unsigned n;

  if (_access == BY_ROWS)
  {
    n = _mat.row_ct();
  }
  else
  {
    n = _mat.col_ct();
  }

  if (_accum_ct == n)
  {
    result = true;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

bool::
matrix_writer::
do_partial_io()
{
  bool result;

  // Preconditions:

  assert(! is_done());
  assert(_dataset.is_attached());
  assert(H5Sget_select_npoints(_dataset.get_space().hid()) == H5Sget_select_npoints(_mem.get_space().hid()));

  // Body:

  unsigned old_ct = ct();

  _timer.start();

  herr_t status = H5Dwrite(_dataset.hid(),
			   _dataset.get_type(),
			   _mem.get_space().hid(),
			   _dataset.get_space().hid(),
			   H5P_DEFAULT,
			   _mem.mem());

  _timer.stop();

  if (status >= 0)
  {
    double kb      = H5Sget_select_npoints(_mem.get_space().hid())*H5Tget_size(_dataset.get_type())/((double)BYTES_PER_KB);
    double elapsed = _timer.elapsed();

    cout << setw(11)
	 << "succeeded "
	 << setw(3)
	 << _accum_ct
	 << " to "
	 << setw(3)
	 << _accum_ct+_cur_write_ct-1
	 << fixed << setprecision(3) << setw(18)
	 << kb
	 << "  "
	 << fixed << setprecision(3) << setw(16)
	 << elapsed*MILLI_PER_MICRO
	 << setw(16)
	 << kb/elapsed/((double)BYTES_PER_KB)
	 << endl;

    _accum_ct += _cur_write_ct;
    result = true;
  }
  else
  {
    cout << setw(11)
	 << "failed   "
	 << setw(3)
	 << _accum_ct
	 << " to "
	 << setw(3)
	 << _accum_ct+_cur_write_ct-1
	 << endl;

    result = false;
  }

  // Postconditions:

  assert(result ? ct() == old_ct + _cur_write_ct : ct() == old_ct);

  // Exit:

  return result;
}

void
matrix_writer::
set_access(unsigned xper_write_ct, access xaccess)
{
  // Preconditions:

  assert(xper_write_ct > 0);
  assert(xaccess == BY_ROWS ? xper_write_ct <= _mat.row_ct() : xper_write_ct <= _mat.col_ct());

  // Body:

  _max_write_ct = xper_write_ct;
  _access       = xaccess;

  // Postconditions:

  assert(invariant());

  // Exit:
}

unsigned
matrix_writer::
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
matrix_writer::
per_write_ct() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _max_write_ct;

  // Postconditions:

  // Exit:

  return result;
}
