#include "datatype.h"
#include "contract.h"
#include <cstdio>
#include <iostream>
#include <string.h>

datatype::
datatype()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:

  assert(!is_attached());
  assert(invariant());
}

datatype::
~datatype()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
datatype::
invariant() const
{
  bool result;

  // Preconditions:

  result = persistent::invariant();

  // Body:

  // Exit:

  return result;
}

datatype::
datatype(const datatype& xother)
{
  not_implemented;
}

datatype&
datatype::
operator=(const datatype& xother)
{
  not_implemented;
  return *this;  // keeps pgi compiler happy until this is implemented.
}

bool
datatype::
open(hid_t xhost, const std::string& xname)
{
  bool result;

  // Preconditions:

  assert(H5Iget_type(xhost) == H5I_FILE || H5Iget_type(xhost) == H5I_GROUP);

  // Body:

  // Attempt to open the datatype.

  H5E_BEGIN_TRY
  {
    _hid = H5Topen(xhost, xname.c_str());
  }
  H5E_END_TRY;

  if (_hid >= 0)
  {
    result = true;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  assert(result == is_attached());
  assert(invariant());

  // Exit:

  return result;
}

bool
datatype::
attach(int xct, hid_t* xmembers)
{
  bool result;

  // Preconditions:

  assert(xct > 0 ? xmembers != 0 : true);

  // Body:

  if (is_attached())
  {
    detach();
  }

  hid_t id = create(xct, xmembers);

  if (id >= 0)
  {
    hdf5_handle::attach(id);

    H5Idec_ref(id);

    result = true;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  assert(invariant());
  assert(result == is_attached());

  // Exit:

  return result;
}

bool
datatype::
attach(int xct, char** xmembers)
{
  bool result;

  // Preconditions:

  assert(xct > 0 ? xmembers != 0 : true);

  // Body:

  if (is_attached())
  {
    detach();
  }

  hid_t id = create(xct, xmembers);

  if (id >= 0)
  {
    hdf5_handle::attach(id);

    H5Idec_ref(id);

    result = true;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  assert(invariant());
  assert(result == is_attached());

  // Exit:

  return result;
}

hid_t
datatype::
predefined(const char* xname)
{
  hid_t result;

  // Preconditions:

  // Body:

  if (xname == 0)
  {
    result = H5I_INVALID_HID;
  }
  else
  {
    // ISSUE:
    // This is admittedly not very efficient.  One could use a yacc/lex
    // parser to make this elegant.  But it's efficiency is good enough for
    // now and it has the virtues of being obviously correct and simple
    // to modify or extend.

    if (strncmp("H5T_IEEE_F32BE", xname, 14) == 0)
    {
      result = H5T_IEEE_F32BE;
    }
    else if (strncmp("H5T_IEEE_F32LE", xname, 14) == 0)
    {
      result = H5T_IEEE_F32LE;
    }
    else if (strncmp("H5T_IEEE_F64BE", xname, 14) == 0)
    {
      result = H5T_IEEE_F64BE;
    }
    else if (strncmp("H5T_IEEE_F64LE", xname, 14) == 0)
    {
      result = H5T_IEEE_F64LE;
    }
    else if (strncmp("H5T_STD_I8BE", xname, 12) == 0)
    {
      result = H5T_STD_I8BE;
    }
    else if (strncmp("H5T_STD_I8LE", xname, 12) == 0)
    {
      result = H5T_STD_I8LE;
    }
    else if (strncmp("H5T_STD_I16BE", xname, 13) == 0)
    {
      result = H5T_STD_I16BE;
    }
    else if (strncmp("H5T_STD_I16LE", xname, 13) == 0)
    {
      result = H5T_STD_I16LE;
    }
    else if (strncmp("H5T_STD_I32BE", xname, 13) == 0)
    {
      result = H5T_STD_I32BE;
    }
    else if (strncmp("H5T_STD_I32LE", xname, 13) == 0)
    {
      result = H5T_STD_I32LE;
    }
    else if (strncmp("H5T_STD_I64BE", xname, 13) == 0)
    {
      result = H5T_STD_I64BE;
    }
    else if (strncmp("H5T_STD_I64LE", xname, 13) == 0)
    {
      result = H5T_STD_I64LE;
    }
    else if (strncmp("H5T_STD_U8BE", xname, 12) == 0)
    {
      result = H5T_STD_U8BE;
    }
    else if (strncmp("H5T_STD_U8LE", xname, 12) == 0)
    {
      result = H5T_STD_U8LE;
    }
    else if (strncmp("H5T_STD_U16BE", xname, 13) == 0)
    {
      result = H5T_STD_U16BE;
    }
    else if (strncmp("H5T_STD_U16LE", xname, 13) == 0)
    {
      result = H5T_STD_U16LE;
    }
    else if (strncmp("H5T_STD_U32BE", xname, 13) == 0)
    {
      result = H5T_STD_U32BE;
    }
    else if (strncmp("H5T_STD_U32LE", xname, 13) == 0)
    {
      result = H5T_STD_U32LE;
    }
    else if (strncmp("H5T_STD_U64BE", xname, 13) == 0)
    {
      result = H5T_STD_U64BE;
    }
    else if (strncmp("H5T_STD_U64LE", xname, 13) == 0)
    {
      result = H5T_STD_U64LE;
    }
    else if (strncmp("H5T_STD_B8BE", xname, 12) == 0)
    {
      result = H5T_STD_B8BE;
    }
    else if (strncmp("H5T_STD_B8LE", xname, 12) == 0)
    {
      result = H5T_STD_B8LE;
    }
    else if (strncmp("H5T_STD_B16BE", xname, 13) == 0)
    {
      result = H5T_STD_B16BE;
    }
    else if (strncmp("H5T_STD_B16LE", xname, 13) == 0)
    {
      result = H5T_STD_B16LE;
    }
    else if (strncmp("H5T_STD_B32BE", xname, 13) == 0)
    {
      result = H5T_STD_B32BE;
    }
    else if (strncmp("H5T_STD_B32LE", xname, 13) == 0)
    {
      result = H5T_STD_B32LE;
    }
    else if (strncmp("H5T_STD_B64BE", xname, 13) == 0)
    {
      result = H5T_STD_B64BE;
    }
    else if (strncmp("H5T_STD_B64LE", xname, 13) == 0)
    {
      result = H5T_STD_B64LE;
    }
    else if (strncmp("H5T_UNIX_D32BE", xname, 14) == 0)
    {
      result = H5T_UNIX_D32BE;
    }
    else if (strncmp("H5T_UNIX_D32LE", xname, 14) == 0)
    {
      result = H5T_UNIX_D32LE;
    }
    else if (strncmp("H5T_UNIX_D64BE", xname, 14) == 0)
    {
      result = H5T_UNIX_D64BE;
    }
    else if (strncmp("H5T_UNIX_D64LE", xname, 14) == 0)
    {
      result = H5T_UNIX_D64LE;
    }
    else if (strncmp("H5T_INTEL_I8", xname, 12) == 0)
    {
      result = H5T_INTEL_I8;
    }
    else if (strncmp("H5T_INTEL_I16", xname, 13) == 0)
    {
      result = H5T_INTEL_I16;
    }
    else if (strncmp("H5T_INTEL_I32", xname, 13) == 0)
    {
      result = H5T_INTEL_I32;
    }
    else if (strncmp("H5T_INTEL_I64", xname, 13) == 0)
    {
      result = H5T_INTEL_I64;
    }
    else if (strncmp("H5T_INTEL_U8", xname, 12) == 0)
    {
      result = H5T_INTEL_U8;
    }
    else if (strncmp("H5T_INTEL_U16", xname, 13) == 0)
    {
      result = H5T_INTEL_U16;
    }
    else if (strncmp("H5T_INTEL_U32", xname, 13) == 0)
    {
      result = H5T_INTEL_U32;
    }
    else if (strncmp("H5T_INTEL_U64", xname, 13) == 0)
    {
      result = H5T_INTEL_U64;
    }
    else if (strncmp("H5T_INTEL_B8", xname, 12) == 0)
    {
      result = H5T_INTEL_B8;
    }
    else if (strncmp("H5T_INTEL_B16", xname, 13) == 0)
    {
      result = H5T_INTEL_B16;
    }
    else if (strncmp("H5T_INTEL_B32", xname, 13) == 0)
    {
      result = H5T_INTEL_B32;
    }
    else if (strncmp("H5T_INTEL_B64", xname, 13) == 0)
    {
      result = H5T_INTEL_B64;
    }
    else if (strncmp("H5T_INTEL_F32", xname, 13) == 0)
    {
      result = H5T_INTEL_F32;
    }
    else if (strncmp("H5T_INTEL_F64", xname, 13) == 0)
    {
      result = H5T_INTEL_F64;
    }
    else if (strncmp("H5T_ALPHA_I8", xname, 12) == 0)
    {
      result = H5T_ALPHA_I8;
    }
    else if (strncmp("H5T_ALPHA_I16", xname, 13) == 0)
    {
      result = H5T_ALPHA_I16;
    }
    else if (strncmp("H5T_ALPHA_I32", xname, 13) == 0)
    {
      result = H5T_ALPHA_I32;
    }
    else if (strncmp("H5T_ALPHA_I64", xname, 13) == 0)
    {
      result = H5T_ALPHA_I64;
    }
    else if (strncmp("H5T_ALPHA_U8", xname, 12) == 0)
    {
      result = H5T_ALPHA_U8;
    }
    else if (strncmp("H5T_ALPHA_U16", xname, 13) == 0)
    {
      result = H5T_ALPHA_U16;
    }
    else if (strncmp("H5T_ALPHA_U32", xname, 13) == 0)
    {
      result = H5T_ALPHA_U32;
    }
    else if (strncmp("H5T_ALPHA_U64", xname, 13) == 0)
    {
      result = H5T_ALPHA_U64;
    }
    else if (strncmp("H5T_ALPHA_B8", xname, 12) == 0)
    {
      result = H5T_ALPHA_B8;
    }
    else if (strncmp("H5T_ALPHA_B16", xname, 13) == 0)
    {
      result = H5T_ALPHA_B16;
    }
    else if (strncmp("H5T_ALPHA_B32", xname, 13) == 0)
    {
      result = H5T_ALPHA_B32;
    }
    else if (strncmp("H5T_ALPHA_B64", xname, 13) == 0)
    {
      result = H5T_ALPHA_B64;
    }
    else if (strncmp("H5T_ALPHA_F32", xname, 13) == 0)
    {
      result = H5T_ALPHA_F32;
    }
    else if (strncmp("H5T_ALPHA_F64", xname, 13) == 0)
    {
      result = H5T_ALPHA_F64;
    }
    else if (strncmp("H5T_NATIVE_CHAR", xname, 15) == 0)
    {
      result = H5T_NATIVE_CHAR;
    }
    else if (strncmp("H5T_NATIVE_SCHAR", xname, 16) == 0)
    {
      result = H5T_NATIVE_SCHAR;
    }
    else if (strncmp("H5T_NATIVE_UCHAR", xname, 16) == 0)
    {
      result = H5T_NATIVE_UCHAR;
    }
    else if (strncmp("H5T_NATIVE_SHORT", xname, 16) == 0)
    {
      result = H5T_NATIVE_SHORT;
    }
    else if (strncmp("H5T_NATIVE_USHORT", xname, 17) == 0)
    {
      result = H5T_NATIVE_USHORT;
    }
    else if (strncmp("H5T_NATIVE_LONG", xname, 15) == 0)
    {
      result = H5T_NATIVE_LONG;
    }
    else if (strncmp("H5T_NATIVE_ULONG", xname, 16) == 0)
    {
      result = H5T_NATIVE_ULONG;
    }
    else if (strncmp("H5T_NATIVE_LLONG", xname, 16) == 0)
    {
      result = H5T_NATIVE_LLONG;
    }
    else if (strncmp("H5T_NATIVE_ULLONG", xname, 17) == 0)
    {
      result = H5T_NATIVE_ULLONG;
    }
    else if (strncmp("H5T_NATIVE_FLOAT", xname, 16) == 0)
    {
      result = H5T_NATIVE_FLOAT;
    }
    else if (strncmp("H5T_NATIVE_DOUBLE", xname, 17) == 0)
    {
      result = H5T_NATIVE_DOUBLE;
    }
    else if (strncmp("H5T_NATIVE_LDOUBLE", xname, 18) == 0)
    {
      result = H5T_NATIVE_LDOUBLE;
    }
    else if (strncmp("H5T_NATIVE_B8", xname, 13) == 0)
    {
      result = H5T_NATIVE_B8;
    }
    else if (strncmp("H5T_NATIVE_B16", xname, 14) == 0)
    {
      result = H5T_NATIVE_B16;
    }
    else if (strncmp("H5T_NATIVE_B32", xname, 14) == 0)
    {
      result = H5T_NATIVE_B32;
    }
    else if (strncmp("H5T_NATIVE_B64", xname, 14) == 0)
    {
      result = H5T_NATIVE_B64;
    }
    else if (strncmp("H5T_NATIVE_OPAQUE", xname, 17) == 0)
    {
      result = H5T_NATIVE_OPAQUE;
    }
    else if (strncmp("H5T_NATIVE_HADDR", xname, 16) == 0)
    {
      result = H5T_NATIVE_HADDR;
    }
    else if (strncmp("H5T_NATIVE_HSIZE", xname, 16) == 0)
    {
      result = H5T_NATIVE_HSIZE;
    }
    else if (strncmp("H5T_NATIVE_HSSIZE", xname, 17) == 0)
    {
      result = H5T_NATIVE_HSSIZE;
    }
    else if (strncmp("H5T_NATIVE_HERR", xname, 15) == 0)
    {
      result = H5T_NATIVE_HERR;
    }
    else if (strncmp("H5T_NATIVE_HBOOL", xname, 16) == 0)
    {
      result = H5T_NATIVE_HBOOL;
    }
    else if (strncmp("H5T_NATIVE_INT8", xname, 15) == 0)
    {
      result = H5T_NATIVE_INT8;
    }
    else if (strncmp("H5T_NATIVE_UINT8", xname, 16) == 0)
    {
      result = H5T_NATIVE_UINT8;
    }
    else if (strncmp("H5T_NATIVE_INT_LEAST8", xname, 21) == 0)
    {
      result = H5T_NATIVE_INT_LEAST8;
    }
    else if (strncmp("H5T_NATIVE_UINT_LEAST8", xname, 22) == 0)
    {
      result = H5T_NATIVE_UINT_LEAST8;
    }
    else if (strncmp("H5T_NATIVE_INT_FAST8", xname, 20) == 0)
    {
      result = H5T_NATIVE_INT_FAST8;
    }
    else if (strncmp("H5T_NATIVE_UINT_FAST8", xname, 21) == 0)
    {
      result = H5T_NATIVE_UINT_FAST8;
    }
    else if (strncmp("H5T_NATIVE_INT16", xname, 16) == 0)
    {
      result = H5T_NATIVE_INT16;
    }
    else if (strncmp("H5T_NATIVE_UINT16", xname, 17) == 0)
    {
      result = H5T_NATIVE_UINT16;
    }
    else if (strncmp("H5T_NATIVE_INT_LEAST16", xname, 22) == 0)
    {
      result = H5T_NATIVE_INT_LEAST16;
    }
    else if (strncmp("H5T_NATIVE_UINT_LEAST16", xname, 23) == 0)
    {
      result = H5T_NATIVE_UINT_LEAST16;
    }
    else if (strncmp("H5T_NATIVE_INT_FAST16", xname, 21) == 0)
    {
      result = H5T_NATIVE_INT_FAST16;
    }
    else if (strncmp("H5T_NATIVE_UINT_FAST16", xname, 22) == 0)
    {
      result = H5T_NATIVE_UINT_FAST16;
    }
    else if (strncmp("H5T_NATIVE_INT32", xname, 16) == 0)
    {
      result = H5T_NATIVE_INT32;
    }
    else if (strncmp("H5T_NATIVE_UINT32", xname, 17) == 0)
    {
      result = H5T_NATIVE_UINT32;
    }
    else if (strncmp("H5T_NATIVE_INT_LEAST32", xname, 22) == 0)
    {
      result = H5T_NATIVE_INT_LEAST32;
    }
    else if (strncmp("H5T_NATIVE_UINT_LEAST32", xname, 23) == 0)
    {
      result = H5T_NATIVE_UINT_LEAST32;
    }
    else if (strncmp("H5T_NATIVE_INT_FAST32", xname, 21) == 0)
    {
      result = H5T_NATIVE_INT_FAST32;
    }
    else if (strncmp("H5T_NATIVE_UINT_FAST32", xname, 22) == 0)
    {
      result = H5T_NATIVE_UINT_FAST32;
    }
    else if (strncmp("H5T_NATIVE_INT64", xname, 16) == 0)
    {
      result = H5T_NATIVE_INT64;
    }
    else if (strncmp("H5T_NATIVE_UINT64", xname, 17) == 0)
    {
      result = H5T_NATIVE_UINT64;
    }
    else if (strncmp("H5T_NATIVE_INT_LEAST64", xname, 22) == 0)
    {
      result = H5T_NATIVE_INT_LEAST64;
    }
    else if (strncmp("H5T_NATIVE_UINT_LEAST64", xname, 23) == 0)
    {
      result = H5T_NATIVE_UINT_LEAST64;
    }
    else if (strncmp("H5T_NATIVE_INT_FAST64", xname, 21) == 0)
    {
      result = H5T_NATIVE_INT_FAST64;
    }
    else if (strncmp("H5T_NATIVE_UINT_FAST64", xname, 22) == 0)
    {
      result = H5T_NATIVE_UINT_FAST64;
    }

    // N.B.:  H5T_NATIVE_INT and H5T_NATIVE_UINT have to come after
    // all the other INT and UINT types because these string comparisons
    // return 0 for all those other types.

    else if (strncmp("H5T_NATIVE_INT", xname, 14) == 0)
    {
      result = H5T_NATIVE_INT;
    }
    else if (strncmp("H5T_NATIVE_UINT", xname, 15) == 0)
    {
      result = H5T_NATIVE_UINT;
    }
    else if (strncmp("H5T_STD_I8BE", xname, 12) == 0)
    {
      result = H5T_STD_I8BE;
    }
    else if (strncmp("H5T_STD_I8LE", xname, 12) == 0)
    {
      result = H5T_STD_I8LE;
    }
    else if (strncmp("H5T_STD_I16BE", xname, 13) == 0)
    {
      result = H5T_STD_I16BE;
    }
    else if (strncmp("H5T_STD_I16LE", xname, 13) == 0)
    {
      result = H5T_STD_I16LE;
    }
    else if (strncmp("H5T_STD_I32BE", xname, 13) == 0)
    {
      result = H5T_STD_I32BE;
    }
    else if (strncmp("H5T_STD_I32LE", xname, 13) == 0)
    {
      result = H5T_STD_I32LE;
    }
    else if (strncmp("H5T_STD_I64BE", xname, 13) == 0)
    {
      result = H5T_STD_I64BE;
    }
    else if (strncmp("H5T_STD_I64LE", xname, 13) == 0)
    {
      result = H5T_STD_I64LE;
    }
    else if (strncmp("H5T_STD_U8BE", xname, 12) == 0)
    {
      result = H5T_STD_U8BE;
    }
    else if (strncmp("H5T_STD_U8LE", xname, 12) == 0)
    {
      result = H5T_STD_U8LE;
    }
    else if (strncmp("H5T_STD_U16BE", xname, 13) == 0)
    {
      result = H5T_STD_U16BE;
    }
    else if (strncmp("H5T_STD_U16LE", xname, 13) == 0)
    {
      result = H5T_STD_U16LE;
    }
    else if (strncmp("H5T_STD_U32BE", xname, 13) == 0)
    {
      result = H5T_STD_U32BE;
    }
    else if (strncmp("H5T_STD_U32LE", xname, 13) == 0)
    {
      result = H5T_STD_U32LE;
    }
    else if (strncmp("H5T_STD_U64BE", xname, 13) == 0)
    {
      result = H5T_STD_U64BE;
    }
    else if (strncmp("H5T_STD_U64LE", xname, 13) == 0)
    {
      result = H5T_STD_U64LE;
    }
    else if (strncmp("H5T_IEEE_F32BE", xname, 14) == 0)
    {
      result = H5T_IEEE_F32BE;
    }
    else if (strncmp("H5T_IEEE_F32LE", xname, 14) == 0)
    {
      result = H5T_IEEE_F32LE;
    }
    else if (strncmp("H5T_IEEE_F64BE", xname, 14) == 0)
    {
      result = H5T_IEEE_F64BE;
    }
    else if (strncmp("H5T_IEEE_F64LE", xname, 14) == 0)
    {
      result = H5T_IEEE_F64LE;
    }
    else
    {
      result = H5I_INVALID_HID;
    }
  }

  // Postconditions:

  // Exit:

  return result;
}

datatype::
datatype(int xct, hid_t* xmembers)
{

  // Preconditions:

  assert(xct > 0 ? xmembers != 0 : true);

  // Body:

  if (xct <= 0)
  {
    _hid = H5I_INVALID_HID;
  }
  else
  {
    _hid = -1;
    attach(xct, xmembers);
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

datatype::
datatype(int xct, char** xmembers)
{

  // Preconditions:

  assert(xct > 0 ? xmembers != 0 : true);

  // Body:

  if (xct <= 0)
  {
    _hid = H5I_INVALID_HID;
  }
  else
  {
    _hid = -1;
    attach(xct, xmembers);
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

datatype::
datatype(char* xname_list)
{

  // Preconditions:

  // Body:

  if (xname_list != 0)
  {
    _hid = -1;
    attach(xname_list);
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

bool
datatype::
attach(char* xname_list)
{
  bool result;

  // Preconditions:

  assert(xname_list != 0);

  // Body:


  if (is_attached())
  {
    detach();
  }

  hid_t id = create(xname_list);

  if (id >= 0)
  {
    hdf5_handle::attach(id);

    H5Idec_ref(id);

    result = true;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  assert(invariant());
  assert(result == is_attached());

  // Exit:

  return result;
}

datatype::
datatype(hid_t xcompound, int xct, unsigned* xindex)
{
  // Preconditions:

  assert(xct > 0 ? xindex != 0 : true);
  assert(H5Iget_type(xcompound) == H5I_DATATYPE);

  // Body:

  attach(xcompound, xct, xindex);

  // Postconditions:

  assert(invariant());

  // Exit:
}

datatype::
datatype(hid_t xcompound, int xct, char** xmembers)
{
  // Preconditions:

  assert(H5Iget_type(xcompound) == H5I_DATATYPE);
  assert(xct > 0 ? xmembers != 0 : true);

  // Body:

  attach(xcompound, xct, xmembers);

  // Postconditions:

  assert(invariant());

  // Exit:
}

bool
datatype::
attach(hid_t xcompound, int xct, unsigned* xindex)
{
  bool result;

  // Preconditions:

  assert(H5Iget_type(xcompound) == H5I_DATATYPE);
  assert(xct > 0 ? xindex != 0 : true);

  // Body:

  if (is_attached())
  {
    detach();
  }

  hid_t id = create(xcompound, xct, xindex);

  if (id >= 0)
  {
    hdf5_handle::attach(id);

    H5Idec_ref(id);

    result = true;
  }
  else
  {
    result = false;
  }

  // Postconditions:

  assert(invariant());
  assert(result == is_attached());

  // Exit:

  return result;
}

bool
datatype::
attach(hid_t xcompound, int xct, char** xmembers)
{
  bool result;

  // Preconditions:

  assert(H5Iget_type(xcompound) == H5I_DATATYPE);
  assert(xct > 0 ? xmembers != 0 : true);

  // Body:

  if (is_attached())
  {
    detach();
  }

  hid_t id = create(xcompound, xct, xmembers);

  if (id >= 0)
  {
    hdf5_handle::attach(id);

    result = true;

    H5Idec_ref(id);
  }
  else
  {
    result = false;
  }

  // Postconditions:

  assert(invariant());
  assert(result == is_attached());

  // Exit:

  return result;
}

hid_t
datatype::
create(int xct, hid_t* xmembers)
{
  hid_t result;

  // Preconditions:

  assert(xct > 0 ? xmembers != 0 : true);

  // Body:

  result = H5I_INVALID_HID; // for now; if everything works out it will become valid later

  if (xct <= 0)
  {
    // ... then there's nothing to do.

  }
  else if (xct == 1)
  {
    // ... only one member in list, no need to make compound type.

    result = H5Tcopy(*xmembers);
  }
  else
  {
    // ... more than one member in list, attempt to make compound type.

    bool status = true;

    // Determine the size of the compound datatype, assuming that each member
    // follows its predecessor in storage w/o any padding.  We need to know the
    // total length before we can create an HDF5 compound datatype.

    size_t nbytes = 0;

    for (int i = 0; status && (i < xct); ++i)
    {
      if (H5Iget_type(xmembers[i]) != H5I_DATATYPE)
      {
	status = false;
      }
      else
      {
	nbytes += H5Tget_size(xmembers[i]);
      }
    }
    if (status == true)
    {
      // Then all of xmembers are valid and it should be possible to
      // create a compound datatype.

      result = H5Tcreate(H5T_COMPOUND, nbytes);

      // Add the members one by one.  Names of the members will be
      // the names of their indices.

      size_t offset = 0;
      char   name[32];  // 32 decimal digits ought to be enough for names that are the
                        // string equivalents of the member index.

      for (int i = 0; (result >= 0) && (i < xct); ++i)
      {
        sprintf(name, "%d", i);

	if (H5Tinsert(result, name, offset, xmembers[i]) < 0)
	{
	  H5Tclose(result);
	  result = H5I_INVALID_HID;
	}

	offset += H5Tget_size(xmembers[i]);
      }
    }
  }

  // Postconditions:

  assert(result >= 0 ? H5Iget_type(result) == H5I_DATATYPE : true);

  // Exit:

  return result;
}

hid_t
datatype::
create(int xct, char** xmembers)
{
  hid_t result;

  // Preconditions:

  assert(xct > 0 ? xmembers != 0 : true);

  // Body:

  result = H5I_INVALID_HID; // for now; if everything works out it will become valid later

  if (xct <= 0)
  {
    result = H5I_INVALID_HID;
  }
  else
  {
    bool status = true; // until proven otherwise.

    // See discussion of predefined datatypes in http://hdf.ncsa.uiuc.edu/HDF5/doc/UG/,
    // section 1.2.2.  The tables there appear to be somewhat out of sync with the
    // predefined type list in .../html/PredefDTypes.html, so this implementation
    // takes a more-or-less common denominator of the two.

    hid_t* id = new hid_t[xct];

    for (int i = 0; status && i < xct; ++i)
    {
      id[i] = predefined(xmembers[i]);

      if (id[i] < 0)
      {
	status = false;
      }
    }

    if (status)
    {
      result = create(xct, id);
    }

    delete [] id;
  }

  // Postconditions:

  assert(result >= 0 ? H5Iget_type(result) == H5I_DATATYPE : true);

  // Exit:

  return result;
}

hid_t
datatype::
create(hid_t xcompound, int xct, unsigned* xindex)
{
  hid_t result;

  // Preconditions:

  assert(H5Iget_type(xcompound) == H5I_DATATYPE);
  assert(xct > 0 ? xindex != 0 : true);

  // Body:

  result = H5I_INVALID_HID; // for now; if everything works out it will become valid later

  // ISSUE:
  // This implementation makes a subset by ignoring any indices
  // in xindex that aren't valid indices.  Is that what we want
  // to do, or do we want to insist that *all* members of xindex
  // be valid?

  // If xindex contains repeats, it will generate an
  // insertion error and the result will be a detached datatype.

  // Figure out how big the subset is going to be.  This computation
  // assumes that members have no padding for alignment reasons.

  size_t nbytes   = 0;
  int    nmembers = H5Tget_nmembers(xcompound);
  int    niters   = (xct > nmembers ? nmembers : xct);

  for (int i = 0; i < niters; ++i)
  {
    hid_t member = H5Tget_member_type(xcompound, xindex[i]);

    if (member >= 0)
    {
      nbytes += H5Tget_size(member);

      H5Tclose(member);
    }
  }

  if (nbytes > 0)
  {
    result = H5Tcreate(H5T_COMPOUND, nbytes);

    size_t offset = 0;

    bool status = true; // i.e., good - so far.

    for (int i = 0; i < niters && status; ++i)
    {
      hid_t member = H5Tget_member_type(xcompound, xindex[i]);

      if (member >= 0)
      {
	// This *has* to succeed; every member has a name, and we know that
	// the member exists.

	char* name = H5Tget_member_name(xcompound, xindex[i]);

	if (H5Tinsert(result, name, offset, member) < 0)
	{
	  H5Tclose(result);
	  result = H5I_INVALID_HID;
	  status = false;
	}

	offset += H5Tget_size(member);

	free(name);
	H5Tclose(member);
      }
    }
  }

  // Postconditions:

  assert(result >= 0 ? H5Iget_type(result) == H5I_DATATYPE : true);

  // Exit:

  return result;
}

hid_t
datatype::
create(hid_t xcompound, int xct, char** xmembers)
{
  hid_t result;

  // Preconditions:

  assert(H5Iget_type(xcompound) == H5I_DATATYPE);
  assert(xct > 0 ? xmembers != 0 : true);

  // Body:


  // Figure out how members the subset will have.

  int nmembers = H5Tget_nmembers(xcompound);
  int niters   = (xct > nmembers ? nmembers : xct);
  int nsubset  = 0;

  for (int i = 0; i < niters; ++i)
  {
    int index = H5Tget_member_index(xcompound, xmembers[i]);

    if (index >= 0)
    {
      ++nsubset;
    }
  }

  if (nsubset > 0)
  {
    // Maybe the character strings in xmembers consist of nothing
    // but digits.  In that case we interpret them as indices of
    // a compound type.  Deal with this case here.

    bool chars_are_digits = true;

    // Look at all the characters; as soon as we find a non-digit
    // we know that we can't interpret xmembers as indices.

    for (int i = 0; (i < niters) && chars_are_digits; ++i)
    {
      size_t nchars = strlen(xmembers[i]);

      for (size_t j = 0; (j < nchars) && chars_are_digits; ++j)
      {
	if (! isdigit(xmembers[i][j]))
	{
	  chars_are_digits = false;
	}
      }
    }

    if (chars_are_digits)
    {
      // xmembers contains indices of compound type members

      unsigned* indices = new unsigned[nsubset];
      int       n       = 0;

      for (int i = 0; i < niters; ++i)
      {
	int index = atoi(xmembers[i]);

	if (index >= 0 && index < nmembers)
	{
	  indices[n] = index;
	  ++n;
	}
      }

      result = create(xcompound, n, indices);

      delete [] indices;
    }
    else
    {
      // xmembers contains names of predefined types

      unsigned* indices = new unsigned[nsubset];
      int       n       = 0;

      for (int i = 0; i < niters; ++i)
      {
	int index = H5Tget_member_index(xcompound, xmembers[i]);

	if (index >= 0)
	{
	  indices[n] = index;
	  ++n;
	}
      }

      result = create(xcompound, n, indices);

      delete [] indices;
    }
  }
  else
  {
    result = H5I_INVALID_HID;
  }

  // Postconditions:

  assert(result >= 0 ? H5Iget_type(result) == H5I_DATATYPE : true);

  // Exit:

  return result;
}

hid_t
datatype::
create(char* xname_list)
{
  hid_t result;

  // Preconditions:

  // Body:

  if (xname_list == 0)
  {
    result = H5I_INVALID_HID;
  }
  else
  {
    size_t larg = strlen(xname_list);

    // Substitute '\0' for ',' in xname_list so it looks like
    // a sequence of standard C strings.

    size_t name_ct = 1;  // number of type names in xarg or arg

    for (size_t i = 0; xname_list[i] != '\0'; ++i)
    {
	if (xname_list[i] == ',')
      {
	xname_list[i] = '\0';
	++name_ct;
      }
    }

    // Make an "argv" array of pointers to the first characters
    // of each name in the "xname_list" array.

    char** argv = new char*[name_ct];

    {
      size_t i, j, k;

      for (i = j = k = 0; i <= larg; ++i)
      {
	if (xname_list[i] == '\0')
	{
	  argv[j] = xname_list+k;
	  ++j;
	  k = i+1;
	}
      }
    }

    result = create(name_ct, argv);

    delete [] argv;

    // Restore xname_list by making all '\0's commas except the last
    // one.

    for (size_t i = 0; i < larg; ++i)
    {
      if (xname_list[i] == '\0')
      {
	xname_list[i] = ',';
      }
    }
  }

  // Postconditions:

  assert(result >= 0 ? H5Iget_type(result) == H5I_DATATYPE : true);

  // Exit:

  return result;
}

hid_t
datatype::
create(hid_t xcompound, char* xname_list)
{
  hid_t result;

  // Preconditions:

  assert(H5Iget_type(xcompound) == H5I_DATATYPE);

  // Body:

  if (xname_list == 0)
  {
    result = H5I_INVALID_HID;
  }
  else
  {
    size_t larg = strlen(xname_list);

    // Substitute '\0' for ',' in xname_list so it looks like
    // a sequence of standard C strings.

    size_t name_ct = 1;  // number of type names in xarg or arg

    for (size_t i = 0; xname_list[i] != '\0'; ++i)
    {
	if (xname_list[i] == ',')
      {
	xname_list[i] = '\0';
	++name_ct;
      }
    }

    // Make an "argv" array of pointers to the first characters
    // of each name in the "xname_list" array.

    char** argv = new char*[name_ct];

    {
      size_t i, j, k;

      for (i = j = k = 0; i <= larg; ++i)
      {
	if (xname_list[i] == '\0')
	{
	  argv[j] = xname_list+k;
	  ++j;
	  k = i+1;
	}
      }
    }

    result = create(xcompound, name_ct, argv);

    delete [] argv;

    // Restore xname_list by making all '\0's commas except the last
    // one.

    for (size_t i = 0; i < larg; ++i)
    {
      if (xname_list[i] == '\0')
      {
	xname_list[i] = ',';
      }
    }
  }

  // Postconditions:

  assert(result >= 0 ? H5Iget_type(result) == H5I_DATATYPE : true);

  // Exit:

  return result;
}

