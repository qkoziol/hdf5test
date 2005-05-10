#include "itr.h"
#include "contract.h"

itr::
itr()
{
  // Preconditions:

  // Body:

  _is_done = false;

  // Postconditions:

  assert(invariant());
  assert(!_is_done);

  // Exit:
}

itr::
~itr()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
itr::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = true;

  // Postconditions:

  // Exit:

  return result;
}

itr::
itr(const itr& xother)
{
  // Preconditions:

  // Body:

  _is_done = xother._is_done;

  // Postconditions:

  assert(_is_done == xother.is_done());
  assert(invariant());

  // Exit:
}

itr&
itr::
operator=(const itr& xother)
{
  itr* ptr_to_result;

  // Preconditions:

  // Body:

  ptr_to_result = this;
  _is_done = xother._is_done;

  // Postconditions:

  assert(is_done() == xother.is_done());
  assert(invariant());

  // Exit:

  return *ptr_to_result;
}
