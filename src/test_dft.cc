
#include <cassert>
#include "dft.h"
#include "hdf5.h"
#include "std_string.h"

string&
test_name()
{
  // Strips off all characters starting with the last . in
  // the name of this file and returns the resulting string.
  // Useful for generating an HDF5 file name specific to this
  // test driver.

  string* result = new string(__FILE__);
  result->erase(result->find_last_of("."), result->size());
  return *result;
}

void
make_hdf5_file()
{

  string name(test_name() + ".h5");
 
  // This creates a file, and implicitly creates a root group.  Everything in
  // the file will be contained in the root group.

  hid_t file = H5Fcreate(name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  assert(file >= 0);

  hid_t space = H5Screate(H5S_SCALAR);

  assert(space >= 0);

  // This creates a dataset named "1" in the root group.  That is, there's a hard
  // link from the root group to this dataset and the link is named "1".

  hid_t ds1 = H5Dcreate(file, "1", H5T_NATIVE_INT, space, H5P_DEFAULT);

  assert(ds1 >= 0);

  // This creates a dataset named "2" in the root group.  That is, there's a hard
  // link from the root group to this dataset and the link is named "2".

  hid_t ds2 = H5Dcreate(file, "2", H5T_NATIVE_INT, space, H5P_DEFAULT);

  assert(ds2 >= 0);

  // This creates a group named 3 contained in the file's root group.
  // Currently, it's empty.  More precisely, this creates a new group and
  // a hard link named "3" from the root group to this group.

  hid_t g2 = H5Gcreate(file, "3", 0);

  assert(g2 >= 0);

  // Now let's create a link from the new group, "3", to the dataset
  // named "2".  More precisely, there are now 3 objects in the
  // file, 2 datasets and a group, and links from the root group to
  // each of these objects.  The links are named "1", "2", and "3".
  // We want to add another link from the last group to the 2nd
  // dataset and we want to call the link "4".

  herr_t rtn = H5Glink(file, H5G_LINK_HARD, "2", "3/4");

  assert(rtn >= 0);
  
  H5Gclose(g2);
  H5Dclose(ds2);
  H5Dclose(ds1);
  H5Sclose(space);
  H5Fclose(file);
}

int
main()
{
  make_hdf5_file();

  string name(test_name() + ".h5");

  hid_t file = H5Fopen(name.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

  assert(file >= 0);

  hid_t root = H5Gopen(file, "/");

  assert(root >= 0);

  dft visitor;

  visitor.traverse(root);

  H5Gclose(root);
  H5Fclose(file);
}
