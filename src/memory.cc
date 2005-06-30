#include "memory.h"
#include "contract.h"

#include "hdf5.h"
#include "pcontainer.h"

memory::
memory()
{
  // Preconditions:

  // Body:

  _mem   = 0;
  _ub    = 0;
  _type  = H5I_INVALID_HID;

  // Postconditions:

  assert(invariant());
  assert(ub() == 0);

  // Exit:
}

memory::
memory(const memory& xother)
{
  not_implemented;
}

memory::
~memory()
{
  // Preconditions:

  // Body:

  if (is_attached())
  {
    detach();
    delete [] _mem;
  }

  // Postconditions:

  // Exit:
}

memory&
memory::
operator=(const memory& xother)
{
  not_implemented;
  return *this;
}


bool
memory::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  // Skip calling ancestral invariants because they don't exist.

  if (_mem == 0)
  {
    result = (_ub == 0 && _type == H5I_INVALID_HID && !_space.is_attached());
  }
  else
  {
    result = (_ub > 0 && H5Iget_type(_type) == H5I_DATATYPE && _space.is_attached());
  }

  // Postconditions:

  // Exit:

  return result;
}

void
memory::
reserve(pcontainer& xcon)
{
  // Preconditions:

  assert(xcon.is_attached());

  // Body:

  // ISSUE:
  // How much memory should we allocate?
  // The policy here will be to allocate as much as the current
  // selection of xcon.  That policy assumes that one wants to
  // map the current selection, which may be scattered throughout
  // its dataspace, to a compact, contiguous memory buffer.  That
  // may not always be the case, but it's certainly a useful
  // behavior in this test suite. 

  hssize_t npts = H5Sget_select_npoints(xcon.get_space().hid());

  assert(npts >= 0);

  size_t size = H5Tget_size(xcon.get_type());

  _ub    = npts*size;

  _mem   = new char[_ub];

  // Since this hid is obtained from the test suite, in
  // accordance with the rules described in hdf5_handle.h,
  // we should not decrement or close this hid here.

  hsize_t dim = npts;

  hid_t sp = H5Screate_simple(1, &dim, &dim);

  // Incrementing the reference count to sp happens in this call.

  _space.attach(sp);

  // Decrement the reference count, since sp goes out of scope at the
  // end of this function.

  H5Idec_ref(sp);

  _type  = xcon.get_type();

  // In accordance with the rules described in hdf5_handle.h,
  // we have to increment the reference count, since this is
  // another copy of the same hid.

  H5Iinc_ref(_type);

  // Postconditions:

  assert(invariant());
  assert(ub() > 0);

  // Exit:
}

size_t
memory::
ub() const
{
  size_t result;

  // Preconditions:

  // Body:

  result = _ub;

  // Postconditions:

  // Exit:

  return result;
}

void*
memory::
mem()
{
  void* result;

  // Preconditions:

  // Body:

  result = _mem;

  // Postconditions:

  // Exit:

  return result;
}

const void*
memory::
mem() const
{
  void* result;

  // Preconditions:

  // Body:

  result = _mem;

  // Postconditions:

  // Exit:

  return result;
}

bool
memory::
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

  assert(is_attached() ? true : result == false);

  // Exit:

  return result;
}

bool
memory::
is_attached() const
{
  bool result;

  // Preconditions:

  // Body:

  result = (_mem != 0);

  // Postconditions:

  assert(result == (mem() != 0));

  // Exit:

  return result;
}

void
memory::
detach()
{
  // Preconditions:

  // Body:

  if (_mem != 0)
  {
    _space.detach();
    H5Idec_ref(_type);
    delete [] _mem;

    _type = H5I_INVALID_HID;
    _mem  = 0;
    _ub   = 0;
  }

  // Postconditions:

  assert(invariant());
  assert(! is_attached());

  // Exit:
}

ostream&
operator<<(ostream& xos, const memory& xmem)
{
  // Preconditions:

  assert(xos.good());

  // Body:

  xos << "dataspace = ("
      << xmem.get_space()
      << ')';

  // Postconditions:

  // Exit:

  return xos;
}
