#include "contract.h"
#include "dataset.h"
#include "datatype.h"
#include "matrix.h"
#include "matrix_writer.h"
#include "memory.h"
#include "temp_file.h"
#include <unistd.h>

/*!
  @file h5write_matrix.cc writes a matrix of some datatype to a dataset
  and reports on the performance.  The datatype is H5T_NATIVE_INTs by
  default, but may be some other user defined datatype.
*/

/*! @class config
  @brief A class defining the parameters of a matrix write test.
*/

class config
{
 public:

  enum storage {CHUNKED, COMPACT, CONTIGUOUS, EXTERNAL};

  matrix       mat;       ///< The matrix to be written.
  extent       ext;       ///< The extent of the destination dataset.
  tuple        chunk;     ///< Chunk size of the destination dataset.
  bool         by_rows;   ///< True if writing by rows, false if writing by columns.
  unsigned     ct;        ///< The number of rows/columns to write at a time.
  std::string  file_name; ///< The name of the hdf5 file containing the destination dataset.
  std::string  ds_name;   ///< The name of the destination dataset.
  storage      type;      ///< How the dataset is stored.
  hid_t        mt;        ///< Datatype in memory.
  hid_t        ft;        ///< Datatype in file.
  bool         rm;        ///< Delete file before exit?

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
  mat(1,1),
  ext(2),
  chunk(2),
  by_rows(true),
  ct(1),
  file_name("matrix.h5"),
  ds_name("matrix"),
  type(CONTIGUOUS),
  mt(H5I_INVALID_HID),
  ft(H5I_INVALID_HID),
  rm(false)
{
  // Preconditions:

  // Body:

  /*
    Establish a sane, if uninteresting default configuration: a 1x1
    unchunked matrix written by rows (formally, but indistinguishable in
    this case from writing by columns).
  */

  ext.size() = ext.max_size() = 1;
  chunk = 0;

  // Postconditions:

  assert(invariant());
  assert(mat.row_ct() == 1);
  assert(mat.col_ct() == 1);
  assert(ext.d() == 2);
  assert(ext.size()[0] = mat.row_ct());
  assert(ext.size()[1] = mat.col_ct());
  assert(ext.max_size()[0] = mat.row_ct());
  assert(ext.max_size()[1] = mat.col_ct());
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

  if (!(mat.row_ct() > 0))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: matrix has zero rows.\n";
    }
  }
  if (!(ext.d() == 2))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: extent does not have dimension 2.\n";
    }
  }
  if (!(ext.max_size()[0] >= mat.row_ct()))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: extent has fewer rows than matrix.\n";
    }
  }
  if (!(ext.max_size()[0] == H5S_UNLIMITED ? chunk[0] > 0 : true))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: extent has unlimited column length but zero column chunk size.\n";
    }
  }
  if (!(mat.col_ct() > 0))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: matrix has zero columns.\n";
    }
  }
  if (!(ext.max_size()[1] >= mat.col_ct()))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: extent has fewer columns than matrix.\n";
    }
  }
  if (!(ext.max_size()[1] == H5S_UNLIMITED ? chunk[1] > 0 : true))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: extent has unlimited row length but zero row chunk size.\n";
    }
  }
  if (!(ct > 0))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: can't write or fewer rows/cols at a time.\n";
    }
  }
  if (!(by_rows ? ct <= mat.row_ct() : ct <= mat.col_ct()))
  {
    result = false;
    if (xwarn)
    {
      std::cerr << "fatal: can't write more rows/cols at a time than exist in matrix.\n";
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

  std::cerr << "usage: h5write_matrix [OPTIONS] HDF5_file_name dataset_name\n"
	    << "   OPTIONS\n"
	    << "      -h                  Print this message.\n"
	    << "      -m   row col        Define matrix dimensions.\n"
	    << "      -c   row col        Define chunk size.  Implies chunked dataset.\n"
	    << "      -row n              Write n rows at a time.\n"
	    << "      -col n              Write n columns at a time.\n"
	    << "      -t   c|e            Force compact or external storage.  Default is contiguous.\n"
	    << "      -mt  name[,name...] Defines memory datatype.  `name' is an HDF5 predefined type name.\n"
	    << "      -ft  name[,name...] Defines file datatype.  `name' is an HDF5 predefined type name.\n"
	    << "      -rm                 Delete HDF5 file before exiting.\n"
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
  int  dash_m   = -1;
  int  dash_c   = -1;
  int  dash_row = -1;
  int  dash_col = -1;
  int  dash_t   = -1;
  int  dash_mt  = -1;
  int  dash_ft  = -1;
  int  dash_rm  = -1;
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
    else if (dash_ft == -1 && strncmp(argv[i], "-ft", 3) == 0)
    {
      dash_ft  = i;
      ft = datatype::create(argv[i+1]);
      if (ft < 0)
      {
	std::cerr << "-ft argument `"
		  << argv[i+1]
		  << "' is bad.\n";
	result = false;
      }
      i += 2;
    }
    else if (dash_m == -1 && strncmp(argv[i], "-m", 2) == 0)
    {
      dash_m = i;
      mat.put_row(atol(argv[i+1]));
      mat.put_col(atol(argv[i+2]));
      i += 3;
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
    else if (dash_c == -1 && strncmp(argv[i], "-c", 2) == 0)
    {
      dash_c   = i;
      type     = CHUNKED;
      chunk[0] = atol(argv[i+1]);
      chunk[1] = atol(argv[i+2]);
      i += 3;
    }
    else if (dash_t == -1 && strncmp(argv[i], "-t", 2) == 0)
    {
      dash_t   = i;

      if (*argv[i+1] == 'c')
      {
	type = COMPACT;
      }
      else
      {
	type = EXTERNAL;
      }
      i += 2;
    }
    else if (dash_rm == -1 && strncmp(argv[i], "-rm", 3) == 0)
    {
      dash_rm = i;
      rm      = true;
      ++i;
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

    // Define the extent.  In general, the extent is just the size of the matrix
    // and has fixed max dimensions.  But for chunked datasets, we define the
    // size to be 0, the max size unlimited in the direction of dataset extension,
    // and the max size to be the size of a row or column, whichever is appropriate
    // in the other direction.  "matrix_writer" will extend the chunked dataset
    // appropriately as it runs the test.

    if (type != CHUNKED)
    {
      mat.get_extent(ext);
    }
    else
    {
      if (by_rows)
      {
	ext.size()[0]     = 0;
	ext.size()[1]     = mat.col_ct();
	ext.max_size()[0] = H5S_UNLIMITED;
	ext.max_size()[1] = mat.col_ct();
      }
      else
      {
	ext.size()[0]     = mat.row_ct();
	ext.size()[1]     = 0;
	ext.max_size()[0] = mat.row_ct();
	ext.max_size()[1] = H5S_UNLIMITED;
      }
    }

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
  if (ft >= 0)
  {
    H5E_BEGIN_TRY
    {
      H5Tclose(ft);
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

    // We'll use a temp_file for this test; it'll be deleted automatically.

    hdf5_file* file;

    if (cmdline.rm == true)
    {
      file = new temp_file(cmdline.file_name);
    }
    else
    {
      file = new hdf5_file();

      file->create(cmdline.file_name);
    }

    // Make a dataset creation property list to specify dataset storage properties.

    hid_t create_plist = H5Pcreate(H5P_DATASET_CREATE);

    assert(create_plist >= 0);

    // ...and specify the storage properties.

    if (cmdline.type == config::CHUNKED)
    {
      herr_t status = H5Pset_chunk(create_plist, 2, &cmdline.chunk[0]);

      assert(status >= 0);
    }
    else if (cmdline.type == config::COMPACT)
    {
      herr_t status = H5Pset_layout(create_plist, H5D_COMPACT);

      assert(status >= 0);
    }
    else if (cmdline.type == config::CONTIGUOUS)
    {
      herr_t status = H5Pset_layout(create_plist, H5D_CONTIGUOUS);

      assert(status >= 0);
    }
    else if (cmdline.type == config::EXTERNAL)
    {
      herr_t status = H5Pset_external(create_plist, cmdline.ds_name.c_str(), 0, H5F_UNLIMITED);

      assert(status >= 0);
    }

    // Define the dataset's dataspace.

    hid_t file_space = H5Screate_simple(2, &cmdline.ext.size()[0], &cmdline.ext.max_size()[0]);

    assert(file_space >= 0);

    // Make a dataset handle, and then attach it to an HDF5 library representation.

    dataset ds;

    if (cmdline.ft < 0)
    {
      cmdline.ft = H5T_NATIVE_INT;  // default file datatype if not specified on command line.
    }

    hid_t ds_hid = H5Dcreate(file->hid(), cmdline.ds_name.c_str(), cmdline.ft, file_space, create_plist);

    assert(ds_hid >= 0);

    ds.attach(ds_hid);

    // We're done with the dataspace and creation property list.  Recycle these resources.

    H5Pclose(create_plist);
    H5Sclose(file_space);

    // Now make a memory buffer.  "matrix_writer" will size it dynamically to be big
    // enough for whatever writing it does.  An empty buffer is good enough for now.

    if (cmdline.mt < 0)
    {
      cmdline.mt = H5T_NATIVE_INT; // default memory datatype if not specified on command line.
    }

    memory mem(cmdline.mt);

    // At last.  Make a tester and run the test.

    matrix_writer test;

    test.set_access(cmdline.ct, cmdline.by_rows);

    if (test.run_test(cmdline.mat, mem, ds))
    {
      result = 0;
    }
    else
    {
      result = 1;
    }

    // Clean up.

    delete file;

    if (cmdline.type == config::EXTERNAL)
    {
      unlink(cmdline.ds_name.c_str());
    }
  }


  // Postconditions:

  // Exit:

  return result;
}

