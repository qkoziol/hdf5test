#include "datatype.h"
#include "contract.h"

datatype::
datatype()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:

  assert(!is_attached());
  assert(invariant());
}

datatype::
~datatype()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
datatype::
invariant() const
{
  bool result;

  // Preconditions:

  result = persistent::invariant();

  // Body:

  // Exit:

  return result;
}

datatype::
datatype(const datatype& xother)
{
  not_implemented;
}

datatype&
datatype::
operator=(const datatype& xother)
{
  not_implemented;
}

bool
datatype::
open(hid_t xhost, const string& xname)
{
  bool result;

  // Preconditions:

  assert(H5Iget_type(xhost) == H5I_FILE || H5Iget_type(xhost) == H5I_GROUP);

  // Body:

  // Attempt to open the datatype.

  H5E_BEGIN_TRY
  {
    _hid = H5Topen(xhost, xname.c_str());
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
