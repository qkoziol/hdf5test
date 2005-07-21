#include "contract.h"
#include "partial.h"

partial::
partial()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:
}

partial::
~partial()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
partial::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = test::invariant();

  // Postconditions:

  // Exit:

  return result;
}

partial::
partial(const partial& xother)
{
  not_implemented;
}

partial&
partial::
operator=(const partial& xother)
{
  not_implemented;
  return *this;
}


bool
partial::
run_test()
{
  bool result;

  // Preconditions:

  // Body:

  result = true;

  for (start(); ! is_done() && result; next())
  {
    if (!do_partial_io())
    {
      result = false;
    }
  }

  // Postconditions:

  assert(invariant());

  // Exit:

  return result;
}
