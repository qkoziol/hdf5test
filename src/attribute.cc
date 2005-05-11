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
  dataspace* ptr_to_result;

  // Preconditions:

  assert(is_attached());

  // Body:

  hid_t space = H5Aget_space(_hid);

  ptr_to_result = new dataspace(space);

  // Postconditions:

  assert(ptr_to_result->is_attached());

  // Exit:

  return *ptr_to_result;
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
  bool result;

  // Preconditions:

  assert(H5Iget_type(xhost) == H5I_DATASET || H5Iget_type(xhost) == H5I_DATATYPE || H5Iget_type(xhost) == H5I_GROUP);

  // Body:

  // Attempt to open the attribute.

  H5E_BEGIN_TRY
  {
    _hid = H5Aopen_name(xhost, xname.c_str());
  }
  H5E_END_TRY;

  if (_hid >= 0)
  {
    result = true;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  assert(result == is_attached());
  assert(invariant());

  // Exit:

  return result;
}


bool
attribute::
is_readable() const
{
  bool result;

  // Preconditions:

  // Body:

  if (is_attached())
  {
    result = true;
  }
  else
  {
    // Nothing to read: unattached.

    result = false;
  }

  // Postconditions:

  assert(result == is_attached());

  // Exit:

  return result;
}
