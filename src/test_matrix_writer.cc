#include "matrix_writer.h"
#include "plist.h"
#include "std_cassert.h"
#include "temp_file.h"

int
main()
{
  // Make a temporary file to hold the destination dataset.

  hdf5_file scratch; //("test_matrix_writer.h5");
  scratch.create("test_matrix_writer.h5");

  // Define a matrix to be written to the dataset.

  matrix mat(5,7);

  // Create a dataset to hold the matrix.

  extent e(2);

  e.max_size()[0] = e.size()[0] = mat.row_ct();
  e.max_size()[1] = e.size()[1] = mat.col_ct();

  hid_t file_space = H5Screate_simple(2, &e.size()[0], &e.max_size()[0]);

  assert(file_space >= 0);

  hid_t ds_id = H5Dcreate(scratch.hid(),
			  "contiguous",
			  H5T_NATIVE_INT,
			  file_space,
			  H5P_DEFAULT);

  assert(ds_id >= 0);

  // Attach the HDF5 dataset to an instance of class "dataset".

  dataset ds(ds_id);

  // Create a memory buffer as the source of the writes.

  memory mem;

  // And create a tester.

  matrix_writer test;

  // Specify how many rows/columns to write at a time, 3 rows in this case.

  test.set_access(3, true);

  // Do the test, writing the matrix in the pieces specified and report the results.

  test.run_test(mat, mem, ds);



  // Now make a chunked dataset and test it.



  // Make a dataset creation property list by which to specify a chunked dataset.

  hid_t create_plist = H5Pcreate(H5P_DATASET_CREATE);

  // Size of chunks is 1 column; max size of extent is unlimited.

  e.size()[0] = mat.row_ct();
  e.size()[1] = 1;
  e.max_size() = H5S_UNLIMITED;

  assert(H5Pset_chunk(create_plist, 2, &e.size()[0]) >= 0);

  assert(plist::is_chunked(create_plist));
 
  H5Dclose(ds_id);

  // Specify that the extent of the file space is 1 column.

  H5Sset_extent_simple(file_space, 2, &e.size()[0], &e.max_size()[0]);

  // Make the chunked dataset.

  ds_id = H5Dcreate(scratch.hid(),
		    "chunked",
		    H5T_NATIVE_INT,
		    file_space,
		    create_plist);

  // And attach to it.

  ds.attach(ds_id);

  // Specify to the tester 1 column at a time - consistent with chunk size.
  // ISSUE: I don't like this redundancy.

  test.set_access(1, false);

  H5Pclose(create_plist);

  // Run the test.

  test.run_test(mat, mem, ds);

  H5Sclose(file_space);

  scratch.detach();

}
