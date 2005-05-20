#include "transfer.h"
#include "contract.h"

#include "hdf5.h"
#include "memory.h"
#include "pcontainer.h"

// ISSUE:
// We don't have sufficient preconditions to ensure that a
// transfer will succeed.  It may be impossible to ensure
// success with preconditions, but we're not even covering
// the obvious conditions, like:
// 1) The HDF5 file was opened RDWR if we want to write to it.
// 2) The file underlying an external dataset actually exists
//    and is appropriately readable/writable.

bool
transfer(const memory& xfrom, pcontainer& xto, hid_t xprop_list)
{
  bool result;

  // Preconditions:

  assert(xto.is_attached());
  assert(unexecutable(xfrom and xto selections are compatible));

  // Body:

  // ISSUE:
  // Both here and in the other transfer function, we have code
  // the tests the dynamic type of an argument and selects
  // behavior depending on that type.  This suggests poor
  // object oriented design, since object oriented languages
  // supply the machinery to do precisely this task behind
  // the scenes.
  // We could avoid such code by having transfer methods
  // in each container class, then invoking these methods
  // here.  Dynamic binding would choose the correct
  // underlying implementation, H5Dwrite() or H5Awrite().
  // However, putting i/o machinery into the container
  // classes perpetuates the asymmetry of the HDF5 library
  // that "transfer" is trying to avoid.  (And it's a lot more
  // work than the small if statement here.)  So, we're left with
  // putting explicit dynamic binding code here.

  if(H5Iget_type(xto.hid()) == H5I_DATASET)
  {
    herr_t rtn = H5Dwrite(xto.hid(),
			  xfrom.get_type(),
			  xfrom.get_space().hid(),
			  xto.get_space().hid(),
			  xprop_list,
			  xfrom.mem());
    if (rtn >= 0)
      result = true;
    else
      result = false;
  }
  else
  {
    herr_t rtn = H5Awrite(xto.hid(),
			  xfrom.get_type(),
			  // xfrom.get_space().hid(),
			  // xto.get_space().hid(),
			  // xprop_list,
			  xfrom.mem());
    if (rtn >= 0)
      result = true;
    else
      result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

bool
transfer(const pcontainer& xfrom, memory& xto, hid_t xprop_list)
{
  bool result;

  // Preconditions:

  assert(unexecutable(xfrom and xto selections are compatible));

  // Body:

  // ISSUE:
  // See above.

  if (xfrom.is_readable())
  {
    if(H5Iget_type(xfrom.hid()) == H5I_DATASET)
    {
      herr_t rtn = H5Dread(xfrom.hid(),
			   xto.get_type(),
			   xto.get_space().hid(),
			   xfrom.get_space().hid(),
			   xprop_list,
			   xto.mem());
      if (rtn >= 0)
	result = true;
      else
	result = false;
    }
    else
    {
      herr_t rtn = H5Aread(xfrom.hid(),
			   xto.get_type(),
			   // xfrom.get_space().hid(),
			   // xto.get_space().hid(),
			   // xprop_list,
			   xto.mem());
      if (rtn >= 0)
	result = true;
      else
	result = false;
    }
  }
  else
  {
    // Not readable.

    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}
