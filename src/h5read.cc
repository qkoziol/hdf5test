#include "config.h"
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
                  line.  Reports bytes read (in kb, where 1 kb = 1e3
                  bytes), elapsed time to open, read, and close the
                  dataset, and the transfer rate (in mb/s, where
                  1 mb = 1e6 bytes).
*/

void
usage()
{
  cerr << "usage: h5read [OPTIONS] HDF5_file dataset [datasets...]\n"
       << "   OPTIONS\n"
       << "      -v   Verbose output\n";
}

int
main(int argc, char** argv)
{
  bool verbose = false;
  int  file_name_index = 1;

  // Command line sanity check:

  // Command line must contain at least 3 args.  The first
  // is the name of the program.  If the 2nd arg is not -v, then
  // the 2nd is the name of an HDF5 file, and the 3rd is the
  // name of a dataset in that file.
  //
  // If the 2nd argv is "-v", then the file and dataset names
  // are args 3 and 4.
  //
  // Additional args are interpreted as additional dataset names.

  if (argc < 3)
  {
    usage();
    exit(1);
  }

  if (strncmp(argv[1], "-v", 2) == 0)
  {
    verbose = true;
    file_name_index = 2;
  }

  if (argc < 4 && verbose)
  {
    usage();
    exit(1);
  }

  // Command line is ok.  Try to open HDF5 file.

  hid_t file;

  H5E_BEGIN_TRY
  {
    file = H5Fopen(argv[file_name_index], H5F_ACC_RDONLY, H5P_DEFAULT);
  }
  H5E_END_TRY;

  if (file < 0)
  {
    cerr << "Unable to open file `"
	 << argv[file_name_index]
	 << "'.\n";
    exit(1);
  }

  // File is open, try reading datasets.

  size_t max_name_len = 12;  // 12 is number of chars in "dataset name".

  for (int i = file_name_index+1; i < argc; ++i)
  {
    size_t size = strlen(argv[i]);

    if (size > max_name_len)
      max_name_len = size;
  }

  // Print a header

  cout << "Read performance for datasets in "
       << argv[file_name_index]
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
       << "  io rate (mb/s)"
       << "  dataset characteristics"
       << '\n';

  dataset ds;
  io_perf tester;
  memory  mem;
  timer   close;
  timer   open;

  for (int i = file_name_index+1; i < argc; ++i)
  {
    cout << setw(max_name_len+2) << left
	 << argv[i];

    open.start();
    ds.open(file, argv[i]);
    open.stop();

    if (ds.is_attached())
    {
      mem.reserve(ds);

      tester.run_test(ds, mem);

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

      string ds_char = output.str();

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
	     << kb/elapsed/((double)BYTES_PER_KB)
	     << "     "
	     << ds_char
	     << '\n';

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

  exit(0);
}
