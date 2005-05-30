/*
  Tests iterated (i.e., partial) io.
*/

#include "dataset.h"
#include "hyperslab.h"

int
main()
{
  {
    cerr << "`test_iterated_io' is currently broken." << endl;
    exit(0);
  }
  /*
    Here's a rough idea:

    open a dataset for reading

    here's the analogy:
    an extent describes the bounds of a dataspace.
    a hyperslab describes a subset of a dataspace.

    get its dataspace. now we have access to the extent of this dataspace.

    we ought to be able to create a hyperslab from an extent.  if you
    do that with no further information, you ought to get a hyperslab that
    describes the entire extent.  there's more than one way to create
    a hyperslab that describes the whole extent because the block size
    and spacing can vary.

    once you have a hyperslab, maybe we ought to have a whole host of
    functions that let you do what in organic chemistry are called isomers

    once you have a hyperslab, you ought to be able to make selections
    on it:
        select a dimension
	select a group of dimensions
	select blocks, etc
    perhaps a relatively few such classes of selections that can be
    applied recursively would allow one to express any kind of subsetting
    very economically.

    so, you could make a hyperslab from a dataset's extent, and optionally
    restrict it some suitable way.  use this hyperslab as a reference

    then you could iterate.  during the iteration you would copy the reference
    hyperslab and restrict it even more during each step of the iteration.
    use this restricted hyperslab as the selection on which to do io.
  */

  // Open a dataset for reading.
  // ERROR:
  // This just creates an unattached handle.  Real executable code would
  // have to open this handle.

  dataset c;

  dataspace& sp = c.get_space();

  // Now loop through all the dimensions of the dataspace
  // and do i/o operations one one dimension at a time.

  int d = sp.d();

  hyperslab h(d);

  for (int i = 0; i < d; ++i)
  {
    // Select only the ith dimension of the hyperslab.

    // Now use the hyperslab to restrict the dataspace.

    sp.select(h);

    // Do i/o operation.
  }
}
