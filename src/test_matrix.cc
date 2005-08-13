#include "contract.h"
#include "hdf5_file.h"
#include "hyperslab.h"
#include "matrix.h"
#include <ostream>

/*
  Tester for class matrix.
*/

std::string&
test_name()
{
  // Strips off all characters starting with the last . in
  // the name of this file and returns the resulting string.
  // Useful for generating an HDF5 file name specific to this
  // test driver.

  std::string* result = new std::string(__FILE__);
  result->erase(result->find_last_of("."), result->size());
  return *result;
}


void
test_dims_selections()
{

  matrix m(1, 2);

  std::cout << "\tA 1x2 matrix: "
	    << m
	    << std::endl;

  m.put_row(3);

  std::cout << "\tChanged to 3 rows: "
	    << m
	    << std::endl;

  m.put_col(4);

  std::cout << "\tChanged to 4 columns: "
	    << m
	    << std::endl;

  m.reserve(5, 6);

  std::cout << "\tChanged to 5x6 matrix: "
	    << m
	    << std::endl;

  hyperslab result(2);

  std::cout << "\tDefault construction of 2-d hyperslab: "
	    << result
	    << std::endl;

  m.select(result);

  std::cout << "\tSelection of entire matrix: "
	    << result
	    << std::endl;

  for (unsigned i = 0; i < m.row_ct(); ++i)
  {
    std::cout << "\tSelection of row "
	      << i
	      << ": ";

    m.select_row(i, result);

    std::cout << result
	      << std::endl;
  }

  for (unsigned i = 0; i < m.col_ct(); ++i)
  {
    std::cout << "\tSelection of column "
	      << i
	      << ": ";

    m.select_col(i, result);

    std::cout << result
	      << std::endl;
  }

  m.select_rows(1, 3, result);

  std::cout << "\tSelection of rows 1 to 3: "
	    << result
	    << std::endl;

  m.select_cols(4, 2, result);

  std::cout << "\tSelection of columns 4 to 5: "
	    << result
	    << std::endl;

}

int
main()
{
  test_dims_selections();

  hdf5_file f;

  assert(f.create(test_name() + ".h5"));

  hdf5_file::rm(f.hid());
}
