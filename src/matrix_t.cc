#include "col_itr.h"
#include "matrix.h"
#include "std_iostream.h"

int
main()
{
  cerr << "`matrix_t' is currently broken.\n";

  exit(0);

  matrix m(2,2,matrix::C);

  cout << m << endl;

  col_itr c(m);

  for (c.start(); ! c.is_done(); c.next())
  {
    cout << c << endl;
  }
}
