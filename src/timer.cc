#include "timer.h"
#include "contract.h"

#include "config.h"

timer::
timer(mode xmode)
{
  // Preconditions:

  // Body:

#ifdef H5_HAVE_PARALLEL
  _start   = 0;
  _elapsed = 0;
#else
  _start.zero();
  _elapsed.zero();
#endif
  _mode    = xmode;

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

#ifdef H5_HAVE_PARALLEL

  result = (now() >= _start);
  result = result && (_start >= 0);
  result = result && (_elapsed >= 0);

#else

  cron zero;

  zero.zero();

  cron now;

  gettimeofday(&now, 0);

  result = (now >= _start);
  result = (_start >= zero);
  result = (_elapsed >= zero);

#endif

  // Postconditions:

  // Exit:

  return result;
}

// For conversion from microseconds to seconds.

#define MILLION 1000000

double
timer::
now() const
{
  double result;

  // Preconditions:

  // Body:

#ifdef H5_HAVE_PARALLEL

  result = MPI_Wtime();

#else

  cron t;

  if (gettimeofday(&t, 0) == 0)
  {
    // The call to gettimeofday() succeeded.

    result = t;
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

#ifdef H5_HAVE_PARALLEL
  _start = now();
  if (_mode == RESET)
    _elapsed = 0;
#else
  struct timeval now;

  gettimeofday(&now, 0);

  _start = now;

  if (_mode == RESET)
  {
    _elapsed.zero();
  }
#endif

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

#ifdef H5_HAVE_PARALLEL
  if (_mode == RESET)
  {
    _elapsed = now()-_start;
  }
  else
  {
    _elapsed += now()-_start;
  }
#else

  // First compute elapsed time between now and timer start,
  // being careful that the elapsed time has nonzero second
  // and microsecond fields.

  cron now, diff;

  gettimeofday(&now, 0);

  cron::minus(now, _start, diff);

  if (_mode == RESET)
  {
    _elapsed = diff;
  }
  else
  {
    // Accumulate last period timed into total elapsed time.
    // Be careful to avoid overflow in addition.

    _elapsed.plus(diff);
  }
#endif

  // Postconditions:

  assert(invariant());

  // You'd think that the elapsed time between a start and a stop
  // would be strictly greater than zero.  But it isn't.  The clock
  // underlying gettimeofday() may not change between calls to start
  // and stop if you're running this on a fast system with whose
  // gettimeofday() has a coarse resolution.  So we have to allow
  // for an elapsed time of zero.

  assert(elapsed() >= 0);

  // Exit:
}

void
timer::
reset()
{
  // Preconditions:

  // Body:

#ifdef H5_HAVE_PARALLEL

  _start = now();
  _elapsed = 0;

#else

  gettimeofday(&_start, 0);
  _elapsed.zero();

#endif

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

void
timer::
put_mode(mode xmode)
{
  // Preconditions:

  // Body:

  _mode = xmode;

  // Postconditions:

  assert(get_mode() == xmode);
  assert(invariant());

  // Exit:
}
