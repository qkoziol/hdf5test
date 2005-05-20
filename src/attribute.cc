#include "attribute.h"
#include "contract.h"

const string
attribute::
_type = "attribute";

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
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
attribute::
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
open(hid_t xloc, const string& xname)
{
  bool result;

  // Preconditions:

  assert(H5Iget_type(xloc) == H5I_DATASET || H5Iget_type(xloc) == H5I_DATATYPE || H5Iget_type(xloc) == H5I_GROUP || H5Iget_type(xloc) == H5I_FILE);

  // Body:

  // This open() is more forgiving than H5Aopen_name().  It has the same semantics as
  // H5Gopen(), H5Topen(), and H5Dopen().  Accordingly, we'll try to open the attribute
  // a number of different ways.

  // Assume that xloc is an HDF5 object with an attribute whose name is 'xname' and
  // attempt an open.

  H5E_BEGIN_TRY
  {
    _hid = H5Aopen_name(xloc, xname.c_str());
  }
  H5E_END_TRY;

  if (_hid < 0)
  {
    // Didn't work.  Maybe xname is a pathname whose last segment is an attribute
    // name.  Then xname minus the last segment should be the name of a dataset, group,
    // or datatype, and xloc should be a group or file id.

    string::size_type size  = xname.size();
    string::size_type slash = xname.find_last_of('/');

    string attr_name(xname, slash+1, size-slash-1);
    string host_name(xname, 0, slash+1);

    // Assume attribute host is a dataset and try opening it.

    hid_t host = H5I_INVALID_HID;

    H5E_BEGIN_TRY
    {
      host = H5Dopen(xloc, host_name.c_str());

      if (host <= 0)
      {
	// Not a dataset; try a group.

	H5E_BEGIN_TRY
	{
	  host = H5Gopen(xloc, host_name.c_str());

	  if (host <= 0)
	  {
	    // Not a group; try a datatype.

	    H5E_BEGIN_TRY
	    {
	      host = H5Topen(xloc, host_name.c_str());
	    }
	    H5E_END_TRY;
	  }
	}
	H5E_END_TRY;
      }
    }
    H5E_END_TRY;

    if (host >= 0)
    {
      // Found a candidate host for attribute; now let's see if we can
      // open the attribute.

      _hid = H5Aopen_name(host, attr_name.c_str());

      // Done with host; recycle resources.

      H5Idec_ref(host);
    }
  }

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

const string&
attribute::
type() const
{
  return _type;
}
