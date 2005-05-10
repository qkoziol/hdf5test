#include "timer.h"
#include "contract.h"

#include <sys/time.h>

timer::
timer(mode xmode)
{
  // Preconditions:

  // Body:

  _start = timer::now();
  _elapsed = 0;
  _mode = xmode;

  // Postconditions:

  assert(invariant());
  assert(elapsed() == 0);
  assert(get_mode() == xmode);
}

timer::
timer(const timer& xother)
{
  // Preconditions:

  // Body:

  _start   = xother._start;
  _elapsed = xother._elapsed;
  _mode    = xother._mode;

  // Postconditions:

  assert(invariant());
}

timer::
~timer()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

timer&
timer::
operator=(const timer& xother)
{
  // Preconditions:

  // Body:

  _start   = xother._start;
  _elapsed = xother._elapsed;
  _mode    = xother._mode;

  // Postconditions:

  assert(invariant());

  // Exit:

  return *this;
}

bool
timer::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = timer::now() >= _start;
  result = result && _start >= 0;
  result = result && _elapsed >= 0;

  // Postconditions:

  return result;
}

// For conversion from microseconds to seconds.

#define MILLION (double)1000000

double
timer::
now()
{
  double result;

  // Preconditions:

  // Body:

#ifdef H5_HAVE_PARALLEL

  result = MPI_Wtime();

#else

  struct timeval t;

  if (gettimeofday(&t, 0) == 0)
  {
    // The call to gettimeofday() succeeded.

    result = (double)t.tv_sec+(double)t.tv_usec/MILLION;
  }
  else
  {
    // The call failed; result is nonsense time.
    // ISSUE:
    // If we assert that this doesn't happen in the postconditions,
    // why do we even take this branch?  Can gettimeofday() ever
    // fail?

    result = -1;
  }

#endif

  // Postconditions:

  assert(result >= 0);

  // Exit:

  return result;
}

double
timer::
elapsed() const
{
  double result;

  // Preconditions:

  // Body:

  result = _elapsed;

  // Postconditions:

  assert(result >= 0);

  // Exit:

  return result;
}

void
timer::
start()
{
  // Preconditions:

  // Body:

  _start = timer::now();
  if (_mode == RESET)
    _elapsed = 0;

  // Postconditions:

  assert(invariant());
  assert(get_mode() == RESET ? elapsed() == 0 : true);

  // Exit:
}

void
timer::
stop()
{
  // Preconditions:

  // Body:

  if (_mode == RESET)
  {
    _elapsed = timer::now()-_start;
  }
  else
  {
    _elapsed += timer::now()-_start;
  }

  // Postconditions:

  assert(invariant());
  assert(elapsed() > 0);

  // Exit:
}

void
timer::
reset()
{
  // Preconditions:

  // Body:

  _start = now();
  _elapsed = 0;

  // Postconditions:

  assert(invariant());
  assert(elapsed() == 0);

  // Exit:
}

timer::mode
timer::
get_mode() const
{
  mode result;

  // Preconditions:

  // Body:

  result = _mode;

  // Postconditions:

  // Exit:

  return result;
}
