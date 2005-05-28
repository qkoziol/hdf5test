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
    int old_ref_ct = H5Iget_ref(_hid);

    H5Idec_ref(_hid);

    // The code here is a little unusual to get around HDF5
    // behavior.  If the reference count to an hid is >= 1
    // then calls to H5Idec_ref() and H5Iget_ref() return
    // w/o generating diagnostic messages.  However, if one
    // calls H5Iget_ref() with an argument that was once
    // valid, but is no longer because the reference count
    // is < 1, then lots of diagnostics are generated.
    // So, we avoid calling H5Iget_ref() after H5Idec_ref()
    // to make sure we don't trigger diagnostics.

    // Another unusual behavior is that decrementing the
    // reference count when the count is 1 results in a
    // reference count of -1.  That emphasizes, I suppose,
    // that the argument is an invalid hid.

    if (old_ref_ct == 1)
    {
      _hid = H5I_INVALID_HID;
    }
  }


  // Postconditions:

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
