#include "dft_ls.h"
#include "contract.h"

#include "attribute.h"
#include "dataset.h"
#include "pcontainer.h"
#include "plist.h"

dft_ls::
dft_ls()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

}

dft_ls::
~dft_ls()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
dft_ls::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = dft::invariant();

  // Postconditions:

  // Exit:

  return result;
}

dft_ls::
dft_ls(const dft_ls& xother)
{
  not_implemented;
}

dft_ls&
dft_ls::
operator=(const dft_ls& xother)
{
  not_implemented;
  return *this;  // keeps pgi compiler happy until this is implemented.
}

void
dft_ls::
preorder_action()
{
  // Preconditions:

  // Body:

  H5I_type_t type = H5Iget_type(current_hid());

  if (type == H5I_DATASET || type == H5I_ATTR)
  {
    pcontainer& current_node(dynamic_cast<pcontainer&>(current()));

    if (dynamic_cast<attribute*>(&current_node) != 0)
    {
      // current node is an attribute.

      cout << "\tfound an attribute named `"
	   << name(true)
	   << "'\n";
    }
    else
    {
      // current node is a dataset.

    cout << "\tfound a ";

      dataset* dset = dynamic_cast<dataset*>(&current_node);

      if (dset->is_contiguous())
	cout << "contiguous";
      if (dset->is_chunked())
      {
	tuple chunk(dset->get_space().d());

	dset->get_chunk_size(chunk);

	cout << "chunked ("
	     << chunk
	     << ')';
      }
      if (dset->is_external())
	cout << "external";
      if (dset->is_compact())
	cout << "compact";

      /*
	hid_t cpl = H5Dget_create_plist(dset->hid());

	plist::write(cpl);

	H5Pclose(cpl);
      */
    cout << " dataset named `"
	 << name(true)
	 << "' with extent "
	 << dset->get_space().get_extent()
	 << '\n';
    }


    current_node.detach();
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
dft_ls::
postorder_action()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
dft_ls::
reset()
{
  // Preconditions:

  // Body:

  // Do nothing.

  // Postconditions:

  assert(invariant());

  // Exit:
}
