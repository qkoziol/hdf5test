#include "col_itr.h"
#include "contract.h"

col_itr::
col_itr(const matrix& xmat) :
  slice_itr(xmat, 1)
  // ERROR: we've ignored C/F styles
{
  // Preconditions:

  not_implemented;

  // Body:

  not_implemented;

  // Postconditions:

  not_implemented;

  // Exit:
}

void
col_itr::
start()
{
  not_implemented;
}

void
col_itr::
next()
{
  not_implemented;
}

bool
col_itr::
is_done() const
{
  not_implemented;
  return true;  // keeps the pgi compiler happy until this is implemented.
}

ostream&
operator<<(ostream& xos, const col_itr& xcol_itr)
{
  // Preconditions:

  not_implemented;

  // Body:

  not_implemented;

  // Postconditions:

  not_implemented;

  // Exit:

  return xos;
}
