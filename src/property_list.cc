#include "property_list.h"
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
