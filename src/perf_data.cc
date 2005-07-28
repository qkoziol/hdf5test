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

  bytes      = x;
  open_file  = x;
  open       = x;
  extend     = x;
  elapsed    = x;
  close      = x;
  close_file = x;

  // Postconditions:

  // no invariant; class is just pod.

  // Exit:

  return *this;
}
