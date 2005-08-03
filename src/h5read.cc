#include "config.h"
#include "contract.h"
#include "dataset.h"
#include "hdf5.h"
#include "io_perf.h"
#include "memory.h"
#include "std_cstring.h"
#include "std_iostream.h"
#include "std_iomanip.h"
#include "std_sstream.h"
#include "timer.h"

/*!
  @file h5read.cc Runs a read test on every dataset named on the command
                  line.  Reports bytes read, elapsed time to open, read,
		  and close the dataset, and the transfer rate.
*/

/*! @class config
    @brief A class defining the parameters of a read test.
*/

class config
{
 public:

  bool  is_verbose;      ///< True if output should be verbose.
  int   filename_index;  ///< Index of the HDF5 file name in the command line argument list;

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
  filename_index(1)
{
  // Preconditions:

  // Body:

  /*
    Establish a default configuration.
   */

  // Postconditions:

  assert(invariant());
  assert(!is_verbose);

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
  // Preconditions:

  // Body:

  cerr << "usage: h5read [OPTIONS] HDF5_file dataset [datasets...]\n"
       << "   OPTIONS\n"
       << "      -h  Print this message.\n"
       << "      -v  Verbose output.\n";

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

  if (argc < 3)
  {
    result = false;
  }
  else
  {
    result = true; // until proven otherwise.

    // Define the indices of the various options.  A value of -1 indicates
    // option not present or not yet found.

    int dash_v = -1;
    int dash_h = -1;

    filename_index = 1;

    for (int i = 1; i < 3 && i < argc; ++i)
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
      else if (*argv[i] == '-')
      {
	// Unknown flag

	result = false;
	++i;
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
    // Commmand line is ok.  Try to run the test.

    hid_t file;

    H5E_BEGIN_TRY
    {
      file = H5Fopen(argv[cmdline.filename_index], H5F_ACC_RDONLY, H5P_DEFAULT);
    }
    H5E_END_TRY;

    if (file < 0)
    {
      cerr << "Unable to open file `"
	   << argv[cmdline.filename_index]
	   << "'.\n";
      result = 1;
    }
    else
    {
      // File is open, try reading datasets.

      size_t max_name_len = 12;  // 12 is number of chars in column title "dataset name".

      for (int i = cmdline.filename_index+1; i < argc; ++i)
      {
	size_t size = strlen(argv[i]);

	if (size > max_name_len)
	  max_name_len = size;
      }

      // Print a header

      cout << "Read performance for datasets in "
	   << argv[cmdline.filename_index]
	   << ":\n"
	   << setw(max_name_len+2) << setfill(' ') << left
	   << "dataset name"
	   << setw(13) << left
	   << "  status"
	   << setw(17) << left
	   << "  bytes read (kb)"
	   << setw(16) << left
	   << "  open time (ms)"
	   << setw(16) << left
	   << " read time (ms)"
	   << setw(17) << left
	   << " close time (ms)"
	   << setw(14) << left
	   << "  io rate (mb/s)";
      if (cmdline.is_verbose)
      {
	cout << "  dataset characteristics";
      }
      cout << '\n';

      dataset ds;
      io_perf tester;
      memory  mem;
      timer   close;
      timer   open;
      string  ds_char;

      for (int i = cmdline.filename_index+1; i < argc; ++i)
      {
	cout << setw(max_name_len+2) << left
	     << argv[i];

	open.start();
	ds.open(file, argv[i]);
	open.stop();

	if (ds.is_attached())
	{
	  // Attach memory to same datatype as dataset.

	  mem.attach(ds.get_type());

	  // Allocate enough memory to read entire dataset.

	  mem.reserve(ds);

	  tester.run_test(ds, mem);

	  if (cmdline.is_verbose)
	  {
	    // Sigh...  We want to print dataset characteristics and closing time,
	    // but that's awkward to do.  We can only print dataset characteristics
	    // of an attached dataset, but we gotta detach/close it to get the
	    // closing time.  And we want to print the dataset characteristics
	    // after the closing time to make a neat printout - dataset characteristics
	    // are an arbitrary length, so they need to be last on the output line.
	    // As a bit of a hack I guess we can write dataset characteristics to
	    // a string before closing, then write the string.

	    ostringstream output;

	    output << ds;

	    ds_char = output.str();
	  }

	  close.start();
	  ds.detach();
	  close.stop();

	  if (tester.status() == test::SUCCESS)
	  {
	    double kb      = tester.bytes()/((double)BYTES_PER_KB);
	    double elapsed = tester.elapsed();

	    cout << setw(13) << left
		 << "succeeded"
		 << setw(15) << right << fixed << setprecision(3)
		 << kb
		 << setw(15) << right << fixed << setprecision(3)
		 << open.elapsed()*BYTES_PER_KB
		 << setw(16) << right << fixed << setprecision(3)
		 << elapsed*BYTES_PER_KB
		 << setw(16) << right << fixed << setprecision(3)
		 << close.elapsed()*BYTES_PER_KB
		 << setw(16) << right << fixed << setprecision(3)
		 << kb/elapsed/((double)BYTES_PER_KB);
	    if (cmdline.is_verbose)
	    {
	      cout << "     "
		   << ds_char;
	    }
	    cout << '\n';

	  }
	  else
	  {
	    cout << setw(13) << left
		 << "failed\n";
	  }
	}
	else
	{ 
	  cout << "open failed\n";
	}
      }

      mem.detach();

      H5Fclose(file);
    }
  }

  return result;
}
