
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

/*! @class config
    @brief A class defining the parameters of a read test.
*/

class config
{
 public:

  bool              is_verbose;      ///< True if output should be verbose.
  int               filename_index;  ///< Index of the HDF5 file name in the command line argument list;
  traverser::filter filter;          ///< Value determines whether to read attributes.

  /// Default constructor.

  config();

  /// Class invariant.  Should always be true when called.  Prints violations to cerr
  /// if xwarn is true.

  bool invariant() const;

  /// Apply command line arguments to configuration.

  bool process_command_line(int argc, char** argv);
};

config::
config() :
  is_verbose(false),
  filename_index(1),
  filter(traverser::ATTRIBUTE)
{
  // Preconditions:

  // Body:

  /*
    Establish a default configuration.
   */

  // Postconditions:

  assert(invariant());
  assert(!is_verbose);
  assert(filter == traverser::ATTRIBUTE);

  // Exit:
}

bool
config::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = true;

  // Postconditions:

  // Exit:

  return result;
}

void
usage()
{
  cerr << "usage: h5dft_read [OPTIONS] HDF5_file [HDF5_file...]\n"
       << "   OPTIONS\n"
       << "      -h  Print this message.\n"
       << "      -a  Include attributes in performance report.\n"
       << "      -v  Verbose output.  Include dataspace and storage characteristics in output.\n";
}

bool
config::
process_command_line(int argc, char** argv)
{
  bool result;

  // Preconditions:

  assert(argv != 0);

  // Body:

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
  //
  //    If more than one optional flag is present, they all must occur as
  //    consecutive args.
  //
  // Optional args:
  //
  //    Any additional args come after the 2nd mandatory arg.  All such args
  //    are interpreted as possible hdf5 files.

  if (argc < 2)
  {
    result = false;
  }
  else
  {
    result = true; // until proven otherwise

    // Define the indices of the various options.  A value of -1 indicates
    // option not present or not yet found.

    int dash_a   = -1;
    int dash_v   = -1;
    int dash_h   = -1;

    filename_index = 1;

    for (int i = 1; i < 4 && i < argc; ++i)
    {
      if (dash_h == -1 && strncmp(argv[i], "-h", 2) == 0)
      {
	dash_h = i;
	++filename_index;
      }
      else if (dash_v == -1 && strncmp(argv[i], "-v", 2) == 0)
      {
	dash_v = i;
	is_verbose = true;
	++filename_index;
      }
      else if (dash_a == -1 && strncmp(argv[i], "-a", 2) == 0)
      {
	dash_a = i;
	filter = traverser::NONE;
	++filename_index;
      }
      else if (*argv[i] == '-')
      {
	// Unknown flag

	result = false;
	++filename_index;
      }
    }

    if (result == true)
    {
      // Parsed ok, but does command line make sense?

      result = invariant();
   
      if (dash_h != -1)
      {
	usage();
      }
    }
  }

    // Postconditions:

  assert(result ? invariant() : true);

  // Exit:

  return result;
}

int
main(int argc, char** argv)
{
  int result;

  // Preconditions:

  assert(argv != 0);

  // Body:

  config cmdline;

  if (! cmdline.process_command_line(argc, argv))
  {
    // Bad command line.

    usage();
    result = 1;
  }
  else
  {
    // Body:

    // Loop through each HDF5 file name on the command line.

    for (int i = cmdline.filename_index; i < argc; ++i)
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

	len.traverse(root, cmdline.filter);

	// Now traverse again to measure i/o rates.

	dft_read_perf tester;

	tester.set_verbose(cmdline.is_verbose);

	tester.traverse(root, cmdline.filter);

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
