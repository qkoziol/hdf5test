#include "matrix.h"
#include "contract.h"

matrix::
matrix(int xrow, int xcol, mode xmode)
{
  // Preconditions:

  // Body:

  not_implemented;

  // Postconditions:

  assert(invariant());

  // Exit:
}

matrix::
~matrix()
{
  // Preconditions:

  // Body:

  not_implemented;

  // Postconditions:

  // Exit:
}

bool
matrix::
invariant() const
{
  bool result;

  // Preconditions:

  not_implemented;

  // Body:

  not_implemented;
  result = hyperslab::invariant();

  // Postconditions:

  not_implemented;

  // Exit:

  return result;
}

ostream&
operator<<(ostream& xos, const matrix& xmatrix)
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

