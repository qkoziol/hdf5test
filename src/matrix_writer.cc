#include "contract.h"
#include "matrix_writer.h"

#include "config.h"
#include <iomanip>
#include "matrix.h"
#include <ostream>

#define MILLI_PER_MICRO ((double)1000)

matrix_writer::
matrix_writer()
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

matrix_writer::
~matrix_writer()
{
  // Preconditions:

  // Body:

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
  
  result = matrix_io::invariant();

  // Postconditions:

  // Exit:

  return result;
}

matrix_writer::
matrix_writer(const matrix_writer& xother)
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
	      << std::fixed << std::setprecision(3) << std::setw(18)
	      << _perf.bytes/((double)BYTES_PER_KB)
	      << "  ";
    if (_dset->is_chunked())
    {
      std::cout << std::fixed << std::setprecision(3) << std::setw(20)
		<< _perf.extend*MILLI_PER_MICRO
		<< "     ";
    }
    std::cout << std::fixed << std::setprecision(3) << std::setw(14)
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

void
matrix_writer::
write_col_heading() const
{
  // Preconditions:

  // Body:

  // Write out a header.

  std::cout << "Writing "
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
  std::cout << std::setw(20)
	    << "bytes written (kB)";
  if (_dset->is_chunked())
  {
    std::cout << std::setw(25)
	      << "select/extend time (ms)";
  }
  std::cout << std::setw(17)
	    << "write time (ms)"
	    << std::setw(16)
	    << "io rate (mB/s)\n";

  // Postconditions:

  // Exit:
}
