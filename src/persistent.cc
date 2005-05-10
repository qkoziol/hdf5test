#include "persistent.h"
#include "contract.h"

persistent::
persistent()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());
  assert(! is_attached());

  // Exit:
}

persistent::
~persistent()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
persistent::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = hdf5_handle::invariant();

  // Postconditions:

  // Exit:

  return result;
}

persistent::
persistent(const persistent& xother)
{
  not_implemented;
}

persistent&
persistent::
operator=(const persistent& xother)
{
  not_implemented;
}
