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

  result = (_timer.elapsed() >= _step.elapsed());

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

  for (start(); ! is_done(); next())
  {
    _step.start();


    // TODO:
    // do_partial_io() returns the number of bytes transferred and signals failure
    // of io operation if it returns a negative number.  We need to do something with
    // that return value here.

    do_partial_io();

    _step.stop();

    // TODO:
    // The step timer's state gets overwritten every time through this loop.  We
    // need to do something with the state here, either writing it out or saving it.
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}
