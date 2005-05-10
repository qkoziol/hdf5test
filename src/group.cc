#include "group.h"
#include "contract.h"

#include "hdf5_file.h"

group::
group()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(! is_attached());
  assert(invariant());

  // Exit:
}

group::
~group()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
group::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = persistent::invariant();

  // Postconditions:

  // Exit:

  return result;
}

bool
group::
open(hid_t xhost, const string& xname)
{
  bool result;

  // Preconditions:

  assert(H5Iget_type(xhost) == H5I_FILE || H5Iget_type(xhost) == H5I_GROUP);

  // Body:

  // TODO:
  // Outfit all open() attempts everywhere in test suite with
  // this sort of masking of error reporting.

  H5E_BEGIN_TRY
  {
    _hid = H5Gopen(xhost, xname.c_str());
  }
  H5E_END_TRY;

  if (_hid < 0)
    result = false;
  else
    result = true;

  // Postconditions:

  assert(invariant());
  assert(result == is_attached());

  // Exit:

  return result;
}

group&
group::
operator=(const group& xother)
{
  group* ptr_to_result;

  // Preconditions:

  // Body:

  _hid = xother._hid;
  if (_hid >= 0)
    H5Iinc_ref(_hid);
  ptr_to_result = this;

  // Postconditions:

  assert(is_attached() == xother.is_attached());
  assert(invariant());
  assert(hid() == xother.hid());

  // Exit:

  return *ptr_to_result;
}
