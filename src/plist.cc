#include "plist.h"
#include "contract.h"

#include "hdf5.h"

bool
plist::
is_contiguous(hid_t xplist)
{
  bool result;

  // Preconditions:

  // Body:

  int    layout;
  herr_t status;

  H5E_BEGIN_TRY
  {
    status = H5Pget(xplist, "layout", &layout);
  }
  H5E_END_TRY;

  if (status >= 0)
  {
    if (layout == H5D_CONTIGUOUS)
      result = true;
    else
      result = false;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

bool
plist::
is_chunked(hid_t xplist)
{
  bool result;

  // Preconditions:

  // Body:

  int    layout;
  herr_t status;

  H5E_BEGIN_TRY
  {
    status = H5Pget(xplist, "layout", &layout);
  }
  H5E_END_TRY;

  if (status >= 0)
  {
    if (layout == H5D_CHUNKED)
      result = true;
    else
      result = false;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

bool
plist::
is_compact(hid_t xplist)
{
  bool result;

  // Preconditions:

  // Body:

  int    layout;
  herr_t status;

  H5E_BEGIN_TRY
  {
    status = H5Pget(xplist, "layout", &layout);
  }
  H5E_END_TRY;

  if (status >= 0)
  {
    if (layout == H5D_COMPACT)
      result = true;
    else
      result = false;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

herr_t
write_property(hid_t xhid, const char* xname, void* xiter_data)
{
  // Preconditions:

  assert(xname != 0);

  // Body:

  size_t size;
  char value[1024];

  assert(H5Pget_size(xhid, xname, &size) >= 0);
  // So, how do we know what sort of thing this value is?
  assert(H5Pget(xhid, xname, &value) >= 0);

  std::cout << "id = "
	    << xhid
	    << " size = "
	    << size
	    << " name = "
	    << xname
	    << std::endl;

  // Postconditions:

  // Exit:

  return 0;
}

void
plist::
write(hid_t xhid)
{
  // Preconditions:

  assert(std::cout.good());
  assert(H5Pget_class(xhid) != H5P_NO_CLASS);

  // Body:

  int index = 0;

  H5Piterate(xhid, &index, write_property, 0);

  // Postconditions:

  // Exit:
}
