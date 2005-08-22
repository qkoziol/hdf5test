#include "dft_read_perf.h"
#include "contract.h"

#include "attribute.h"
#include "config.h"
#include "dataset.h"
#include "dft_namelen.h"
#include <iomanip>
#include <ostream>
#include "pcontainer.h"

dft_read_perf::
dft_read_perf()
{
  // Preconditions:

  // Body:

  _success_ct = 0;
  _failure_ct = 0;
  _longest    = 0;
  _verbose    = false;

  // Postconditions:

  assert(invariant());
  assert(! is_verbose());

  // Exit:

}

dft_read_perf::
~dft_read_perf()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
dft_read_perf::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = dft::invariant();

  // Postconditions:

  // Exit:

  return result;
}

dft_read_perf::
dft_read_perf(const dft_read_perf& xother)
{
  not_implemented;
}

dft_read_perf&
dft_read_perf::
operator=(const dft_read_perf& xother)
{
  not_implemented;
  return *this;  // keeps pgi compiler happy until this is implemented.
}

void
dft_read_perf::
preorder_action()
{
  // Preconditions:

  // Body:

  H5I_type_t type = H5Iget_type(current_hid());

  if (type == H5I_DATASET || type == H5I_ATTR)
  {
    pcontainer& src(dynamic_cast<pcontainer&>(current()));

    _dest.attach(src.get_type());

    _dest.reserve(src);

    _tester.run_test(src, _dest);

    if (_tester.status() == test::SUCCESS)
      ++_success_ct;
    else
      ++_failure_ct;

    std::cout << std::setw(_longest+2)
	      << std::left
	      << name(true)
	      << std::setw(11)
	      << std::left
	      << src.type_name();

    if (_tester.status() == test::SUCCESS)
    {
      double kB      = _tester.bytes()/((double)BYTES_PER_KB);
      double elapsed = _tester.elapsed();

      std::cout << std::setw(13) << std::left
		<< " succeeded"
		<< std::setw(15) << std::right << std::fixed << std::setprecision(3)
		<< kB
		<< std::setw(19) << std::right << std::fixed << std::setprecision(3)
		<< elapsed*BYTES_PER_KB
		<< "         "
		<< std::setw(13) << std::right << std::fixed << std::setprecision(3)
		<< kB/elapsed/((double)BYTES_PER_KB)
		<< "     ";

      if (_verbose)
      {
	dataset* ds = dynamic_cast<dataset*>(&src);

	if (ds != 0)
	{
	  std::cout << *ds;
	}
	else
	{
	  attribute* attr = dynamic_cast<attribute*>(&src);

	  std::cout << *attr;
	}
      }
      std::cout << '\n';
    }
    else
    {
      std::cout << std::setw(13)
		<< std::left
		<< " failed\n";
    }

    _dest.detach();
    src.detach();
  }

  // Postconditions:

  // Exit:
}

void
dft_read_perf::
postorder_action()
{

  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

void
dft_read_perf::
reset()
{
  // Preconditions:

  // Body:

  _success_ct = 0;
  _failure_ct = 0;

  // Discover the longest name that will be encountered during
  // the traversal and record it.  This allows proper formatting
  // of the output.

  // ISSUE:
  // Does this initial traversal affect the performance results
  // through some sort of caching?

  dft_namelen len;

  len.traverse(start(), _filter);

  _longest = len.longest();

  // Make sure field with for names is at least as long
  // as "container name".

  if (_longest < 14)
    _longest = 14;

  // Print a header

  std::cout << std::setw(_longest+2) << std::setfill(' ') << std::left
	    << "container name"
	    << std::setw(11)
	    << "  type   "
	    << std::setw(13) << std::left
	    << "test status"
	    << std::setw(15)
	    << "  bytes read (kB)  "
	    << std::setw(25)
	    << "      read time (ms) "
	    << std::setw(16) << std::left
	    << "  io rate (mB/s)";
  if (_verbose)
  {
    std::cout << "  persistent container characteristics";
  }
  std::cout << std::endl;

  // Postconditions:

  assert(invariant());
  assert(success_ct() == 0);
  assert(failure_ct() == 0);

  // Exit:
}

unsigned
dft_read_perf::
success_ct() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _success_ct;

  // Postconditions:

  // Exit:

  return result;
}

unsigned
dft_read_perf::
failure_ct() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _failure_ct;

  // Postconditions:

  // Exit:

  return result;
}

void
dft_read_perf::
set_verbose(bool x)
{
  // Preconditions:

  // Body:

  _verbose = x;

  // Postconditions:

  assert(invariant());
  assert(is_verbose() == x);

  // Exit:

}

bool
dft_read_perf::
is_verbose() const
{
  bool result;

  // Preconditions:

  // Body:

  result = _verbose;

  // Postconditions:

  // Exit:

  return result;
}
