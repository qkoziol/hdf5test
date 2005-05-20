#include "bft_ls.h"
#include "contract.h"

#include "attribute.h"
#include "pcontainer.h"

bft_ls::
bft_ls()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

}

bft_ls::
~bft_ls()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
bft_ls::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = bft::invariant();

  // Postconditions:

  // Exit:

  return result;
}

bft_ls::
bft_ls(const bft_ls& xother)
{
  not_implemented;
}

bft_ls&
bft_ls::
operator=(const bft_ls& xother)
{
  not_implemented;
}

void
bft_ls::
visit_action()
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
bft_ls::
reset()
{
  // Preconditions:

  // Body:

  // Do nothing.

  // Postconditions:

  assert(invariant());

  // Exit:
}
