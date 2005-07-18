
#include "contract.h"
#include "dft_read_perf.h"
#include "dft_namelen.h"
#include "hdf5.h"
#include "std_cstring.h"
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
  cerr << "usage: h5dft_read [OPTIONS] HDF5_file [HDF5_file...]\n"
       << "   OPTIONS\n"
       << "      -a   Include attributes in performance report.\n"
       << "      -v   Verbose output.  Include dataspace and storage characteristics in output.\n";
}

int
main(int argc, char** argv)
{

  // Preconditions:

  // Command line sanity check:



  // The command line must contain exactly 2 mandatory args.  It may
  // contain 0, 1, or 2 optional flags, and may contain any number of
  // optional args.  Any flags must be supplied after the 1st mandatory
  // arg, and before the 2nd mandatory arg.
  //
  // Mandatory args:
  //
  //    1st:  name of the program
  //    2nd:  name of an hdf5 file.
  //
  // Optional flags:
  //
  //    -a:  include attributes in measurements and output.
  //    -v:  verbose output; print dataspace and dataset storage characteristics.
  //    -av: equivalent to -a -v
  //    -va: equivalent to -av
  //
  //    If more than one optional flag is present, they all must occur as
  //    consecutive args.
  //
  // Optional args:
  //
  //    Any additional args come after the 2nd mandatory arg.  All such args
  //    are interpreted as possible hdf5 file names.
  //

  if (argc < 2)
  {
    // Insufficient number of args.

    usage();
    exit(1);
  }


  // Body:

  // Parse command line and set options.

  traverser::filter filter = traverser::ATTRIBUTE;
  int               file_name_index = argc-1;  // index of 1st file name on command line.
  int               result = 0;
  bool              verbose = false;

  for (int i = 1; i < file_name_index; ++i)
  {
    if (strncmp(argv[i], "-av", 3) == 0)
    {
      filter = traverser::NONE;
      verbose = true;
    }
    else if (strncmp(argv[i], "-va", 3) == 0)
    {
      filter = traverser::NONE;
      verbose = true;
    }
    else if (strncmp(argv[i], "-a", 2) == 0)
    {
      filter  = traverser::NONE;
    }
    else if (strncmp(argv[i], "-v", 2) == 0)
    {
      verbose = true;
    }
    else
    {
      file_name_index = i;
    }
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

      tester.set_verbose(verbose);

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
