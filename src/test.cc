#include "test.h"
#include "contract.h"

test::
test()
{
  // Preconditions:

  // Body:

  _status = NOT_RUN;

  // Postconditions:

  assert(invariant());
  assert(status() == NOT_RUN);

  // Exit:
}

test::
~test()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:

}

bool
test::
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

test::
test(const test& xother)
{
  not_implemented;
}

test&
test::
operator=(const test& xother)
{
  not_implemented;
}

double
test::
performance() const
{
  double result;

  // Preconditions:

  assert(status() == SUCCESS);

  // Body:

  result = _performance;

  // Postconditions:

  // Exit:

  return result;
}

test::state
test::
status() const
{
  state result;

  // Preconditions:

  // Body:

  result = _status;

  // Postconditions:

  // Exit:

  return result;
}


ostream&
operator<<(ostream& xos, const test& xt)
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
