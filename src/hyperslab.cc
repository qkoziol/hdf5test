#include "hyperslab.h"
#include "contract.h"

hyperslab::
hyperslab()
{
  // Preconditions:

  not_implemented;

  // Body:

  not_implemented;

  // Postconditions:

  not_implemented;

  // Exit:
}

hyperslab::
hyperslab(int xdim)
{
  // Preconditions:

  not_implemented;

  // Body:

  not_implemented;

  // Postconditions:

  not_implemented;

  // Exit:
}

hyperslab::
hyperslab(const tuple& xblock_size,
	  const tuple& xorigin,
	  const tuple& xstride,
	  const tuple& xct)
{
  not_implemented;
}

hyperslab::
hyperslab(const extent& xextent)
{
  not_implemented;
}

hyperslab::
~hyperslab()
{
  // Preconditions:

  not_implemented;

  // Body:

  not_implemented;

  // Postconditions:

  not_implemented;

  // Exit:
}

int
hyperslab::
d() const
{
  not_implemented;
  return -1;  // keeps pgi compiler happy until this is implemented.
}

tuple&
hyperslab::
ct()
{
  not_implemented;
  // the following keeps the pgi compiler happy until
  // this is implemented.
  tuple* junk = new tuple;
  return *junk;
}

bool
hyperslab::invariant() const
{
  not_implemented;
  return false;  // keeps pgi compiler happy until this is implemented.
}
