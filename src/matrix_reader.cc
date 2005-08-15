#include "contract.h"
#include "matrix_reader.h"

#include "config.h"
#include <iomanip>
#include "matrix.h"
#include <ostream>

#define MILLI_PER_MICRO ((double)1000)

matrix_reader::
matrix_reader()
{
  // Preconditions:

  // Body:

  // Default access pattern.  It can be modified by a subsequent call to
  // set_access().

  _max_read_ct = 1;
  _cur_read_ct = 1;
  _by_rows     = true;
  _accum_ct    = 0;
  _mat         = 0;
  _src         = 0;

  _size.reserve(2);
  _size = 0;

  _max_size.reserve(2);
  _max_size = H5S_UNLIMITED;

  // Postconditions:

  assert(invariant());
  assert(ct() == 0);
  assert(per_read_ct() == 1);
  assert(access_is_by_rows());

  // Exit:
}

matrix_reader::
~matrix_reader()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
matrix_reader::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:
  
  result = partial::invariant();
  result = result && (is_done() ? _accum_ct > 0 : true);
  result = result && (_max_read_ct >= _cur_read_ct);
  result = result && (_cur_read_ct > 0);
  result = result && (_size.d() == 2);

  // Postconditions:

  // Exit:

  return result;
}

matrix_reader::
matrix_reader(const matrix_reader& xother)
{
  not_implemented;
}

matrix_reader&
matrix_reader::
operator=(const matrix_reader& xother)
{
  not_implemented;
  return *this;
}

void
matrix_reader::
start()
{
  // Preconditions:

  // Body:

  // Write out a header.

  std::cout << "Reading "
	    << _max_read_ct;

  if (_by_rows)
    std::cout << " rows";
  else
    std::cout << " columns";

  std::cout << " of "
	    << *_mat
	    << " matrix at a time.\n"
	    << *_src
	    << '\n'
	    << std::setw(11)
	    << "status   ";

  if (_by_rows)
  {
    std::cout << std::setw(12)
	      << "rows    ";
  }
  else
  {
    std::cout << std::setw(12)
	      << "columns   ";
  }
  std::cout << std::setw(17)
	    << "bytes read (kb)";
  if (_src->is_chunked())
  {
    std::cout << std::setw(25)
	      << "select/extend time (ms)";
  }
  std::cout << std::setw(16)
	    << "read time (ms)"
	    << std::setw(16)
	    << "io rate (mb/s)\n";

  // We haven't read any rows/cols yet.

  _accum_ct = 0;

  // Make sure the timer is in reset mode so we can time all the
  // individual reads.  Make sure the time is zeroed.

  _timer.put_mode(timer::RESET);

  _timer.reset();

  // Select the first _cur_read_ct rows or columns of
  // the dataset's dataspace as the read source.

  _timer.start();
 
  select_extend();

  _timer.stop();

  _perf.extend = _timer.elapsed();

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
matrix_reader::
next()
{
  // Preconditions:

  // Body:

  // Protecting all the machinery of next() with this if
  // statement might seem a little strange, but there's a
  // method to this madness.  After the last piece of the
  // matrix is read, but before next() is called, is_done()
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
matrix_reader::
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
matrix_reader::
do_partial_io()
{
  bool result;

  // Preconditions:

  assert(! is_done());
  assert(_src->is_attached());
  assert(H5Sget_select_npoints(_src->get_space().hid()) == H5Sget_select_npoints(_dest->get_space().hid()));

  // Body:

  unsigned old_ct = ct();

  _timer.start();

  herr_t err = H5Dread(_src->hid(),
		       _dest->get_type(),
		       _dest->get_space().hid(),
		       _src->get_space().hid(),
		       H5P_DEFAULT,
		       _dest->mem());

  _timer.stop();

  if (err >= 0)
  {
    _perf.bytes   = H5Sget_select_npoints(_dest->get_space().hid())*H5Tget_size(_src->get_type());

    _perf.elapsed = _timer.elapsed();

    _accum_ct += _cur_read_ct;

    _status = SUCCESS;

    result = true;
  }
  else
  {
    _status = FAILURE;

    result = false;
  }

  // Postconditions:

  assert(result ? ct() == old_ct + _cur_read_ct : ct() == old_ct);
  assert(status() != NOT_RUN);
  assert(result ? status() == SUCCESS : status() == FAILURE);

  // Exit:

  return result;
}

void
matrix_reader::
set_access(unsigned xper_read_ct, bool xby_rows)
{
  // Preconditions:

  assert(xper_read_ct > 0);

  // Body:

  _max_read_ct = xper_read_ct;
  _cur_read_ct = xper_read_ct;
  _by_rows     = xby_rows;

  // Postconditions:

  assert(invariant());
  assert(access_is_by_rows() == xby_rows);
  assert(per_read_ct() == xper_read_ct);

  // Exit:
}

unsigned
matrix_reader::
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
matrix_reader::
per_read_ct() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _max_read_ct;

  // Postconditions:

  // Exit:

  return result;
}

bool
matrix_reader::
run_test(matrix& xmat, dataset& xsrc, memory& xdest)
{
  bool result;

  // Preconditions:

  assert(access_is_by_rows() ? xmat.row_ct() >= per_read_ct() : xmat.col_ct() >= per_read_ct());
  assert(xsrc.is_attached());
  assert(xsrc.get_space().d() == 2);
  // ISSUE:
  // Is any condition necessary for mem?  We'll resize it as necessary.
  //assert(xsrc.is_attached() ? xsrc.get_space().get_select_npoints() == per_write_npoints() : true);
  // HACK: commented out - too strict and memory dataspace is 1-d, anyway
  //assert(xsrc.is_attached() ? xsrc.get_space().is_subset_congruent(xmat, per_read_ct(), access_is_by_rows()) : true);

  // Body:

  _mat  = &xmat;
  _dest = &xdest;
  _src  = &xsrc;
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
  _dest = 0;
  _src  = 0;

  // Postconditions:

  assert(invariant());

  // Exit:

  return result;
}

bool
matrix_reader::
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
matrix_reader::
per_read_npoints() const
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
      result = _cur_read_ct*_mat->col_ct();
    }
    else
    {
      result = _cur_read_ct*_mat->row_ct();
    }
  }

  // Postconditions:

  // Exit:

  return result;
}

void
matrix_reader::
select_extend()
{
  // Preconditions:

  // Body:

  // Determine the number of rows/columns to write next time.

  // Compute the minimum of the number of not-yet-written
  // rows/columns and the standard number of rows/columns
  // to be written.  Only on the last iteration, and only
  // if the number of rows/columns is not evenly divisible
  // by the standard row/column write count should the
  // size of the selection change.

  unsigned unwritten;

  if (_by_rows)
  {
    unwritten = _mat->row_ct()-_accum_ct;
  }
  else
  {
    unwritten = _mat->col_ct()-_accum_ct;
  }

  _cur_read_ct = _max_read_ct;

  if (_cur_read_ct > unwritten)
  {
    _cur_read_ct = unwritten;
  }


  // Now ensure that the dataset's extent encompasses the upcoming write.

  if (_src->is_chunked())
  {
    // Define the desired size of the matrix on disk after the upcoming
    // write.

    if (_by_rows)
    {
      _size[0] = _accum_ct + _cur_read_ct;
      _size[1] = _mat->col_ct();
    }
    else
    {
      _size[0] = _mat->row_ct();
      _size[1] = _accum_ct + _cur_read_ct;
    }

    // Ask HDF5 to extend the dataset's dataspace to the desired size.

    herr_t status = H5Sset_extent_simple(_src->get_space().hid(), 2, &_size[0], &_max_size[0]);

    assert(status >= 0);

    status = H5Dextend(_src->hid(), &_size[0]);

    assert(status >= 0);
  }

  // Select the next group of rows/columns in the dataset's dataspace.

  hyperslab h(2);

  if (_by_rows)
  {
    _mat->select_rows(_accum_ct, _cur_read_ct, h);
  }
  else
  {
    _mat->select_cols(_accum_ct, _cur_read_ct, h);
  }

  _src->get_space().select(h);

  // Make sure the memory buffer is big enough.

  _dest->reserve(*_src);

  _dest->get_space().select(h.npoints());

  // Postconditions:

  // Exit:

}

void
matrix_reader::
write_results()
{
  // Preconditions:

  assert(std::cout.good());

  // Body:

  if (status() == SUCCESS)
  {
    std::cout << std::setw(11)
	      << "succeeded "
	      << std::setw(3)
	      << _accum_ct-_cur_read_ct+1
	      << " to "
	      << std::setw(3)
	      << _accum_ct
	      << std::fixed << std::setprecision(3) << std::setw(15)
	      << _perf.bytes/((double)BYTES_PER_KB)
	      << "  ";
    if (_src->is_chunked())
    {
      std::cout << std::fixed << std::setprecision(3) << std::setw(20)
		<< _perf.extend*MILLI_PER_MICRO
		<< "     ";
    }
    std::cout << std::fixed << std::setprecision(3) << std::setw(13)
	      << _perf.elapsed*MILLI_PER_MICRO
	      << std::setw(16)
	      << _perf.bytes/_perf.elapsed/((double)BYTES_PER_KB)/((double)BYTES_PER_KB)
	      << std::endl;
  }
  else
  {
    std::cout << std::setw(11)
	      << "failed   "
	      << std::setw(3)
	      << _accum_ct
	      << " to "
	      << std::setw(3)
	      << _accum_ct-1
	      << std::endl;
  }

  // Postconditions:

  // Exit:
}
