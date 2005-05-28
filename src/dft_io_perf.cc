#include "dft_io_perf.h"
#include "contract.h"

#include "memory.h"
#include "pcontainer.h"

dft_io_perf::
dft_io_perf()
{
  // Preconditions:

  // Body:

  _success_ct = 0;
  _failure_ct = 0;

  // Postconditions:

  assert(invariant());

  // Exit:

}

dft_io_perf::
~dft_io_perf()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
dft_io_perf::
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

dft_io_perf::
dft_io_perf(const dft_io_perf& xother)
{
  not_implemented;
}

dft_io_perf&
dft_io_perf::
operator=(const dft_io_perf& xother)
{
  not_implemented;
  return *this;  // keeps pgi compiler happy until this is implemented.
}

void
dft_io_perf::
preorder_action()
{
  // Preconditions:

  // Body:

  H5I_type_t type = H5Iget_type(current_hid());

  if (type == H5I_DATASET || type == H5I_ATTR)
  {
    const pcontainer& src(dynamic_cast<const pcontainer&>(current()));
    memory            dest;

    dest.reserve(src);

    _tester.run_test(src, dest);

    if (_tester.status() == test::SUCCESS)
      ++_success_ct;
    else
      ++_failure_ct;

    cout << "\ti/o performance report for "
	 << src.type()
	 << " `"
	 << name(true)
	 << "': "
	 << _tester;

    if (_tester.status() == test::SUCCESS)
      cout << " bytes/s.";

    cout << '\n';
  }

  // Postconditions:

  // Exit:
}

void
dft_io_perf::
postorder_action()
{

  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

void
dft_io_perf::
reset()
{
  // Preconditions:

  // Body:

  _success_ct = 0;
  _failure_ct = 0;

  // Postconditions:

  assert(invariant());
  assert(success_ct() == 0);
  assert(failure_ct() == 0);

  // Exit:
}

unsigned
dft_io_perf::
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
dft_io_perf::
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
