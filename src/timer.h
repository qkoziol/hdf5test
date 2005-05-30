#ifndef TIMER_H
#define TIMER_H

#ifndef H5_HAVE_PARALLEL
#include <sys/time.h>
#endif

//  A stopwatch.  Reports elapsed time in seconds.

class timer
{
 public:


  // Standard features:


  // Timer can operate in one of two modes.  In ACCUMULATE
  // it accumulates all elapsed time between any successive
  // start() and stop() calls.  In reset, each start() call
  // sets the elapsed time to 0.

  enum mode {ACCUMULATE, RESET};

  // Default constructor.

  timer(mode xmode = RESET);

  // Copy constructor.

  timer(const timer& xother);

  // Destructor.

  ~timer();

  // Assignment operator.

  timer& operator=(const timer& xother);

  // Class invariant.

  bool invariant() const;


  // Queries:


  // Get elapsed time.

  double elapsed() const;

  // Get current time - origin of timescale unspecified.

  double now() const;

  // What is the operating mode?

  mode get_mode() const;


  // Commands:


  // Start timer.

  void start();

  // Stop timer.

  void stop();

  // Change the mode.

  void put_mode(mode xmode);

  // Resets accumulated elapsed time to 0.

  void reset();

#ifndef H5_HAVE_PARALLEL

  // An interface on struct timeval for defining various
  // conveniences.

  class cron : public timeval
  {
  public:

    // Is this >= xother?

    bool operator>=(const struct timeval& xother) const;

    // Is this > xother?

    bool operator>(const struct timeval& xother) const;

    // Is this == xother?

    bool operator==(const struct timeval& xother) const;

    // Assignment.

    cron& operator=(const struct timeval& xother);

    // Conversion to double.

    operator double() const;

    // xresult = xa-xb.

    static void minus(const struct timeval& xa, const struct timeval& xb, struct timeval& xresult);

    // this -= xother.

    void minus(const struct timeval& xother);

    // xresult = xa+xb.

    static void plus(const struct timeval& xa, const struct timeval& xb, struct timeval& xresult);

    // this += xother.

    void plus(const struct timeval& xother);

    // Leaves the value of x unchanged but ensures that the microsecond
    // field is a proper fraction of a second.

    static void make_proper(struct timeval& x);

    // True if x's microsecond field is a proper fraction of a second.

    static bool is_proper(const struct timeval& x);

    // Zeroes tv_sec and tv_usec fields.

    void zero();
  };

#endif

 protected:

#ifdef H5_HAVE_PARALLEL

  double _start;   // Starting time.
  double _elapsed; // Elapsed time after last stop().

#else

  cron  _start;   // Starting time.
  cron  _elapsed; // Elapsed time after last stop().

#endif

  mode _mode; // Operating mode.

 private:
};

#endif // TIMER_H
