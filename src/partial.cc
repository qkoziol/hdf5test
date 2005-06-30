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


void
partial::
run_test()
{
  // Preconditions:

  // Body:

  bool error = false;

  for (start(); ! is_done() && ! error; next())
  {

    // TODO:
    // do_partial_io() returns the number of bytes transferred and signals failure
    // of io operation if it returns a negative number.  We need to do something with
    // that return value here.

    if (do_partial_io() < 0)
    {
      error = true;
    }
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}
