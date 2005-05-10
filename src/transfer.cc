#include "transfer.h"
#include "contract.h"

#include "container.h"
#include "hdf5.h"
#include "memory.h"

bool
transfer(const memory& xfrom, container& xto, hid_t xprop_list)
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
transfer(const container& xfrom, memory& xto, hid_t xprop_list)
{
  bool result;

  // Preconditions:

  assert(xfrom.is_attached());
  assert(unexecutable(xfrom and xto selections are compatible));

  // Body:

  // ISSUE:
  // See above.

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

  // Postconditions:

  // Exit:

  return result;
}
