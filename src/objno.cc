#include "objno.h"
#include "contract.h"

#include "array_of.h"

objno::
objno(const H5G_stat_t& xstat)
{
  id[0]   = xstat.objno[0];
  id[1]   = xstat.objno[1];
  idx     = 0;
  is_attr = false;
}

bool
objno::
operator<(const objno& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  // All this logic is just a field by field comparison,
  // arbitrarily assigning id[0] the highest precedence,
  // id[1] the next highest, and idx the lowest precedence.

  if (id[0] < xother.id[0])
  {
    result = true;
  }
  else if (id[0] == xother.id[0])
  {
    if (id[1] < xother.id[1])
    {
      result = true;
    }
    else if (id[1] == xother.id[1])
    {
      if (idx < xother.idx)
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

  id[0] = xother.id[0];
  id[1] = xother.id[1];
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

  id[0] = statbuf.objno[0];
  id[1] = statbuf.objno[1];

  if ( H5Iget_type(xhid) == H5I_ATTR)
  {
    not_implemented;
    is_attr = true;
  }
  else
  {
    is_attr = false;
    idx     = 0;
  }

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
      << xobj.id[0]
      << ", "
      << xobj.id[1]
      << ')';

  // Postconditions:

  // Exit:

  return xos;
}
