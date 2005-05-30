#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

/*
  These values should be determined when the library is
  built.  Note: I suspect that the FreeBSD/SunOS definitions
  would be universally correct.
 */

// For FreeBSD (sleipnir)
// For SunOS (shanti)

/*
typedef long suseconds_t;
#define SUSECONDS_T_MIN  LONG_MIN
#define SUSECONDS_T_MAX  LONG_MAX
*/

// For x86 32 bit linux (heping, lotus, mir)
// For Aix (copper)

#define SUSECONDS_T_MIN  INT_MIN
#define SUSECONDS_T_MAX  INT_MAX



#endif
