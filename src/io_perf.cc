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
run_test(pcontainer& xcon, memory& xmem)
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

    size_t type_size = H5Tget_size(xcon.get_type());

    if (xcon.get_space().d() > 0)
    {
      _results.bytes = H5Sget_select_npoints(xcon.get_space().hid())*type_size;
    }
    else
    {
      _results.bytes = type_size;
    }

    _results.elapsed = _timer.elapsed();

    _status = SUCCESS;
  }
  else
  {
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

    size_t type_size = H5Tget_size(xcon.get_type());

    if (xcon.get_space().d() > 0)
    {
      _results.bytes = xcon.get_space().get_extent().npoints()*type_size;
    }
    else
    {
      _results.bytes = type_size;
    }

    _results.elapsed = _timer.elapsed();

    _status = SUCCESS;
  }
  else
  {
    _status = FAILURE;
  }

  // Postconditions:

  // Exit:
}

bool
io_perf::
run_test()
{
  not_implemented;
  // The following is just a placebo to keep the compiler happy
  // in the absence of a real implementation.
  return false;
}


double
io_perf::
bytes() const
{
  double result;

  // Preconditions:

  assert(status() == SUCCESS);

  // Body:

  result = _results.bytes;

  // Postconditions:

  // Exit:

  return result;
}

double
io_perf::
elapsed() const
{
  double result;

  // Preconditions:

  assert(status() == SUCCESS);

  // Body:

  result = _results.elapsed;

  // Postconditions:

  // Exit:

  return result;
}
