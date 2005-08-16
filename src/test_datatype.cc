#include "hdf5.h"

#include "contract.h"
#include "datatype.h"
#include <iostream>

struct map
{
  const char* name;
  hid_t       hid;
};

bool
test_map()
{
  bool result;

  // Preconditions:

  // Body:

  struct map test[128] = {{"H5T_IEEE_F32BE",          H5T_IEEE_F32BE},
			  {"H5T_IEEE_F32LE",          H5T_IEEE_F32LE},
			  {"H5T_IEEE_F64BE",          H5T_IEEE_F64BE},
			  {"H5T_IEEE_F64LE",          H5T_IEEE_F64LE},
			  {"H5T_STD_I8BE",            H5T_STD_I8BE},
			  {"H5T_STD_I8LE",            H5T_STD_I8LE},
			  {"H5T_STD_I16BE",           H5T_STD_I16BE},
			  {"H5T_STD_I16LE",           H5T_STD_I16LE},
			  {"H5T_STD_I32BE",           H5T_STD_I32BE},
			  {"H5T_STD_I32LE",           H5T_STD_I32LE},
			  {"H5T_STD_I64BE",           H5T_STD_I64BE},
			  {"H5T_STD_I64LE",           H5T_STD_I64LE},
			  {"H5T_STD_U8BE",            H5T_STD_U8BE},
			  {"H5T_STD_U8LE",            H5T_STD_U8LE},
			  {"H5T_STD_U16BE",           H5T_STD_U16BE},
			  {"H5T_STD_U16LE",           H5T_STD_U16LE},
			  {"H5T_STD_U32BE",           H5T_STD_U32BE},
			  {"H5T_STD_U32LE",           H5T_STD_U32LE},
			  {"H5T_STD_U64BE",           H5T_STD_U64BE},
			  {"H5T_STD_U64LE",           H5T_STD_U64LE},
			  {"H5T_STD_B8BE",            H5T_STD_B8BE},
			  {"H5T_STD_B8LE",            H5T_STD_B8LE},
			  {"H5T_STD_B16BE",           H5T_STD_B16BE},
			  {"H5T_STD_B16LE",           H5T_STD_B16LE},
			  {"H5T_STD_B32BE",           H5T_STD_B32BE},
			  {"H5T_STD_B32LE",           H5T_STD_B32LE},
			  {"H5T_STD_B64BE",           H5T_STD_B64BE},
			  {"H5T_STD_B64LE",           H5T_STD_B64LE},
			  {"H5T_UNIX_D32BE",          H5T_UNIX_D32BE},
			  {"H5T_UNIX_D32LE",          H5T_UNIX_D32LE},
			  {"H5T_UNIX_D64BE",          H5T_UNIX_D64BE},
			  {"H5T_UNIX_D64LE",          H5T_UNIX_D64LE},
			  {"H5T_INTEL_I8",            H5T_INTEL_I8},
			  {"H5T_INTEL_I16",           H5T_INTEL_I16},
			  {"H5T_INTEL_I32",           H5T_INTEL_I32},
			  {"H5T_INTEL_I64",           H5T_INTEL_I64},
			  {"H5T_INTEL_U8",            H5T_INTEL_U8},
			  {"H5T_INTEL_U16",           H5T_INTEL_U16},
			  {"H5T_INTEL_U32",           H5T_INTEL_U32},
			  {"H5T_INTEL_U64",           H5T_INTEL_U64},
			  {"H5T_INTEL_B8",            H5T_INTEL_B8},
			  {"H5T_INTEL_B16",           H5T_INTEL_B16},
			  {"H5T_INTEL_B32",           H5T_INTEL_B32},
			  {"H5T_INTEL_B64",           H5T_INTEL_B64},
			  {"H5T_INTEL_F32",           H5T_INTEL_F32},
			  {"H5T_INTEL_F64",           H5T_INTEL_F64},
			  {"H5T_ALPHA_I8",            H5T_ALPHA_I8},
			  {"H5T_ALPHA_I16",           H5T_ALPHA_I16},
			  {"H5T_ALPHA_I32",           H5T_ALPHA_I32},
			  {"H5T_ALPHA_I64",           H5T_ALPHA_I64},
			  {"H5T_ALPHA_U8",            H5T_ALPHA_U8},
			  {"H5T_ALPHA_U16",           H5T_ALPHA_U16},
			  {"H5T_ALPHA_U32",           H5T_ALPHA_U32},
			  {"H5T_ALPHA_U64",           H5T_ALPHA_U64},
			  {"H5T_ALPHA_B8",            H5T_ALPHA_B8},
			  {"H5T_ALPHA_B16",           H5T_ALPHA_B16},
			  {"H5T_ALPHA_B32",           H5T_ALPHA_B32},
			  {"H5T_ALPHA_B64",           H5T_ALPHA_B64},
			  {"H5T_ALPHA_F32",           H5T_ALPHA_F32},
			  {"H5T_ALPHA_F64",           H5T_ALPHA_F64},
			  {"H5T_NATIVE_CHAR",         H5T_NATIVE_CHAR},
			  {"H5T_NATIVE_SCHAR",        H5T_NATIVE_SCHAR},
			  {"H5T_NATIVE_UCHAR",        H5T_NATIVE_UCHAR},
			  {"H5T_NATIVE_SHORT",        H5T_NATIVE_SHORT},
			  {"H5T_NATIVE_USHORT",       H5T_NATIVE_USHORT},
			  {"H5T_NATIVE_INT",          H5T_NATIVE_INT},
			  {"H5T_NATIVE_UINT",         H5T_NATIVE_UINT},
			  {"H5T_NATIVE_LONG",         H5T_NATIVE_LONG},
			  {"H5T_NATIVE_ULONG",        H5T_NATIVE_ULONG},
			  {"H5T_NATIVE_LLONG",        H5T_NATIVE_LLONG},
			  {"H5T_NATIVE_ULLONG",       H5T_NATIVE_ULLONG},
			  {"H5T_NATIVE_FLOAT",        H5T_NATIVE_FLOAT},
			  {"H5T_NATIVE_DOUBLE",       H5T_NATIVE_DOUBLE},
			  {"H5T_NATIVE_LDOUBLE",      H5T_NATIVE_LDOUBLE},
			  {"H5T_NATIVE_B8",           H5T_NATIVE_B8},
			  {"H5T_NATIVE_B16",          H5T_NATIVE_B16},
			  {"H5T_NATIVE_B32",          H5T_NATIVE_B32},
			  {"H5T_NATIVE_B64",          H5T_NATIVE_B64},
			  {"H5T_NATIVE_OPAQUE",       H5T_NATIVE_OPAQUE},
			  {"H5T_NATIVE_HADDR",        H5T_NATIVE_HADDR},
			  {"H5T_NATIVE_HSIZE",        H5T_NATIVE_HSIZE},
			  {"H5T_NATIVE_HSSIZE",       H5T_NATIVE_HSSIZE},
			  {"H5T_NATIVE_HERR",         H5T_NATIVE_HERR},
			  {"H5T_NATIVE_HBOOL",        H5T_NATIVE_HBOOL},
			  {"H5T_NATIVE_INT8",         H5T_NATIVE_INT8},
			  {"H5T_NATIVE_UINT8",        H5T_NATIVE_UINT8},
			  {"H5T_NATIVE_INT_LEAST8",   H5T_NATIVE_INT_LEAST8},
			  {"H5T_NATIVE_UINT_LEAST8",  H5T_NATIVE_UINT_LEAST8},
			  {"H5T_NATIVE_INT_FAST8",    H5T_NATIVE_INT_FAST8},
			  {"H5T_NATIVE_UINT_FAST8",   H5T_NATIVE_UINT_FAST8},
			  {"H5T_NATIVE_INT16",        H5T_NATIVE_INT16},
			  {"H5T_NATIVE_UINT16",       H5T_NATIVE_UINT16},
			  {"H5T_NATIVE_INT_LEAST16",  H5T_NATIVE_INT_LEAST16},
			  {"H5T_NATIVE_UINT_LEAST16", H5T_NATIVE_UINT_LEAST16},
			  {"H5T_NATIVE_INT_FAST16",   H5T_NATIVE_INT_FAST16},
			  {"H5T_NATIVE_UINT_FAST16",  H5T_NATIVE_UINT_FAST16},
			  {"H5T_NATIVE_INT32",        H5T_NATIVE_INT32},
			  {"H5T_NATIVE_UINT32",       H5T_NATIVE_UINT32},
			  {"H5T_NATIVE_INT_LEAST32",  H5T_NATIVE_INT_LEAST32},
			  {"H5T_NATIVE_UINT_LEAST32", H5T_NATIVE_UINT_LEAST32},
			  {"H5T_NATIVE_INT_FAST32",   H5T_NATIVE_INT_FAST32},
			  {"H5T_NATIVE_UINT_FAST32",  H5T_NATIVE_UINT_FAST32},
			  {"H5T_NATIVE_INT64",        H5T_NATIVE_INT64},
			  {"H5T_NATIVE_UINT64",       H5T_NATIVE_UINT64},
			  {"H5T_NATIVE_INT_LEAST64",  H5T_NATIVE_INT_LEAST64},
			  {"H5T_NATIVE_UINT_LEAST64", H5T_NATIVE_UINT_LEAST64},
			  {"H5T_NATIVE_INT_FAST64",   H5T_NATIVE_INT_FAST64},
			  {"H5T_NATIVE_UINT_FAST64",  H5T_NATIVE_UINT_FAST64},
			  {"H5T_STD_I8BE",            H5T_STD_I8BE},
			  {"H5T_STD_I8LE",            H5T_STD_I8LE},
			  {"H5T_STD_I16BE",           H5T_STD_I16BE},
			  {"H5T_STD_I16LE",           H5T_STD_I16LE},
			  {"H5T_STD_I32BE",           H5T_STD_I32BE},
			  {"H5T_STD_I32LE",           H5T_STD_I32LE},
			  {"H5T_STD_I64BE",           H5T_STD_I64BE},
			  {"H5T_STD_I64LE",           H5T_STD_I64LE},
			  {"H5T_STD_U8BE",            H5T_STD_U8BE},
			  {"H5T_STD_U8LE",            H5T_STD_U8LE},
			  {"H5T_STD_U16BE",           H5T_STD_U16BE},
			  {"H5T_STD_U16LE",           H5T_STD_U16LE},
			  {"H5T_STD_U32BE",           H5T_STD_U32BE},
			  {"H5T_STD_U32LE",           H5T_STD_U32LE},
			  {"H5T_STD_U64BE",           H5T_STD_U64BE},
			  {"H5T_STD_U64LE",           H5T_STD_U64LE},
			  {"H5T_IEEE_F32BE",          H5T_IEEE_F32BE},
			  {"H5T_IEEE_F32LE",          H5T_IEEE_F32LE},
			  {"H5T_IEEE_F64BE",          H5T_IEEE_F64BE},
			  {"H5T_IEEE_F64LE",          H5T_IEEE_F64LE}};

  int nerr = 0;

  for (int i = 0; i < 128; ++i)
  {
    hid_t range = datatype::predefined(test[i].name);

    if (test[i].hid != range)
    {
      std::cerr << "\tmap is incorrect at "
		<< test[i].name
		<< ".  It returns "
		<< range
		<< " but it should return "
		<< test[i].hid
		<< std::endl;
      ++nerr;
    }
  }

  std::cerr << "\tSummary: predefined name to hid_t map has "
	    << nerr;

  if (nerr == 1)
  {
    std::cerr << " error.\n";
  }
  else
  {
    std::cerr << " errors.\n";
  }

  if (nerr == 0)
  {
    result = true;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

bool
test_compound()
{
  // Make an HDF5 compund datatype with 3 members

  hid_t members[3] = {H5T_NATIVE_INT, H5T_NATIVE_DOUBLE, H5T_NATIVE_CHAR};

  hid_t cmpd1 = datatype::create(3, members);
  assert(cmpd1 >= 0);

  // Make the same datatype another way.

  char* names[3] = {"H5T_NATIVE_INT", "H5T_NATIVE_DOUBLE", "H5T_NATIVE_CHAR"};

  hid_t cmpd2 = datatype::create(3, names);
  assert(cmpd2 >= 0);

  // Do we get the same result as with the 1st method?

  assert(H5Tequal(cmpd1, cmpd2) > 0);

  // And make the same datatype yet a 3rd way.

  char concat[] = "H5T_NATIVE_INT,H5T_NATIVE_DOUBLE,H5T_NATIVE_CHAR";

  hid_t cmpd3 = datatype::create(concat);
  assert(cmpd3 >= 0);

  // Better be the same as the first 2 methods.

  assert(H5Tequal(cmpd2, cmpd3) > 0);

  // Now duplicate the datatype by making a "whole" subset of it.

  char* mbr_names[3] = {"0", "1", "2"};

  hid_t cmpd4 = datatype::create(cmpd1, 3, mbr_names);
  assert(cmpd4 >= 0);

  // Better be the same as cmpd1.

  assert(H5Tequal(cmpd4, cmpd1) > 0);

  // Another duplication of the datatype by making a "whole" subset of it.

  unsigned mbr_indices[3] = {0, 1, 2};

  hid_t cmpd5 = datatype::create(cmpd1, 3, mbr_indices);
  assert(cmpd5 >= 0);

  // Better be the same as cmpd1.

  assert(H5Tequal(cmpd5, cmpd1) > 0);

  // A subset of cmpd1.

  char subset_name[] = "0,2";

  hid_t cmpd6 = datatype::create(cmpd1, subset_name);
  assert(cmpd6 >= 0);
  assert(H5Tget_nmembers(cmpd6) == 2);

  // Now let's see if we can attach a datatype by all the techniques available.
  // And we'll check to make sure that we duplicate cmpd1 with each technique.

  datatype dt;

  dt.attach(3, members);

  assert(H5Tequal(dt.hid(), cmpd1) > 0);

  dt.attach(3, names);

  assert(H5Tequal(dt.hid(), cmpd1) > 0);

  dt.attach(concat);

  assert(H5Tequal(dt.hid(), cmpd1) > 0);

  dt.attach(cmpd1, 3, mbr_names);

  assert(H5Tequal(dt.hid(), cmpd1) > 0);

  dt.attach(cmpd1, 3, mbr_indices);

  assert(H5Tequal(dt.hid(), cmpd1) > 0);

  dt.detach();

  // Finally, let's see if we can create new datatypes by each available technique,
  // checking to make sure that cmpd1 is duplicated each time.

  datatype dt1(3, members);

  assert(H5Tequal(dt1.hid(), cmpd1) > 0);

  datatype dt2(3, names);

  assert(H5Tequal(dt2.hid(), cmpd1) > 0);

  datatype dt3(concat);

  assert(H5Tequal(dt3.hid(), cmpd1) > 0);

  datatype dt4(cmpd1, 3, mbr_names);

  assert(H5Tequal(dt4.hid(), cmpd1) > 0);

  datatype dt5(cmpd1, 3, mbr_indices);

  assert(H5Tequal(dt5.hid(), cmpd1) > 0);

  H5Tclose(cmpd1);
  H5Tclose(cmpd2);
  H5Tclose(cmpd3);
  H5Tclose(cmpd4);
  H5Tclose(cmpd5);
  H5Tclose(cmpd6);

  return true; // just a placebo so compiler doesn't complain about not returning a value
}

int
main()
{
  int result;

  result = 0;

  if (!test_map())
  {
    ++result;
  }
  if (!test_compound())
  {
    ++result;
  }

  // Exit:

  return result;
}
