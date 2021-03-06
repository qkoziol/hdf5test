
#include <cassert>
#include "bft_ls.h"
#include "hdf5.h"
#include <iostream>

/*
  An HDF5 contents lister reminiscent of h5ls.
*/

int
main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cerr << "\tUsage: test_bft_ls HDF5_file [HDF5_file...]\n";
    exit(1);
  }
  else
  {
    for (int i = 1; i < argc; ++i)
    {
      hid_t file;

      H5E_BEGIN_TRY
      {
	file = H5Fopen(argv[i], H5F_ACC_RDONLY, H5P_DEFAULT);
      }
      H5E_END_TRY;

      if (file < 0)
      {
	std::cerr << "Failed to open `"
		  << argv[i]
		  << "'.  Skipping it.\n";
      }
      else
      {
	std::cout << "Objects encountered in a breadth first traversal of `"
		  << argv[i]
		  << "':\n";

	hid_t root = H5Gopen(file, "/");

	assert(root >= 0);

	bft_ls lister;

	lister.traverse(root);
      }
    }
  }
}
