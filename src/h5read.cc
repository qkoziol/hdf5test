#include "dataset.h"
#include "hdf5.h"
#include "io_perf.h"
#include "memory.h"
#include "std_cstring.h"
#include "std_iostream.h"
#include "std_iomanip.h"

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
       << setw(11)
       << " status  "
       << setw(19)
       << "  bytes read (kb)  "
       << setw(21)
       << "   elapsed time (ms) "
       << setw(16) << left
       << "  io rate (mb/s)"
       << '\n';

  dataset ds;
  io_perf tester;
  memory  mem;

  for (int i = file_name_index+1; i < argc; ++i)
  {
    cout << setw(max_name_len+2) << left
	 << argv[i];

    ds.open(file, argv[i]);

    if (ds.is_attached())
    {
      mem.reserve(ds);

      tester.run_test(ds, mem);

      if (tester.status() == test::SUCCESS)
      {
	double kb      = tester.bytes()/1e3;
	double elapsed = tester.elapsed();

	cout << setw(13) << left
	     << "succeeded"
	     << setw(11) << right << fixed << setprecision(3)
	     << kb
	     << setw(19) << right << fixed << setprecision(3)
	     << elapsed*1e3
	     << "    "
	     << setw(13) << right << fixed << setprecision(3)
	     << kb/elapsed/1e3
	     << '\n';

      }
      else
      {
	cout << setw(13) << left
	     << "failed\n";
      }

      ds.detach();
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
