
#include <cassert>
#include <cstdlib>
#include "matrix_writer.h"
#include "plist.h"
#include "temp_file.h"
#include "timer.h"

bool
compact_test(hdf5_file& xfile, matrix_writer& xtest)
{
  bool result;

  // Preconditions:

  assert(xfile.is_attached());

  // Body:

  // Make a dataset creation property list by which to specify a chunked dataset.

  hid_t create_plist = H5Pcreate(H5P_DATASET_CREATE);

  // Define a matrix to be written to the dataset.

  matrix mat(5,7);

  // Define the size of the dataspace.

  extent e(2);

  mat.get_extent(e);

  assert(H5Pset_layout(create_plist, H5D_COMPACT) >= 0);

  assert(plist::is_compact(create_plist));

  // Make the file space.

  hid_t file_space = H5Screate_simple(2, &e.size()[0], &e.max_size()[0]);

  assert(file_space >= 0);

  // Make the compact dataset.

  hid_t ds_id = H5Dcreate(xfile.hid(),
			  "compact",
			  H5T_NATIVE_INT,
			  file_space,
			  create_plist);

  // And attach to it.

  dataset ds(ds_id);

  // Specify to the tester 2 rows at a time.

  xtest.set_access(2, true);

  // Create a memory buffer as the source of the write.

  memory mem;

  // Run the test.

  result = xtest.run_test(mat, mem, ds);

  // Cleanup:

  ds.detach();
  H5Dclose(ds_id);
  H5Sclose(file_space);
  H5Pclose(create_plist);

  // Postconditions:

  assert(xfile.is_attached());

  // Exit:

  return result;
}

bool
contiguous_test(hdf5_file& xfile, matrix_writer& xtest)
{
  bool result;

  // Preconditions:

  assert(xfile.is_attached());

  // Body:

  // Define a matrix to be written to the dataset.

  matrix mat(5,7);

  // Create a dataset to hold the matrix.

  extent e(2);

  mat.get_extent(e);

  hid_t file_space = H5Screate_simple(2, &e.size()[0], &e.max_size()[0]);

  assert(file_space >= 0);

  hid_t ds_id = H5Dcreate(xfile.hid(),
			  "contiguous",
			  H5T_NATIVE_INT,
			  file_space,
			  H5P_DEFAULT);

  assert(ds_id >= 0);

  // Attach the HDF5 dataset to an instance of class "dataset".

  dataset ds(ds_id);

  // Specify how many rows/columns to write at a time, 3 rows in this case.

  xtest.set_access(3, true);

  // Create a memory buffer as the source of the write.

  memory mem;

  // Do the test, writing the matrix in the pieces specified and report the results.

  result = xtest.run_test(mat, mem, ds);

  // Cleanup:

  ds.detach();
  H5Dclose(ds_id);
  H5Sclose(file_space);

  // Postconditions:

  assert(xfile.is_attached());

  // Exit:

  return result;
}

bool
datatype_test(hdf5_file& xfile, matrix_writer& xtest)
{
  bool result;

  // Preconditions:

  assert(xfile.is_attached());

  // Body:

  // Define a matrix to be written to the dataset.

  matrix mat(15,17);

  // Define the datatype associated with the dataset.

  hid_t ds_type = H5T_NATIVE_DOUBLE;

  // Create a dataset to hold the matrix.

  extent e(2);

  mat.get_extent(e);

  hid_t file_space = H5Screate_simple(2, &e.size()[0], &e.max_size()[0]);

  assert(file_space >= 0);

  hid_t ds_id = H5Dcreate(xfile.hid(),
			  "datatype",
			  ds_type,
			  file_space,
			  H5P_DEFAULT);

  assert(ds_id >= 0);

  // Attach the HDF5 dataset to an instance of class "dataset".

  dataset ds(ds_id);

  // Specify how many rows/columns to write at a time, 3 rows in this case.

  xtest.set_access(3, true);

  // Create a memory buffer as the source of the write.

  memory mem;

  // Define the datatype that memory uses to be something different from the
  // dataset.  This should trigger the use of some kind of data conversion during
  // the writes.

  mem.attach(H5T_NATIVE_INT);

  // Do the test, writing the matrix in the pieces specified and report the results.

  result = xtest.run_test(mat, mem, ds);

  // Cleanup:

  ds.detach();
  H5Dclose(ds_id);
  H5Sclose(file_space);

  // Postconditions:

  assert(xfile.is_attached());

  // Exit:

  return result;
}

class chunk_test
{
 public:

  chunk_test();

  /// Run a performance test.  Creates a new chunked dataset named xname, of
  /// specified chunk size.  Writes a matrix of ints to it, xct rows at a
  /// time if xby_rows, and xct columns at a time if !xby_rows.

  bool do_test(const std::string& xname,
	       extent&            xchunk,
	       matrix&            xmat,
	       int                xct,
	       bool               xby_rows);

 protected:

  /// Data members:

  temp_file     _file;

  matrix_writer _test;
};

chunk_test::
chunk_test() : _file("test_chunks.h5")
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(_file.is_attached());

  // Exit:
}

bool
chunk_test::
do_test(const std::string& xname, extent& xchunk, matrix& xmat, int xct, bool xby_rows)
{
  bool result;

  // Preconditions:

  // I'm not going to specify the preconditions; they're complicated, and this is
  // not meant for general use.  Know what you're doing when you call this.

  // Body:

  // Make a dataset creation property list to specify a chunked dataset.

  hid_t create_plist = H5Pcreate(H5P_DATASET_CREATE);

  assert(create_plist >= 0);

  assert(H5Pset_chunk(create_plist, 2, &xchunk.size()[0]) >= 0);

  assert(plist::is_chunked(create_plist));

  // Make the file space.

  {
    std::cout << "creating a chunked dataset with space size = "
	      << xchunk.size()
	      << " and max size = "
	      << xchunk.max_size()
	      << std::endl;
  }
  hid_t file_space = H5Screate_simple(2, &xchunk.size()[0], &xchunk.max_size()[0]);

  assert(file_space >= 0);

  // Make the chunked dataset.

  hid_t ds_id = H5Dcreate(_file.hid(),
			  xname.c_str(),
			  H5T_NATIVE_INT,
			  file_space,
			  create_plist);

  // And attach to it.

  dataset ds(ds_id);

  // Specify to the tester xct columns/rows at a time.

  _test.set_access(xct, xby_rows);

  // Create a memory buffer as the source of the write.

  memory mem;

  // Run the test.

  result = _test.run_test(xmat, mem, ds);

  // Cleanup:

  timer t;

  t.start();

  ds.detach();
  H5Dclose(ds_id);
  H5Sclose(file_space);
  H5Pclose(create_plist);

  t.stop();

  std::cout << "Time to close dataset, dataspace, and dataset create plist is "
	    << t.elapsed()*1000.0
	    << " (ms)."
	    << std::endl;

  // Postconditions:

  assert(_file.is_attached());

  // Exit:

  return result;
}

int
main()
{
  // Make a temporary file to hold the destination dataset.

  hdf5_file scratch;

  scratch.create("test_matrix_writer.h5");

  // And a tester:

  matrix_writer test;

  // And do some tests:

  contiguous_test(scratch, test);
  compact_test(scratch, test);
  datatype_test(scratch, test);

  chunk_test ct;

  extent chunk(2);

  chunk.size() = 1;
  chunk.max_size() = H5S_UNLIMITED;

  matrix m(15,11);

  ct.do_test("by_row", chunk, m, 1, true);
  ct.do_test("by_col", chunk, m, 1, false);
  ct.do_test("two rows", chunk, m, 2, false);
  ct.do_test("four rows", chunk, m, 4, false);
  ct.do_test("eight rows", chunk, m, 8, false);
  ct.do_test("fifteen rows", chunk, m, 11, false);

  matrix m2(128,128);

  ct.do_test("big by col", chunk, m2, 16, false);
  ct.do_test("big by row", chunk, m2, 16, true);

  timer t;

  t.start();

  scratch.detach();

  t.stop();

  std::cout << "Time to close hdf5 file is "
	    << t.elapsed()*1000.0
	    << " (ms)."
	    << std::endl;

  t.start();

  system("sync");

  t.stop();

  std::cout << "Time to flush filesystem buffers is "
	    << t.elapsed()*1000.0
	    << " (ms)."
	    << std::endl;
  // TODO:
  // Things to try:
  // 1)  different datatypes on either end
  // 2)  filtering
}
