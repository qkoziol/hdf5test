#include "plist.h"
#include "contract.h"

#include "hdf5.h"

bool
is_contiguous(hid_t xplist)
{
  bool result;

  // Preconditions:

  assert(H5Pget_class(xplist) == H5P_DATASET_CREATE);

  // Body:

  int layout;

  H5Pget(xplist, "layout", &layout);

  if (layout == H5D_CONTIGUOUS)
    result = true;
  else
    result = false;

  // Postconditions:

  // Exit:

  return result;
}

bool
is_chunked(hid_t xplist)
{
  bool result;

  // Preconditions:

  assert(H5Pget_class(xplist) == H5P_DATASET_CREATE);

  // Body:

  int layout;

  H5Pget(xplist, "layout", &layout);

  if (layout == H5D_CHUNKED)
    result = true;
  else
    result = false;

  // Postconditions:

  // Exit:

  return result;
}

bool
is_compact(hid_t xplist)
{
  bool result;

  // Preconditions:

  assert(H5Pget_class(xplist) == H5P_DATASET_CREATE);

  // Body:

  int layout;

  H5Pget(xplist, "layout", &layout);

  if (layout == H5D_COMPACT)
    result = true;
  else
    result = false;

  // Postconditions:

  // Exit:

  return result;
}

herr_t
plist::
write_property(hid_t xhid, const char* xname, void* xiter_data)
{
  // Preconditions:

  // Body:

  size_t size;
  char value[1024];

  assert(H5Pget_size(xhid, xname, &size) >= 0);
  // So, how do we know what sort of thing this value is?
  assert(H5Pget(xhid, xname, &value) >= 0);

  cout << "id = "
       << xhid
       << " size = "
       << size
       << " name = "
       << xname
       << endl;

  // Postconditions:

  // Exit:

  return 0;
}

void
plist::
write(hid_t xhid)
{
  // Preconditions:

  assert(cout.good());
  assert(H5Pget_class(xhid) != H5P_NO_CLASS);

  // Body:

  int index = 0;

  H5Piterate(xhid, &index, write_property, 0);

  // Postconditions:

  // Exit:
}
