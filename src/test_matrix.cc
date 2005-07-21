#include "contract.h"
#include "hdf5_file.h"
#include "hyperslab.h"
#include "matrix.h"
#include "std_iostream.h"

/*
  Tester for class matrix.
 */

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
test_dims_selections()
{

  matrix m(1, 2);

  cout << "\tA 1x2 matrix: "
       << m
       << endl;

  m.put_row(3);

  cout << "\tChanged to 3 rows: "
       << m
       << endl;

  m.put_col(4);

  cout << "\tChanged to 4 columns: "
       << m
       << endl;

  m.reserve(5, 6);

  cout << "\tChanged to 5x6 matrix: "
       << m
       << endl;

  hyperslab result(2);

  cout << "\tDefault construction of 2-d hyperslab: "
       << result
       << endl;

  m.select(result);

  cout << "\tSelection of entire matrix: "
       << result
       << endl;

  for (unsigned i = 0; i < m.row_ct(); ++i)
  {
    cout << "\tSelection of row "
	 << i
	 << ": ";

    m.select_row(i, result);

    cout << result
	 << endl;
  }

  for (unsigned i = 0; i < m.col_ct(); ++i)
  {
    cout << "\tSelection of column "
	 << i
	 << ": ";

    m.select_col(i, result);

    cout << result
	 << endl;
  }

  m.select_rows(1, 3, result);

  cout << "\tSelection of rows 1 to 3: "
       << result
       << endl;

  m.select_cols(4, 2, result);

  cout << "\tSelection of columns 4 to 5: "
       << result
       << endl;

}

int
main()
{
  test_dims_selections();

  hdf5_file f;

  assert(f.create(test_name() + ".h5"));

  hdf5_file::rm(f.hid());
}
