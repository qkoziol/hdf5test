#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

/*
  If you're interested in why tv_sec_t, tv_usec_t, TV_USEC_T_MIN,
  and TV_USEC_T_MAX need to be defined, see the discussion in
  cron.cc.  In any case, to install this package, determine the
  types of struct timeval's tv_sec and tv_usec fields, and define
  tv_sec_t and tv_usec_t to be these types.  Then determine the
  min and max values that a tv_usec_t can attain and define
  TV_USEC_T_MIN and TV_USEC_T_MAX appropriately.
*/

// For x86 32 bit linux (heping, lotus, mir)
// For FreeBSD (sleipnir)
// For SunOS (shanti)

typedef long tv_sec_t;
typedef long tv_usec_t;
#define TV_USEC_T_MIN  LONG_MIN
#define TV_USEC_T_MAX  LONG_MAX

// For Aix (copper)

/*
typedef int tv_sec_t;
typedef int tv_usec_t;
#define TV_USEC_T_MIN  INT_MIN
#define TV_USEC_T_MAX  INT_MAX
*/

// Define whether a kilobyte is 1000 or 1024 bytes

#define BYTES_PER_KB 1000

#endif
