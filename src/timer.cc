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

#ifndef H5_HAVE_PARALLEL

void
timer::cron::
make_proper(struct timeval& x)
{
  // Precondtions:

  // Body:

  if (x.tv_usec >= MILLION || x.tv_usec <= -MILLION)
  {
    int n = x.tv_usec/MILLION;

    x.tv_sec += n;

    x.tv_usec = x.tv_usec%MILLION;
  }

  // Postconditions:

  // Exit:
}

bool
timer::cron::
is_proper(const struct timeval& x)
{
  bool result;

  // Precondtions:

  // Body:

  if (x.tv_usec >= 0 && x.tv_usec < MILLION)
  {
    result = true;
  }
  else if (x.tv_usec < 0 && x.tv_usec > -MILLION)
  {
    result = true;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

timer::cron&
timer::cron::
operator=(const struct timeval& xother)
{
  // Preconditions:

  // Body:

  tv_sec  = xother.tv_sec;
  tv_usec = xother.tv_usec;

  // Postconditions:

  // Exit:

  return *this;
}

bool
timer::cron::
operator>=(const struct timeval& xother) const
{
  bool result;

  // Preconditions:

  assert(is_proper(*this));
  assert(is_proper(xother));

  // Body:

  if (tv_sec > xother.tv_sec)
  {
    result = true;
  }
  else if (tv_sec == xother.tv_sec)
  {
    if (tv_usec >= xother.tv_usec)
    {
      result = true;
    }
    else
    {
      result = false;
    }
  }
  else
  {
    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

bool
timer::cron::
operator>(const struct timeval& xother) const
{
  bool result;

  // Preconditions:

  assert(is_proper(*this));
  assert(is_proper(xother));

  // Body:

  if (tv_sec > xother.tv_sec)
  {
    result = true;
  }
  else if (tv_sec == xother.tv_sec)
  {
    if (tv_usec > xother.tv_usec)
    {
      result = true;
    }
    else
    {
      result = false;
    }
  }
  else
  {
    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

bool
timer::cron::
operator==(const struct timeval& xother) const
{
  bool result;

  // Preconditions:

  assert(is_proper(*this));
  assert(is_proper(xother));

  // Body:

  result = (tv_sec == xother.tv_sec) && (tv_usec == xother.tv_usec);

  // Postconditions:

  // Exit:

  return result;
}

void
timer::cron::
zero()
{
  // Preconditions:

  // Body:

  tv_sec = 0;
  tv_usec = 0;

  // Postconditions:

  assert(tv_sec == 0);
  assert(tv_usec == 0);

  // Exit:
}

void
timer::cron::
plus(const struct timeval& xa, const struct timeval& xb, struct timeval& xresult)
{
  // Preconditions:

  // Body:

  // Addition is complicated by the fact that we can get
  // overflow: it's not just a case of adding some ints.

  suseconds_t max, min;

  if (xa.tv_usec > xb.tv_usec)
  {
    max = xa.tv_usec;
    min = xb.tv_usec;
  }
  else
  {
    max = xb.tv_usec;
    min = xa.tv_usec;
  }

  if (max >= 0 && min > SUSECONDS_T_MAX-max)
  {
    // Then max+min is positive and greater than SUSECONDS_T_MAX.
    // Overflow occurs in naive addition.

    int nsecs = SUSECONDS_T_MAX / MILLION;

    xresult.tv_sec  = xa.tv_sec + xb.tv_sec + nsecs;
    xresult.tv_usec = (max - nsecs * MILLION) + min;

    assert(xresult.tv_usec > 0);
  }
  else if (min < 0 && max < SUSECONDS_T_MIN-min)
  {
    // Then max+min is negative and less than SUSECONDS_T_MIN.
    // Underflow occurs in naive addition.

    int nsecs = SUSECONDS_T_MAX / MILLION;

    xresult.tv_sec   = xa.tv_sec + xb.tv_sec - nsecs;
    xresult.tv_usec = (min + nsecs * MILLION) + max;

    assert(xresult.tv_usec < 0);
  }
  else
  {
    // No overflow

    xresult.tv_usec = max + min;
    xresult.tv_sec  = xa.tv_sec + xb.tv_sec;
  }

  make_proper(xresult);

  // Postconditions:

  assert(is_proper(xresult));

  // Exit:
}

void
timer::cron::
plus(const struct timeval& xother)
{
  // Preconditions:

  // Body:

  plus(*this, xother, *this);

  // Postconditions:

  assert(is_proper(*this));

  // Exit:
}

void
timer::cron::
minus(const struct timeval& xa, const struct timeval& xb, struct timeval& xresult)
{
  // Preconditions:

  // Body:

  // Subtraction is complicated by the fact that we can get
  // overflow: it's not just a case of subtracting some ints.

  if (xa.tv_usec >= 0 && xb.tv_usec < xa.tv_usec-SUSECONDS_T_MAX)
  {
    // xa.tv_usec-x.tv_usec is greater than SUSECONDS_T_MAX;
    // overflow occurs in naive subtraction.

    int nsecs = SUSECONDS_T_MAX / MILLION;

    xresult.tv_sec  = xa.tv_sec - xb.tv_sec + nsecs;
    xresult.tv_usec = (xa.tv_usec - nsecs * MILLION) - xb.tv_usec;
  }
  else if (xb.tv_usec > 0 && xa.tv_usec < SUSECONDS_T_MIN+xb.tv_usec)
  {
    // xa.tv_usec-x.tv_usec is less than SUSECONDS_T_MIN;
    // underflow occurs in naive subtraction.

    int nsecs = SUSECONDS_T_MAX / MILLION;

    xresult.tv_sec  = xa.tv_sec - xb.tv_sec - nsecs;
    xresult.tv_usec = (xa.tv_usec + nsecs * MILLION) - xb.tv_usec;
  }
  else
  {
    // xa.tv_usec-xa.tv_usec can be represented in a suseconds_t.

    xresult.tv_usec = xa.tv_usec - xb.tv_usec;
    xresult.tv_sec  = xa.tv_sec  - xb.tv_sec;
  }

  make_proper(xresult);

  // Postconditions:

  assert(is_proper(xresult));

  // Exit:
}

void
timer::cron::
minus(const struct timeval& xother)
{
  // Preconditions:

  // Body:

  minus(*this, xother, *this);

  // Postconditions:

  assert(is_proper(*this));

  // Exit:
}

timer::cron::
operator double() const
{
  double result;

  // Preconditions:

  // Body:

  result = (double)tv_sec + (double)tv_usec/(double)MILLION;

  // Postconditions:

  // Exit:

  return result;
}

#endif
