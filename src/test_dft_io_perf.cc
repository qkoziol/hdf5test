
#include <cassert>
#include "dft_io_perf.h"
#include "hdf5.h"
#include "std_iostream.h"

/*!
  @file Runs an io test on every container in an HDF5 file.
*/

int
main(int argc, char** argv)
{
  int result;

  // Preconditions:

  // Body:

  if (argc < 2)
    {
      cerr << "\tUsage: test_dft_io_perf HDF5_file [HDF5_file...]\n";
      result = 1;
    }
  else
    {
      result = 0;

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
	      cerr << "Failed to open `"
		   << argv[i]
		   << "'.  Skipping it.\n";
	      ++result;
	    }
	  else
	    {
	      cout << "Objects encountered in a depth first traversal of `"
		   << argv[i]
		   << "':\n";

	      hid_t root = H5Gopen(file, "/");

	      assert(root >= 0);

	      dft_io_perf tester;

	      tester.traverse(root);

	      cout << "Summary for `"
		   << argv[i]
		   << "': "
		   << tester.success_ct()
	           << " tests succeeded and "
		   << tester.failure_ct()
		   << " tests failed.\n\n";

              H5Gclose(root);
              H5Fclose(file);
	    }
	}
    }

  // Postconditions:

  // Exit:

  return result;
}
