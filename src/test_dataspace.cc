#include "dataspace.h"
#include "std_iostream.h"
#include "tuple.h"

int
main()
{

  tuple size(2), max_size(2);

  size = 2;
  max_size = 4;

  dataspace space(size, max_size);

  cout << "\tdimension = "
       << space.d()
       << "\n\tnumber of points = "
       << space.get_extent().npoints()
       << "\n\textent: "
       << space.get_extent()
       << endl;
}
