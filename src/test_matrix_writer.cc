#include "matrix_writer.h"

int
main()
{
  temp_file scratch("test_matrix_writer.h5");

  matrix m(60,60);

  matrix_writer test(scratch, m);

  test.set_access(2, matrix_writer::BY_COLUMNS);

  test.run_test();
}

