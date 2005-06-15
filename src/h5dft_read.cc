
#include "contract.h"
#include "dft_read_perf.h"
#include "dft_namelen.h"
#include "hdf5.h"
#include "std_iostream.h"

/*!
  @file Runs a read test on every container in an HDF5 file.
*/

int
main(int argc, char** argv)
{
  int result;

  // Preconditions:

  // Body:

  if (argc < 2)
  {
    // Then the command line has an insufficient number of arguments.

    cerr << "\tUsage: h5dft_read HDF5_file [HDF5_file...]\n";
    result = 1;
  }
  else
  {
    // Then the command line supplied at least one HDF5 file name.

    result = 0;

    // Loop through each HDF5 file name on the command line.

    for (int i = 1; i < argc; ++i)
    {
      hid_t file;

      // Try to open the HDF5 file.

      H5E_BEGIN_TRY
      {
	file = H5Fopen(argv[i], H5F_ACC_RDONLY, H5P_DEFAULT);
      }
      H5E_END_TRY;

      if (file < 0)
      {
	// Something's wrong with the file.  Skip it.

	cerr << "Failed to open `"
	     << argv[i]
	     << "'.  Skipping it.\n";
	++result;
      }
      else
      {
	// The file appears to be a legitimate HDF5 file.  Measure
	// rates of reading datasets and attributes and report results.

	cout << "Objects encountered in a depth first traversal of `"
	     << argv[i]
	     << "':\n";

	hid_t root = H5Gopen(file, "/");

	assert(root >= 0);

	// Do a quick traversal of the file to get max length of any
	// name in it.  This assists formatting of results.

	dft_namelen len;

	len.traverse(root);

	// Now traverse again to measure i/o rates.

	dft_read_perf tester;

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
