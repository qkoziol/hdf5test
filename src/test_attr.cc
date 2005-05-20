#include "attribute.h"
#include "hdf5.h"
#include "std_cassert.h"
#include "std_iostream.h"

int
main()
{
  hid_t file = H5I_INVALID_HID;

  H5E_BEGIN_TRY
  {
    file = H5Fopen("../../testfiles/file5.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
  }
  H5E_END_TRY;

  if (file >= 0)
  {
    attribute attr;

    attr.open(file, "/string");

    assert(attr.is_attached());

    cout << "\tSucceeded in opening attribute `/string'.\n";
  }
  else
  {
    cerr << "\t`file5.h5' doesn't exist.  Can't run test.\n";
  }
}
