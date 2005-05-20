#include "objno.h"
#include "contract.h"

#include "array_of.h"

objno::
objno(const H5G_stat_t& xstat)
{
  a = xstat.objno[0];
  b = xstat.objno[1];
}

bool
objno::
operator<(const objno& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  if (a < xother.a)
  {
    result = true;
  }
  else if (a == xother.a)
  {
    if (b < xother.b)
    {
      result = true;
    }
    else
    {
      result = false;
    }
  }
  else
  {
    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

objno&
objno::
operator=(const objno& xother)
{
  objno* ptr_to_result;

  // Preconditions:

  // Body:

  a = xother.a;
  b = xother.b;
  ptr_to_result = this;

  // Postconditions:

  // Exit:

  return *ptr_to_result;
}


objno::
objno(const hid_t xhid)
{
  // Preconditions:

  
  assert(H5Iget_type(xhid) == H5I_DATASET || H5Iget_type(xhid) == H5I_ATTR || H5Iget_type(xhid) == H5I_DATATYPE || H5Iget_type(xhid) == H5I_GROUP);

  // Body:

  hid_t file = H5Iget_file_id(xhid);

  array_of<char> name;

  ssize_t size = H5Iget_name(xhid, 0, 0);

  name.reserve(size+1);

  H5Iget_name(xhid, &name[0], size+1);

  H5G_stat_t statbuf;

  H5Gget_objinfo(file, &name[0], 0, &statbuf);

  H5Idec_ref(file);

  a = statbuf.objno[0];
  b = statbuf.objno[1];

  // Postconditions:

  // Exit:
}

ostream&
operator<<(ostream& xos, const objno& xobj)
{
  // Preconditions:

  assert(xos.good());

  // Body:

  xos << '('
      << xobj.a
      << ", "
      << xobj.b
      << ')';

  // Postconditions:

  // Exit:

  return xos;
}
