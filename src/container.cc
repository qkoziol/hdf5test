#include "container.h"
#include "contract.h"

container::
container()
{
  // Preconditions:

  // Body:

  _type = H5I_INVALID_HID;

  // Postconditions:

  assert(invariant());
}

container::
~container()
{
  // Preconditions:

  // Body:

  // Nothing to do.  Standard C++ destructor
  // process is sufficient.  This function exists
  // only to keep g++ from complaining about a
  // virtual class with non-virtual destructor.

  // Postconditions:

  // Exit:
}

bool
container::
invariant() const
{
  // Preconditions:

  // Body:

  bool result = (_space.is_attached() && H5Iget_type(_type) == H5I_DATATYPE) ||
                (!_space.is_attached() && H5Iget_type(_type) == H5I_BADID);

  // Postconditions:

  // Exit:

  return result;
}

dataspace&
container::
get_space()
{
  // Preconditions:

  // Body:

  dataspace& result = _space;

  // Postconditions:

  assert(invariant());

  // Exit:

  return result;
}

const dataspace&
container::
get_space() const
{
  // Preconditions:

  // Body:

  const dataspace& result = _space;

  // Postconditions:

  // Exit:

  return result;
}

hid_t
container::
get_type() const
{
  // Preconditions:

  // Body:

  hid_t result = _type;

  // Postconditions:

  // Exit:

  return result;
}
