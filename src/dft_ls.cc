#include "dft_ls.h"
#include "contract.h"

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
}

void
dft_ls::
preorder_action()
{
  // Preconditions:

  // Body:

  cout << "\tpreorder action, start hid = "
       << start()
       << ". current hid = "
       << current_hid()
       << ".  is_node(current) = "
       << is_node(current_hid())
       << ". depth = "
       << depth()
       << " name = "
       << pathname()
       << endl;

  // Postconditions:

  // ISSUE:
  // One ought to be able to call the class invariant at the exit
  // of any routine.  One also does not expect that the invariant
  // holds in the middle of any class feature.  But preorder_action()
  // and postorder_action() are called in the midst of the traversal.
  // So, are we in the middle of a function or at the end of a
  // function?  Can we write an invariant that works at the
  // end of preorder_action() and postorder_action()?
  //assert(invariant());

  // Exit:
}

void
dft_ls::
postorder_action()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // ISSUE:
  // Same as in preorder_action().
  //assert(invariant());

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
