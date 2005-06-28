
#include "contract.h"
#include "dft_read_perf.h"
#include "dft_namelen.h"
#include "hdf5.h"
#include "std_iostream.h"

/*!
  @file h5dft_read.cc Runs a read test on every dataset in an HDF5 file
                      (plus every attribute with the -a option).  Reports
                      bytes read (in kb, where 1 kb = 1e3 bytes), elapsed
                      time to read the data, and the transfer rate (in
                      mb/s, where 1 mb = 1e6 bytes).
*/

void
usage()
{
  cerr << "usage: h5dft_read [OPTIONS] HDF5_file dataset [datasets...]\n"
       << "   OPTIONS\n"
       << "      -a   Include attributes in performance report.\n";
}

int
main(int argc, char** argv)
{

  // Preconditions:

  // Command line sanity check:

  // Command line must contain at least 2 args.  The first
  // is the name of the program.  If the 2nd arg is not -a, then
  // the 2nd is the name of an HDF5 file.
  //
  // If the 2nd argv is "-a", then the 3rd arg is the name of an
  // HDF5 file.
  //
  // Additional args are interpreted as additional HDF5 file names.

  if (argc < 2 || (argc < 3 && strncmp(argv[1], "-a", 2) == 0))
  {
    // Then the command line has an insufficient number of arguments.

    usage();
    exit(1);
  }

  // Body:

  traverser::filter filter;
  int               file_name_index;
  int               result = 0;

  if (strncmp(argv[1], "-a", 2) == 0)
  {
    filter = traverser::NONE;
    file_name_index = 2;
  }
  else
  {
    filter = traverser::ATTRIBUTE;
    file_name_index = 1;
  }

  // Loop through each HDF5 file name on the command line.

  for (int i = file_name_index; i < argc; ++i)
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

      len.traverse(root, filter);

      // Now traverse again to measure i/o rates.

      dft_read_perf tester;

      tester.traverse(root, filter);

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

  // Postconditions:

  // Exit:

  return result;
}
