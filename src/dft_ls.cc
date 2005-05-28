#include "dft_ls.h"
#include "contract.h"

#include "attribute.h"
#include "pcontainer.h"

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
    const pcontainer& current_node(dynamic_cast<const pcontainer&>(current()));

    cout << "\tfound a";

    if (dynamic_cast<const attribute*>(&current_node) != 0)
    {
      // Kinda klugey, but this test decides whether we eventually
      // print "found a dataset" or "found an attribute".

      cout << 'n';
    }

    cout << ' '
	 << current_node.type()
	 << " named `"
	 << name(true)
	 << "'\n";
     
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
