#include "contract.h"
#include "dataset.h"
#include "datatype.h"
#include "hdf5_file.h"
#include "matrix.h"
#include "matrix_reader.h"
#include "memory.h"
#include <unistd.h>

/*!
  @file h5write_matrix.cc reads a matrix from a dataset and reports on the performance.
  The memory datatype may be specified on the command line to be something different
  from the file datatype, thereby forcing a conversion during data transfer.
*/

/*! @class config
  @brief A class defining the parameters of a matrix read test.
*/

class config
{
 public:

  bool         by_rows;   ///< True if reading by rows, false if reading by columns.
  unsigned     ct;        ///< The number of rows/columns to read at a time.
  std::string  file_name; ///< The name of the hdf5 file containing the source dataset.
  std::string  ds_name;   ///< The name of the source dataset.
  hid_t        mt;        ///< Datatype in memory.

  /// Default constructor.

  config();

  /// Destructor.

  ~config();

  /// Class invariant.  Should always be true when called.  Prints violations to cerr
  /// if xwarn is true.

  bool invariant(bool xwarn=false) const;

  /// Apply command line arguments to configuration.

  bool process_command_line(int argc, char** argv);
};

config::
config() :
  by_rows(true),
  ct(1),
  file_name("matrix.h5"),
  ds_name("matrix"),
  mt(H5I_INVALID_HID)
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());
  assert(by_rows);
  assert(ct == 1);

  // Exit:
}

bool
config::
invariant(bool xwarn) const
{
  bool result;

  // Preconditions:

  // Body:

  result = true; // unless proven otherwise.

  if (!(ct > 0))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: can't write or fewer rows/cols at a time.\n";
    }
  }
  if (!(!file_name.empty()))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: file name cannot be empty.\n";
    }
  }
  if (ds_name.empty())
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: dataset name cannot be empty.\n";
    }
  }

  // Postconditions:

  // Exit:

  return result;
}

void
usage()
{
  // Preconditions:

  // Body:

  std::cerr << "usage: h5read_matrix [OPTIONS]\n"
	    << "   OPTIONS\n"
	    << "      -h                  Print this message.\n"
	    << "      -row n              Write n rows at a time.\n"
	    << "      -col n              Write n columns at a time.\n"
	    << "      -mt  name[,name...] Defines memory datatype.  `name' is an HDF5 predefined type name.\n"
	    << "      -f   file_name      Use `file_name' instead of default matrix.h5.\n"
	    << "      -d   dataset_name   Use `dataset_name' instead of default `matrix'.\n";

  // Postconditions:

  // Exit:
}

bool
config::
process_command_line(int argc, char** argv)
{
  bool result;

  // Preconditions:

  assert(argv != 0);

  // Body:

  result = true;  // until proven otherwise.

  // Define the indices of the various options.  A value of -1 indicates
  // option not present or not yet found.

  int  dash_h   = -1;
  int  dash_row = -1;
  int  dash_col = -1;
  int  dash_mt  = -1;
  int  dash_f   = -1;
  int  dash_d   = -1;

  // ERROR:
  // In various places within this loop, I assume that the existence of command line
  // argument i implies that there also exist valid command line arguments i+1, i+2,...
  // Should deal with possible errors due to nonexistent or invalid args.

  for (int i = 1; i < argc;)
  {
    if (dash_h == -1 && strncmp(argv[i], "-h", 2) == 0)
    {
      dash_h = i;
      ++i;
    }
    else if (dash_mt == -1 && strncmp(argv[i], "-mt", 3) == 0)
    {
      dash_mt  = i;
      mt = datatype::create(argv[i+1]);
      if (mt < 0)
      {
	std::cerr << "-mt argument `"
		  << argv[i+1]
		  << "' is bad.\n";
	result = false;
      }
      i += 2;
    }
    else if (dash_row == -1 && strncmp(argv[i], "-row", 4) == 0)
    {
      dash_row = i;
      by_rows  = true;
      ct       = atol(argv[i+1]);
      i += 2;
    }
    else if (dash_col == -1 && strncmp(argv[i], "-col", 4) == 0)
    {
      dash_col = i;
      by_rows  = false;
      ct       = atol(argv[i+1]);
      i += 2;
    }
    else if (dash_f == -1 && strncmp(argv[i], "-f", 2) == 0)
    {
      dash_f = i;
      file_name = argv[i+1];
      i += 2;
    }
    else if (dash_d == -1 && strncmp(argv[i], "-d", 2) == 0)
    {
      dash_d = i;
      ds_name = argv[i+1];
      i += 2;
    }
    else if (*argv[i] == '-')
    {
      // Unknown flag.

      result = false;
      ++i;
    }
  }

  if (dash_h != -1 && result)
  {
    usage();
  }
  if (result)
  {
    // Command line args make sense so far.

    // Is the configuration valid?  If not, write diagnostic messages to cerr.

    result = invariant(true);
  }

  // Postconditions:

  assert(result ? invariant() : true);

  // Exit:

  return result;
}

config::
~config()
{
  // Preconditions:

  // Body:

  // ISSUE:
  // The datatypes might be compound datatypes, but
  // they also may be immutable predefined datatypes.
  // If they are immutable, we can't close them.  But
  // I don't know any way to determine whether a given
  // datatype is mutable or immutable.  So, instead,
  // we just try to close the datatype, which will succeed
  // for the compound mutable types, and will fail for
  // the immutable predefined types.  Since we suppress
  // the error messages, no harm is done.

  if (mt >= 0)
  {
    H5E_BEGIN_TRY
    {
      H5Tclose(mt);
    }
    H5E_END_TRY;
  }

  // Postconditions:

  // Exit:
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
    // Commmand line is ok.  Try to run the test.

    // Set result to an error condition, and reset it to no error if all steps
    // execute correctly.

    result = 1;

    // Try to open the specified file.

    hdf5_file file;

    if (!file.open(cmdline.file_name))
    {
      std::cerr << "Couldn't open file `"
		<< cmdline.file_name
		<< "'.  Quitting...\n";
    }
    else
    {

      // Now try to open the specified dataset.

      dataset ds;

      ds.open(file.hid(), cmdline.ds_name);

      if (! ds.is_attached())
      {
	std::cerr << "Couldn't open dataset `"
		  << cmdline.ds_name
		  << "' in file `"
		  << cmdline.file_name
		  << "'.  Quitting...\n";
      }
      else
      {
	// Now let's see if this dataset is legit.  It ought to be 2 dimensional.  As an
	// enhancement, one could probably regard a 1-d dataset as a 1-row or 1-col matrix,
	// but we'll leave that for another day.

	unsigned dim = ds.get_space().get_extent().d();

	if (dim != 2)
	{
	  std::cerr << "Dataset `"
		    << cmdline.ds_name
		    << "' doesn't store a matrix because it has dimension "
		    << dim
		    << ".  Quitting...\n";
	}
	else
	{
	  // Define the matrix represented by the dataset.

	  matrix mat(ds);

	  // Now make a memory buffer.  "matrix_reader" will size it dynamically to be big
	  // enough for whatever writing it does.  An empty buffer is good enough for now.

	  if (cmdline.mt < 0)
	  {
	    cmdline.mt = H5T_NATIVE_INT; // default memory datatype if not specified on command line.
	  }

	  memory mem(cmdline.mt);

	  // At last.  Make a tester and run the test.

	  matrix_reader test;

	  test.set_access(cmdline.ct, cmdline.by_rows);

	  if (test.run_test(mat, ds, mem))
	  {
	    result = 0;
	  }
	}
      }
    }
  }

  // Postconditions:

  // Exit:

  return result;
}

