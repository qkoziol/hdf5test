#include "container.h"
#include "contract.h"

container::
container()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(! is_attached());
  assert(invariant());

  // Exit:
}

container::
container(const container& xother)
{
  not_implemented;
}

container::
~container()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

container&
container::
operator=(const container& xother)
{
  not_implemented;
}


bool
container::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = persistent::invariant();

  // Postconditions:

  // Exit:

  return result;
}
