#include "attribute.h"
#include "contract.h"

attribute::
attribute()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(! is_attached());
  assert(invariant());

  // Exit:
}

attribute::
~attribute()
{
  not_implemented;
}

bool
attribute::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = container::invariant();

  // Postconditions:

  // Exit:

  return result;
}

attribute::
attribute(const attribute& xother)
{
  not_implemented;
}

attribute&
attribute::
operator=(const attribute& xother)
{
  not_implemented;
}

dataspace&
attribute::
get_space() const
{
  not_implemented;
}


hid_t
attribute::
get_type() const
{
  hid_t result;

  // Preconditions:

  assert(is_attached());

  // Body:

  result = H5Aget_type(_hid);

  // Postconditions:

  assert(H5Iget_type(result) == H5I_DATATYPE);

  // Exit:

  return result;
}

bool
attribute::
open(hid_t xhost, const string& xname)
{
  not_implemented;
}

