#include "contract.h"
#include "temp_file.h"

#include <unistd.h>

temp_file::
temp_file(const string& xname, hid_t xcreation_plist, hid_t xaccess_plist)
{
  // Preconditions:

  // ISSUE:
  // The empty string is not a valid filename, but is any
  // nonempty string valid?

  assert(!xname.empty());

  // ERROR:
  // The following doesn't ensure that the hids are actually property list ids.
  // But there's no public HDF5 function that does ensure such.  This at least
  // prevents the worst errors.
  assert(xcreation_plist >= 0);
  assert(xaccess_plist >= 0);

  // Body:

  _hid  = H5Fcreate(xname.c_str(), H5F_ACC_TRUNC, xcreation_plist, xaccess_plist);
  _name = xname;

  // Postconditions:

  assert(invariant());

  // Exit:
}

temp_file::
~temp_file()
{
  // Preconditions:

  // Body:

  unlink(_name.c_str());

  // Postconditions:

  assert(! exists(_name));

  // Exit:
}

bool
temp_file::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = hdf5_file::invariant();

  result = result && is_attached();

  result = result && !_name.empty();

  // You'd think that a successful call to H5Fcreate() would create a file that
  // HDF5 thinks is an HDF5 file, but you'd be wrong.  So this part of the invariant
  // is deliberately commented out.

  // result = result && is_hdf5(_name);

  result = result && is_writable(_name);

  // Postconditions:

  // exit:

  return result;
}

temp_file::
temp_file(const temp_file& xother)
{
  not_implemented;
}

temp_file&
temp_file::
operator=(const temp_file& xother)
{
  not_implemented;
  return *this;
}
