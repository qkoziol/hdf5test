#include "dataset.h"
#include "contract.h"

#include "dataspace.h"
#include "hdf5_file.h"
#include "std_string.h"

const string
dataset::
_type_name = "dataset";

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

  detach();

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

  result = pcontainer::invariant();

  // Postconditions:

  // Exit:

  return result;
}

dataset::
dataset(const dataset& xother)
{
  // Preconditions:

  // Body:

  if (xother.is_attached())
  {
    _hid   = xother._hid;
    H5Iinc_ref(_hid);
    _type  = H5Tcopy(xother._type);
    _space = xother._space;
  }
  else
  {
    _hid  = H5I_BADID;
    _type = H5I_BADID;
  }

  // Postconditions:

  assert(invariant());
  // TODO:
  // Somehow express that this and xother are the same.

  // Exit:
}

dataset&
dataset::
operator=(const dataset& xother)
{
  not_implemented;
  return *this;  // keeps the pgi compiler happy until this is implemented.
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
    _type = H5Dget_type(_hid);

    hid_t ds = H5Dget_space(_hid);

    _space.attach(ds);

    H5Idec_ref(ds);

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

  H5Idec_ref(plist);

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

  H5Idec_ref(plist);

  // Postconditions:

  // Exit:

  return result;
}

bool
dataset::
is_external() const
{
  bool result;

  // Preconditions:

  assert(is_attached());

  // Body:

  hid_t plist = H5Dget_create_plist(_hid);

  int ext_ct = H5Pget_external_count(plist);

  if (ext_ct > 0)
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
  {
    _space.attach(xspace);
    _type = H5Tcopy(xtype);
    result = true;
  }
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
  // Preconditions:

  assert(!xfile.is_write_protected());
  assert(H5Iget_type(xtype) == H5I_DATATYPE);
  assert(H5Iget_type(xspace) == H5I_DATASPACE);
  assert(xcreate_plist == H5P_DEFAULT || H5Pget_class(xcreate_plist) == H5P_DATASET_CREATE);

  // Body:

  not_implemented;

  // Postconditions:

  // Exit:

  return false; // keeps the pgi compiler happy until this is implemented.
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

  assert(_hid >= 0);

  H5Idec_ref(plist);

  if (_hid >= 0)
  {
    hid_t space = H5Dget_space(_hid);

    assert(space >= 0);

    _space.attach(space);

    H5Idec_ref(space);

    _type = H5Tcopy(xtype);

    result = true;
  }
  else
  {
    result = false;
  }

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

#if H5_VERS_MINOR==6
	H5Z_filter_t filter = H5Pget_filter(plist, i, &flags, 0, 0, 0, 0);
#else
	H5Z_filter_t filter = H5Pget_filter(plist, i, &flags, 0, 0, 0, 0, 0);
#endif

	if (H5Zfilter_avail(filter) <= 0)
	{
	  result = false;
	  break;
	}
      }
      H5Idec_ref(plist);
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
type_name() const
{
  return _type_name;
}

void
dataset::
attach(hid_t xhid)
{
  // Preconditions:

  assert(H5Iget_type(xhid) == H5I_DATASET);

  // Body:

  if (is_attached())
    detach();

  hdf5_handle::attach(xhid);

  _space.attach(H5Dget_space(_hid));

  _type = H5Dget_type(xhid);

  // Postconditions:

  assert(invariant());
  assert(is_attached());
  assert(get_space().is_attached());

  // Exit:
}

void
dataset::
get_chunk_size(tuple& xresult) const
{

  // Preconditions:

  assert(is_attached());
  assert(is_chunked());
  assert(xresult.d() == get_space().d());

  // Body:

  hid_t plist = H5Dget_create_plist(_hid);

  H5Pget_chunk(plist, get_space().d(), &xresult[0]);

  H5Pclose(plist);

  // Postconditions:

  // Exit:
}

ostream&
operator<<(ostream& xos, const dataset& xds)
{
  // Preconditions:

  assert(xos.good());

  // Body:

  xos << "dataspace = ("
      << xds.get_space()
      << ')';

  if (xds.is_chunked())
  {
    tuple chunk(xds.get_space().d());

    xds.get_chunk_size(chunk);

    xos << " chunk size = ("
	<< chunk
	<< ')';
  }
  else if (xds.is_external())
  {
    xos << " external, file sizes = (";
    {
      hid_t plist = H5Dget_create_plist(xds.hid());

      int ext_ct = H5Pget_external_count(plist);

      tuple size(ext_ct);

      for (int i = 0; i < ext_ct; ++i)
      {
	H5Pget_external(plist, i, 0, 0, 0, &size[i]);
      }

      xos << size
	  << ')';

      H5Pclose(plist);
    }
  }
  else if (xds.is_compact())
  {
    xos << " compact";
  }

  // Postconditions:

  // Exit:

  return xos;
}

dataset::
dataset(hid_t xhid)
{
  // Preconditions:

  assert(H5Iget_type(xhid) == H5I_DATASET);

  // Body:

  _hid = xhid;

  H5Iinc_ref(_hid);

  _type = H5Dget_type(_hid);

  hid_t space = H5Dget_space(_hid);

  _space.attach(space);

  H5Sclose(space);


  // Postconditions:

  assert(invariant());
  assert(is_attached());

  // Exit:
}
