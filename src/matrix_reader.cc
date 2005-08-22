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

  // Postconditions:

  assert(invariant());
  assert(ct() == 0);
  assert(per_transfer_ct() == 1);
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
  
  result = matrix_io::invariant();

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
write_col_heading() const
{
  // Preconditions:

  // Body:

  // Write out a header.

  std::cout << "Reading "
	    << _max_ct;

  if (_by_rows)
    std::cout << " rows";
  else
    std::cout << " columns";

  std::cout << " of "
	    << *_mat
	    << " matrix at a time.\n"
	    << *_dset
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
	    << "bytes read (kB)";
  if (_dset->is_chunked())
  {
    std::cout << std::setw(25)
	      << "select/extend time (ms)";
  }
  std::cout << std::setw(16)
	    << "read time (ms)"
	    << std::setw(16)
	    << "io rate (mB/s)\n";

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
	      << _accum_ct-_cur_ct+1
	      << " to "
	      << std::setw(3)
	      << _accum_ct
	      << std::fixed << std::setprecision(3) << std::setw(15)
	      << _perf.bytes/((double)BYTES_PER_KB)
	      << "  ";
    if (_dset->is_chunked())
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
