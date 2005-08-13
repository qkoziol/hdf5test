#ifndef TIMER_H
#define TIMER_H

#include "cron.h"
#include <ostream>
#ifndef H5_HAVE_PARALLEL
#include <sys/time.h>
#endif

/*!  @class timer
     @brief A stopwatch.  Reports elapsed time in seconds.
*/

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
