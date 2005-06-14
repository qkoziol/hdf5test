#include "matrix_writer.h"

int
main()
{
  temp_file scratch("junk");
  matrix m(2,3);

  matrix_writer junk(scratch, m);

  junk.set_access(2, matrix_writer::BY_COLUMNS);

  junk.run_test();
}

