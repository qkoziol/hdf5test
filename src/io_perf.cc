#include "io_perf.h"
#include "contract.h"

#include "memory.h"
#include "pcontainer.h"
#include "transfer.h"

io_perf::
io_perf()
{
  // Preconditions:

  // Body:

  _bytes   = 0;
  _elapsed = 0;

  // Postconditions:

  assert(invariant());

  // Exit:
}

io_perf::
~io_perf()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
io_perf::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = test::invariant();

  // Postconditions:

  // Exit:

  return result;
}

io_perf::
io_perf(const io_perf& xother)
{
  not_implemented;
}

io_perf&
io_perf::
operator=(const io_perf& xother)
{
  not_implemented;
  return *this;
}

void
io_perf::
run_test(const pcontainer& xcon, memory& xmem)
{
  // Preconditions:

  assert(xcon.is_attached());
  // TODO:
  // Make this executable:
  assert(unexecutable(xmem is big enough));

  // Body:

  _timer.start();

  // ISSUE:
  // How do we introduce transfer properties?
  // For now we make do with the default.

  bool rtn = transfer(xcon, xmem, H5P_DEFAULT);

  _timer.stop();

  if (rtn)
  {
    // ERROR:
    // We don't want to compute performance on the basis of the
    // size of the extent, but on the actual amount of data
    // transferred.  This should be the number of points in
    // the selection * the number of bytes in a datatype
    // divided by the elapsed time.
    _bytes = xcon.get_space().get_extent().npoints();
    _elapsed = _timer.elapsed();

    _performance = _bytes/_elapsed;

    _status = SUCCESS;
  }
  else
  {
    _performance = 0;
    _status = FAILURE;
  }

  // Postconditions:

  // Exit:
}

void
io_perf::
run_test(const memory& xmem, pcontainer& xcon)
{
  // Preconditions:

  assert(xcon.is_attached());
  // TODO:
  // Make this executable:
  assert(unexecutable(xmem is big enough));

  // Body:

  _timer.start();

  // ISSUE:
  // How do we introduce transfer properties?
  // For now we make do with the default.

  bool rtn = transfer(xmem, xcon, H5P_DEFAULT);

  _timer.stop();

  if (rtn)
  {
    // ERROR:
    // We don't want to compute performance on the basis of the
    // size of the extent, but on the actual amount of data
    // transferred.  This should be the number of points in
    // the selection * the number of bytes in a datatype
    // divided by the elapsed time.

    _bytes = xcon.get_space().get_extent().npoints();
    _elapsed = _timer.elapsed();

    _performance = _bytes/_elapsed;

    _status = SUCCESS;
  }
  else
  {
    _performance = 0;
    _status = FAILURE;
  }

  // Postconditions:

  // Exit:
}

void
io_perf::
run_test()
{
  not_implemented;
}


ostream&
operator<<(ostream& xos, const io_perf& xiot)
{
  // Preconditions:

  assert(xos.good());

  // Body:

  if (xiot.status() == test::SUCCESS)
  {
    xos << "succeeded.  "
	<< xiot._bytes
        << " bytes transferred in "
	<< xiot._elapsed
	<< " seconds = "
	<< xiot.performance();
  }
  else if (xiot.status() == test::FAILURE)
  {
    xos << "failed.";
  }
  else
  {
    xos << "not run.";
  }

  // Postconditions:

  // Exit:

  return xos;
}
