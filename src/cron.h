#ifndef CRON_H
#define CRON_H

#include <ostream>
#ifndef H5_HAVE_PARALLEL
#include <sys/time.h>
#endif


/*! @class cron
    @brief An interface on struct timeval providing comparison, arithmetic, and initialization
           operators.
*/

class cron : public timeval
{
  friend std::ostream& operator<<(std::ostream& xos, const cron& xcron);

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

#endif // CRON_H
