#include "hdf5_handle.h"
#include "contract.h"

hdf5_handle::
hdf5_handle()
{
  // Preconditions:

  // Body:

  _hid = H5I_INVALID_HID;

  // Postconditions:

  // Exit:

  assert(invariant());
  assert(! is_attached());
}

hdf5_handle::
hdf5_handle(const hdf5_handle& xother)
{
  not_implemented;
}

hdf5_handle::
~hdf5_handle()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
hdf5_handle::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  // Normally we'd call the ancestor's invariant,
  // handle::invariant(), but that doesn't exist.

  result = (_hid >= 0 == is_attached());

  // Postconditions:

  // Exit:

  return result;
}

hid_t
hdf5_handle::
hid() const
{
  const hid_t result = _hid;

  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:

  return result;
}

bool
hdf5_handle::
is_attached() const
{
  bool result;

  // Preconditions:

  // Body:

  result = (_hid >= 0);

  // Postconditions:

  // Exit:

  return result;
}

void
hdf5_handle::
detach()
{
  // Preconditions:

  // Body:

  if (_hid >= 0)
  {
    H5Idec_ref(_hid);

    _hid = H5I_INVALID_HID;
  }
  else

  // Postconditions:

  assert(hid() < 0);

  // Exit:
}

void
hdf5_handle::
attach(hid_t xhid)
{
  // Preconditions:

  assert(H5Iget_type(xhid) != H5I_BADID);

  // Body:

  _hid = xhid;
  H5Iinc_ref(xhid);

  // Postconditions:

  assert(invariant());
  assert(is_attached());
  assert(hid() == xhid);

  // Exit:
}
