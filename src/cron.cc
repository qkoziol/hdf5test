#include "cron.h"
#include "contract.h"
#include <sys/types.h>

// ISSUE:
// The serial timer depends on gettimeofday() for its notion
// of time.  gettimeofday() in turn, returns its notion of
// time in a struct timeval.  On all systems, a struct timeval
// contains two counters: tv_sec, which counts the number of
// seconds, and tv_usec, which counts the number of microseconds.
// Each of these counters is some kind of integer.  This code has
// been tested on linux (both 32 and 64 bit), AIX, FreeBSD, Solaris.
// In all of these cases, tv_sec and tv_usec are defined to be the
// same kind of integer, and they are either both ints or both
// longs.  FreeBSD and Solaris defines both to be longs.  Linux and
// AIX "hide" the types behind at least one additional layer of
// type definitions.  They define tv_sec to be a time_t, and tv_usec
// tv_usec to be a suseconds_t.  AIX in turn defines both a time_t and
// a suseconds_t to be ints.  Linux defines them both to be longs.
// So, in the end, both fields are either ints or longs on all systems
// tested.  Whether this conclusion is more general is unknown.

// The types matter because in this class we do integer arithmetic on the
// microsecond field.  The microsecond counter can overflow or underflow,
// and these conditions cause additional arithmetic to occur on the
// second counter.  To detect overflow or underflow, we have to know the
// max and min values that can be represented by the microsecond counter
// type.  And these max and min values depend on the integer type that
// represents them.

// The types and max/min values need to be detected at configure time.
// I don't know whether the gnu configuration and build system does this
// or whether such detection needs to be written for this project.
// It hasn't been written, and is left as an exercise for the installer.

#include "config.h"

// For conversion from microseconds to seconds.

#define MILLION 1000000

void
cron::
make_proper(struct timeval& x)
{
  // Precondtions:

  // Body:

  if (x.tv_usec >= MILLION || x.tv_usec <= -MILLION)
  {
    tv_usec_t n = x.tv_usec/MILLION;

    x.tv_sec += n;

    x.tv_usec = x.tv_usec%MILLION;
  }

  // Postconditions:

  // Exit:
}

bool
cron::
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

cron&
cron::
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
cron::
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
cron::
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
cron::
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
cron::
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
cron::
plus(const struct timeval& xa, const struct timeval& xb, struct timeval& xresult)
{
  // Preconditions:

  // Body:

  // Addition is complicated by the fact that we can get
  // overflow: it's not just a case of adding some ints.

  tv_usec_t max, min;

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

  if (max >= 0 && min > TV_USEC_T_MAX-max)
  {
    // Then max+min is positive and greater than TV_USEC_T_MAX.
    // Overflow occurs in naive addition.

    tv_usec_t nsecs = TV_USEC_T_MAX / MILLION;

    xresult.tv_sec  = xa.tv_sec + xb.tv_sec + nsecs;
    xresult.tv_usec = (max - nsecs * MILLION) + min;

    assert(xresult.tv_usec > 0);
  }
  else if (min < 0 && max < TV_USEC_T_MIN-min)
  {
    // Then max+min is negative and less than TV_USEC_T_MIN.
    // Underflow occurs in naive addition.

    tv_usec_t nsecs = TV_USEC_T_MAX / MILLION;

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
cron::
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
cron::
minus(const struct timeval& xa, const struct timeval& xb, struct timeval& xresult)
{
  // Preconditions:

  // Body:

  // Subtraction is complicated by the fact that we can get
  // overflow: it's not just a case of subtracting some ints.

  if (xa.tv_usec >= 0 && xb.tv_usec < xa.tv_usec-TV_USEC_T_MAX)
  {
    // xa.tv_usec-x.tv_usec is greater than TV_USEC_T_MAX;
    // overflow occurs in naive subtraction.

    tv_usec_t nsecs = TV_USEC_T_MAX / MILLION;

    xresult.tv_sec  = xa.tv_sec - xb.tv_sec + nsecs;
    xresult.tv_usec = (xa.tv_usec - nsecs * MILLION) - xb.tv_usec;
  }
  else if (xb.tv_usec > 0 && xa.tv_usec < TV_USEC_T_MIN+xb.tv_usec)
  {
    // xa.tv_usec-x.tv_usec is less than TV_USEC_T_MIN;
    // underflow occurs in naive subtraction.

    tv_usec_t nsecs = TV_USEC_T_MAX / MILLION;

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
cron::
minus(const struct timeval& xother)
{
  // Preconditions:

  // Body:

  minus(*this, xother, *this);

  // Postconditions:

  assert(is_proper(*this));

  // Exit:
}

cron::
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

std::ostream&
operator<<(std::ostream& xos, const cron& xcron)
{
  // Preconditions:

  assert(xos.good());

  // Body:

  xos << '{'
      << xcron.tv_sec
      << ','
      << xcron.tv_usec
      << '}';

  // Postconditions:

  // Exit:

  return xos;
}

