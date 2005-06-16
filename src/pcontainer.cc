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

  result = persistent::invariant() && container::invariant();

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

void
pcontainer::
detach()
{
  // Preconditions:

  // Body:

  hdf5_handle::detach();

  _space.detach();

  if (H5Iget_type(_type) == H5I_DATATYPE)
  {
    H5Idec_ref(_type);
  }

  _type = H5I_INVALID_HID;

  // Postconditions:

  assert(invariant());
  assert(!is_attached());
  assert(!get_space().is_attached());

  // Exit:
}
