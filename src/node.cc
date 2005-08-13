#include "node.h"
#include "contract.h"

node::
node()
{
  // Preconditions:

  // Body:

  self   = -1;
  parent = -1;
  index  = -1;

  // Postconditions:

  // Exit:
}

bool
node::
operator==(const node& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  result = (self == xother.self) && (parent == xother.parent) && (index == xother.index);

  // Postconditions:

  // Exit:

  return result;
}

bool
node::
operator!=(const node& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  result = (self != xother.self) || (parent != xother.parent) || (index != xother.index);

  // Postconditions:

  // Exit:

  return result;
}

std::ostream&
operator<<(std::ostream& xos, const node& xnode)
{
  // Preconditions:

  assert(xos.good());

  // Body:

  xos << "parent = "
      << xnode.parent
      << " self = "
      << xnode.self
      << " index = "
      << xnode.index;

  // Postconditions:

  // Exit:

  return xos;
}
