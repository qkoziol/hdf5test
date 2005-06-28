#include "perf_data.h"
#include "contract.h"

perf_data::
perf_data()
{
  // Preconditions:

  // Body:

  *this = 0;

  // Postconditions:

  // no invariant; class is just pod.

  // Exit:
}

perf_data&
perf_data::
operator=(size_t x)
{
  // Preconditions:

  // Body:

  bytes   = x;
  open    = x;
  elapsed = x;
  close   = x;

  // Postconditions:

  // no invariant; class is just pod.

  // Exit:

  return *this;
}
