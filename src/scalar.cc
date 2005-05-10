#include "scalar.h"
#include "contract.h"

scalar::
scalar()
{
  // Preconditions:

  // Body:

  _hid = H5Screate(H5S_SCALAR);

  // Postconditions:

  assert(is_attached());
  assert(invariant());

  // Exit:
}

scalar::
~scalar()
{
}

bool
scalar::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = dataspace::invariant();
  result = result && d() == 0;

  // Postconditions:

  // Exit:

  return result;
}

