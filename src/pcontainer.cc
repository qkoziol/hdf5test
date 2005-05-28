#include "pcontainer.h"
#include "contract.h"

pcontainer::
pcontainer()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());
  assert(! is_attached());

  // Exit:
}

pcontainer::
~pcontainer()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
pcontainer::
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

pcontainer::
pcontainer(const pcontainer& xother)
{
  not_implemented;
}

pcontainer&
pcontainer::
operator=(const pcontainer& xother)
{
  not_implemented;
  return *this;
}
