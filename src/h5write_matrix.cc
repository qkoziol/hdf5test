#include "contract.h"
#include "dataset.h"
#include "matrix.h"
#include "matrix_writer.h"
#include "memory.h"
#include "temp_file.h"
/*
  What do we want this to do?
  1)  write a matrix of specified size to an HDF5 file
  2)  specify by rows or by columns
  3)  specify how many rows/columns at a time
  4)  specify chunk size

  How?

  How about a class to deal with options or configuration?
 */

/*! @class config
    @brief A class defining the parameters of a matrix write test.
*/

class config
{
 public:

  enum storage {CHUNKED, COMPACT, CONTIGUOUS, EXTERNAL};

  matrix    mat;      ///< The matrix to be written.
  extent    ext;      ///< The extent of the destination dataset.
  tuple     chunk;    ///< Chunk size of the destination dataset.
  bool      by_rows;  ///< True if writing by rows, false if writing by columns.
  unsigned  ct;       ///< The number of rows/columns to write at a time.
  string    filename; ///< The name of the hdf5 file containing the destination dataset.
  string    ds_name;  ///< The name of the destination dataset.
  storage   type;     ///< How the dataset is stored.

  /// Default constructor.

  config();

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
  filename("h5write_matrix.h5"),
  ds_name("matrix"),
  type(CONTIGUOUS)
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

  result = (mat.row_ct() > 0);
  if (!result && xwarn)
  {
    cerr << "rows == 0";
  }
  result = result && (ext.d() == 2);
  result = result && (ext.max_size()[0] >= mat.col_ct());
  result = result && (ext.max_size()[0] == H5S_UNLIMITED ? chunk[0] > 0 : true);
  result = (mat.col_ct() > 0);
  result = result && (ext.max_size()[1] >= mat.row_ct());
  result = result && (ext.max_size()[1] == H5S_UNLIMITED ? chunk[1] > 0 : true);
  result = result && (ct > 0);
  result = result && (by_rows ? ct <= mat.row_ct() : ct <= mat.col_ct());
  result = result && (!filename.empty());
  result = result && (!ds_name.empty());

  // Postconditions:

  // Exit:

  return result;
}

void
usage()
{
  // Preconditions:

  // Body:

  cerr << "usage: h5write_matrix [OPTIONS] HDF5_file dataset\n"
       << "   OPTIONS\n"
       << "      -h            Print this message.\n"
       << "      -m   row col  Define matrix dimensions.\n"
       << "      -c   row col  Define chunk size.  Implies chunked dataset.\n"
       << "      -row n        Write n rows at a time.\n"
       << "      -col n        Write n columns at a time.\n"
       << "      -t   c|e      Force compact or external storage.  Default is contiguous.\n";

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
    // Define the indices of the various options.  A value of -1 indicates
    // option not present or not yet found.

    int dash_h   = -1;
    int dash_m   = -1;
    int dash_c   = -1;
    int dash_row = -1;
    int dash_col = -1;
    int dash_t   = -1;

    filename = argv[argc-2];
    ds_name = argv[argc-1];

    for (int i = 1; i < argc-2;)
    {
      if (dash_h == -1 && strncmp(argv[i], "-h", 2) == 0)
      {
	dash_h = i;
	usage();
	++i;
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
    }

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

    /*
      Set result = 1;
      Make a dataspace.
      Make a dataset creation property list.
      If chunked, specify the chunk size.
      If chunked, contiguous, or compact, specify the layout.
      If external, specify external.
      Open the file if possible, clobbering the contents.  If not, create a new file.
      Create the dataset.
      Make the memory buffer.
      Use matrix_writer to run the test.
      Close resources.
      Set result = 0
     */

    // Set result to an error condition, and reset it to no error if all steps
    // execute correctly.

    result = 1;

    // We'll use a temp_file for this test; it'll be deleted automatically.

    temp_file file(cmdline.filename.c_str());

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

    hid_t ds_hid = H5Dcreate(file.hid(), cmdline.ds_name.c_str(), H5T_NATIVE_INT, file_space, create_plist);

    assert(ds_hid >= 0);

    ds.attach(ds_hid);

    // We're done with the dataspace and creation property list.  Recycle these resources.

    H5Pclose(create_plist);
    H5Sclose(file_space);

    // Now make a memory buffer.  "matrix_writer" will size it dynamically to be big
    // enough for whatever writing it does.  An empty buffer is good enough for now.

    memory mem;

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
  }

  // Postconditions:

  // Exit:

  return result;
}

