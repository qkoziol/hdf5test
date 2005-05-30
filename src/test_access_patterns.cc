/*
  Tests io performance of different access
  patterns on various kinds of containers.

  At the moment it just tests whether the
  compiler believes that the class interfaces
  allow specifying various kinds of access
  patterns.
*/

#include "attribute.h"
#include "dataset.h"
#include "dataspace.h"
#include "extent.h"
#include "hyperslab.h"

int
main()
{
  {
    cerr << "`test_access_patterns' is currently broken." << endl;
    exit(0);
  }

  // In this block, we'll test the basic ability to obtain
  // information about size of a dataspace of a contiguous
  // dataset.

  {
    dataset cg;

    // ERROR:
    // cg is not attached and no way to ensure it's really contiguous.

    dataspace d = cg.get_space();

    int ndim = d.get_extent().d(); // dimension of the dataspace

    for (int i = 0; i < ndim; ++i)
    {
      // Retrieve the size of the dataspace's
      // i'th dimension.

      d.get_extent().size(i);

      // ... and the i'th dimension's max size.

      d.get_extent().max_size(i);
    }

    // Get all the sizes and max sizes in array form.

    d.get_extent().size();
    d.get_extent().max_size();
  }


  // Repeat above for an attribute.


  {
    attribute a;

    dataspace& d = a.get_space();

    int ndim = d.get_extent().d(); // dimension of the dataspace

    for (int i = 0; i < ndim; ++i)
    {
      // Retrieve the size of the dataspace's
      // i'th dimension.

      d.get_extent().size(i);

      // ... and the i'th dimension's max size.

      d.get_extent().max_size(i);
    }

    // Get all the sizes and max sizes in array form.

    d.get_extent().size();
    d.get_extent().max_size();
  }


  // We ought to be able to make selections on the dataspaces
  // of every kind of container.

  {
    dataset c;
    hyperslab  h;

    c.get_space().select(h);

    c.get_space().select(dataspace::ALL);

    c.get_space().select(dataspace::NONE);
  }
}
