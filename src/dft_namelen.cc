#include "dft_namelen.h"
#include "contract.h"

#include "attribute.h"
#include "pcontainer.h"

dft_namelen::
dft_namelen()
{
  // Preconditions:

  // Body:

  _longest = 0;
  _mode = PATH;

  // Postconditions:

  assert(invariant());
  assert(longest() == 0);
  assert(get_mode() == PATH);

  // Exit:

}

dft_namelen::
~dft_namelen()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
dft_namelen::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = dft::invariant();

  // Postconditions:

  // Exit:

  return result;
}

dft_namelen::
dft_namelen(const dft_namelen& xother)
{
  not_implemented;
}

dft_namelen&
dft_namelen::
operator=(const dft_namelen& xother)
{
  not_implemented;
  return *this;  // keeps pgi compiler happy until this is implemented.
}

void
dft_namelen::
preorder_action()
{
  // Preconditions:

  // Body:

  size_t len;

  if (_mode == PATH)
  {
    len = strlen(name(true));
  }
  else
  {
    len = strlen(name(false));
  }

  if (len > _longest)
    _longest = len;

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
dft_namelen::
postorder_action()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
dft_namelen::
reset()
{
  // Preconditions:

  // Body:

  _longest = 0;

  // Postconditions:

  assert(invariant());
  assert(longest() == 0);

  // Exit:
}

size_t
dft_namelen::
longest() const
{
  size_t result;

  // Preconditions:

  // Body:

  result = _longest;

  // Postconditions:

  // Exit:

  return result;
}

void
dft_namelen::
set(mode xmode)
{
  // Preconditions::

  // Body:

  _mode = xmode;

  // Postconditions:

  assert(invariant());
  assert(get_mode() == xmode);

  // Exit:
}

dft_namelen::mode
dft_namelen::
get_mode() const
{
  mode result;

  // Preconditions::

  // Body:

  result = _mode;

  // Postconditions:

  // Exit:

  return result;
}
