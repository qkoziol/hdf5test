#include "dataset.h"
#include "contract.h"

#include "dataspace.h"
#include "hdf5_file.h"
#include "std_string.h"

const string
dataset::
_type = "dataset";

dataset::
dataset()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(! is_attached());
  assert(invariant());

  // Exit:
}

dataset::
~dataset()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
dataset::
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

dataset::
dataset(const dataset& xother)
{
  not_implemented;
}

dataset&
dataset::
operator=(const dataset& xother)
{
  not_implemented;
}

bool
dataset::
open(hid_t xhost, const string& xname)
{
  bool result;

  // Preconditions:

  assert(H5Iget_type(xhost) == H5I_FILE || H5Iget_type(xhost) == H5I_GROUP);

  // Body:

  // Attempt to open the dataset.

  H5E_BEGIN_TRY
  {
    _hid = H5Dopen(xhost, xname.c_str());
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

hid_t
dataset::
get_type() const
{
  hid_t result;

  // Preconditions:

  assert(is_attached());

  // Body:

  result = H5Dget_type(_hid);

  // Postconditions:

  assert(H5Iget_type(result) == H5I_DATATYPE);

  // Exit:

  return result;
}

dataspace&
dataset::
get_space() const
{
  dataspace* ptr_to_result;

  // Preconditions:

  assert(is_attached());

  // Body:

  hid_t space = H5Dget_space(_hid);

  ptr_to_result = new dataspace(space);

  // Postconditions:

  assert(ptr_to_result->is_attached());

  // Exit:

  return *ptr_to_result;
}

bool
dataset::
is_contiguous() const
{
  bool result;

  // Preconditions:

  assert(is_attached());

  // Body:

  hid_t plist = H5Dget_create_plist(_hid);

  int layout;

  H5Pget(plist, "layout", &layout);

  if (layout == H5D_CONTIGUOUS)
    result = true;
  else
    result = false;

  H5Pclose(plist);

  // Postconditions:

  // Exit:

  return result;
}

bool
dataset::
is_chunked() const
{
  bool result;

  // Preconditions:

  assert(is_attached());

  // Body:

  hid_t plist = H5Dget_create_plist(_hid);

  int layout;

  H5Pget(plist, "layout", &layout);

  if (layout == H5D_CHUNKED)
    result = true;
  else
    result = false;

  H5Pclose(plist);

  // Postconditions:

  // Exit:

  return result;
}

bool
dataset::
is_compact() const
{
  bool result;

  // Preconditions:

  assert(is_attached());

  // Body:

  hid_t plist = H5Dget_create_plist(_hid);

  int layout;

  H5Pget(plist, "layout", &layout);

  if (layout == H5D_COMPACT)
    result = true;
  else
    result = false;

  H5Pclose(plist);

  // Postconditions:

  // Exit:

  return result;
}

bool
dataset::
create(const hdf5_file& xfile,
       hid_t            xhost,
       const string&    xname,
       hid_t            xtype,
       hid_t            xspace,
       hid_t            xcreate_plist)
{
  bool result;

  // Preconditions:

  assert(!xfile.is_write_protected());
  assert(H5Iget_type(xhost) == H5I_FILE || H5Iget_type(xhost) == H5I_GROUP);
  assert(H5Iget_type(xtype) == H5I_DATATYPE);
  assert(H5Iget_type(xspace) == H5I_DATASPACE);
  // ISSUE
  // The 2nd part of the following fails, even if xcreate_plist is demonstrably a
  // dataset creation property list.  Bug in hdf5-1.6.4?
  assert(unexecutable(xcreate_plist == H5P_DEFAULT || H5Pget_class(xcreate_plist) == H5P_DATASET_CREATE));

  // Body:

  _hid = H5Dcreate(xhost, xname.c_str(), xtype, xspace, xcreate_plist);

  if (_hid >= 0)
    result = true;
  else
    result = false;

  // Postconditions:

  assert(result == is_attached());
  assert(invariant());

  // Exit:

  return result;

}

bool
dataset::
create(const hdf5_file& xfile,
       const string&    xgroup,
       const string&    xname,
       hid_t            xtype,
       hid_t            xspace,
       hid_t            xcreate_plist)
{
  bool result;

  // Preconditions:

  assert(!xfile.is_write_protected());
  assert(H5Iget_type(xtype) == H5I_DATATYPE);
  assert(H5Iget_type(xspace) == H5I_DATASPACE);
  assert(xcreate_plist == H5P_DEFAULT || H5Pget_class(xcreate_plist) == H5P_DATASET_CREATE);

  // Body:

  not_implemented;
}

bool
dataset::
create(const hdf5_file& xfile,
       const string&    xname,
       hid_t            xtype,
       const tuple&     xchunk_size,
       hid_t            xcreate_plist)
{
  bool result;

  // Preconditions:

  assert(!xfile.is_write_protected());
  assert(H5Iget_type(xtype) == H5I_DATATYPE);
  assert(xcreate_plist == H5P_DEFAULT || H5Pget_class(xcreate_plist) == H5P_DATASET_CREATE);
  assert(xchunk_size.is_positive());

  // Body:

  tuple max(xchunk_size);

  max = H5S_UNLIMITED;

  dataspace d(xchunk_size, max);

  // ERROR:
  // we have a plist from args

  hid_t plist = H5Pcreate(H5P_DATASET_CREATE);

  H5Pset_chunk(plist, xchunk_size.d(), &xchunk_size[0]);

  _hid = H5Dcreate(xfile.hid(),
                   xname.c_str(),
                   xtype,
                   d.hid(),
                   plist);

  H5Pclose(plist);

  if (_hid >= 0)
    result = true;
  else
    result = false;

  // Postconditions:

  assert(result == is_attached());
  assert(invariant());
  assert(result == is_chunked());

  // Exit:

  return result;
}


bool
dataset::
is_readable() const
{
  bool result;

  // Preconditions:

  // Body:

  if (is_attached())
  {
    // Only chunked datasets are filtered, so only chunked datasets
    // may encounter the situation where a required filter is unregistered.

    if (is_chunked())
    {
      hid_t plist = H5Dget_create_plist(_hid);

      int n = H5Pget_nfilters(plist);

      result = true;

      for (int i = 0; i < n; ++i)
      {
	unsigned int flags;
	size_t       cd_nelmts;
	unsigned int cd_values;

	H5Z_filter_t filter = H5Pget_filter(plist, i, &flags, 0, 0, 0, 0);

	if (H5Zfilter_avail(filter) <= 0)
	{
	  result = false;
	  break;
	}
      }
      H5Pclose(plist);
    }
    else
    {
      // Not a chunked dataset.

      result = true;
    }
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

const string&
dataset::
type() const
{
  return _type;
}
