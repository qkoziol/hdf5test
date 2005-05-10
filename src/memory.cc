#include "memory.h"
#include "contract.h"

#include "container.h"
#include "hdf5.h"
#include "std_iostream.h"

memory::
memory()
{
  // Preconditions:

  // Body:

  _mem   = 0;
  _ub    = 0;
  _type  = H5I_INVALID_HID;
  _space = H5I_INVALID_HID;

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

  if (_mem != 0)
  {
    H5Idec_ref(_space);
    H5Idec_ref(_type);
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
}


bool
memory::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  if (_mem == 0)
  {
    result = (_ub == 0 && _type == H5I_INVALID_HID && _space ==  H5I_INVALID_HID);
  }
  else
  {
    result = (_ub > 0 && H5Iget_type(_type) == H5I_DATATYPE && H5Iget_type(_space) == H5I_DATASPACE);
  }

  // Postconditions:

  // Exit:

  return result;
}

void
memory::
reserve(const container& xcon)
{
  // Preconditions:

  assert(xcon.is_attached());

  // Body:

  // ISSUE:
  // How much memory should we allocate?
  // One choice is to allocate as
  // much as the container's dataspace.  This could
  // be wasteful.  As an example, suppose we're transferring
  // one column at a time of a very large matrix between
  // memory and container.  If we only need one column at
  // a time in memory, then there's no need to allocate
  // memory to hold the entire matrix.  But, for the
  // moment, this is the easy implementation, so that's
  // what we'll do.

  hsize_t npts = xcon.get_space().get_extent().npoints();

  size_t size = H5Tget_size(xcon.get_type());

  _ub   = npts*size;
  _mem  = new char[_ub];
  _space  = H5Scopy(xcon.get_space().hid());
  _type = H5Tcopy(xcon.get_type());

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

hid_t
memory::
get_type() const
{
  hid_t result;

  // Preconditions:

  // Body:

  result = _type;

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

dataspace&
memory::
get_space() const
{
  dataspace* ptr_to_result;

  // Preconditions:

  assert(ub() > 0);

  // Body:

  ptr_to_result = new dataspace(_space);

  // Postconditions:

  assert(ptr_to_result->is_attached());

  // Exit:

  return *ptr_to_result;
}
