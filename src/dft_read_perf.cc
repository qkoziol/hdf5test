#include "dft_read_perf.h"
#include "contract.h"

#include "dft_namelen.h"
#include "memory.h"
#include "pcontainer.h"
#include "std_iomanip.h"
#include "std_iostream.h"

dft_read_perf::
dft_read_perf()
{
  // Preconditions:

  // Body:

  _success_ct = 0;
  _failure_ct = 0;
  _longest    = 0;

  // Postconditions:

  assert(invariant());

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
    memory            dest;

    dest.reserve(src);

    _tester.run_test(src, dest);

    if (_tester.status() == test::SUCCESS)
      ++_success_ct;
    else
      ++_failure_ct;

    cout << setw(_longest+2)
	 << left
	 << name(true)
	 << setw(11)
	 << left
	 << src.type_name();

      if (_tester.status() == test::SUCCESS)
      {
	double kb      = _tester.bytes()/1e3;
	double elapsed = _tester.elapsed();

	cout << setw(13) << left
	     << " succeeded"
	     << setw(15) << right << fixed << setprecision(3)
	     << kb
	     << setw(19) << right << fixed << setprecision(3)
	     << elapsed*1e3
	     << "    "
	     << setw(13) << right << fixed << setprecision(3)
	     << kb/elapsed/1e3
	     << '\n';
      }
      else
      {
	cout << setw(13)
	     << left
	     << " failed\n";
      }
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

  cout << setw(_longest+2)
       << setfill(' ')
       << left
       << "container name"
       << setw(11)
       << "  type   "
       << setw(13)
       << left
       << "test status"
       << setw(15)
       << "  bytes read (kb)  "
       << setw(25)
       << "   elapsed time (ms) "
       << setw(16)
       << left
       << "  io rate (mb/s)"
       << endl;


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
