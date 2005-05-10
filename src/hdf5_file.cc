#include "hdf5_file.h"
#include "contract.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

hdf5_file::
hdf5_file()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());
  assert(! is_attached());

  // Exit:
}

hdf5_file::
~hdf5_file()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
hdf5_file::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = hdf5_handle::invariant();

  // Postconditions:

  // Exit:

  return result;
}

bool
hdf5_file::
open(const string& xname, access xacc)
{
  // Preconditions:

  assert(is_hdf5(xname));
  assert(xacc == RDWR ? is_writable(xname) : true);

  // Body;

  unsigned flag;

  if (xacc == RDONLY)
  {
    flag = H5F_ACC_RDONLY;
    _is_write_protected = true;
  }
  else
  {
    flag = H5F_ACC_RDWR;
    _is_write_protected = false;
  }

  // TODO:
  // Add property list interface instead of just
  // defaulting to H5P_DEFAULT.

  H5E_BEGIN_TRY
  {
    _hid = H5Fopen(xname.c_str(), flag, H5P_DEFAULT);
  }
  H5E_END_TRY;

  // Postconditions:

  assert(invariant());
  assert(is_attached());
  assert(xacc == RDONLY ? is_write_protected() : !is_write_protected());

  // Exit:
}

bool
hdf5_file::
create(const string& xname)
{
  bool result;

  // Preconditions:

  assert(can_create(xname));

  // Body;

  // TODO:
  // Add interface to allow specification of default args
  // supplied below.

  _hid = H5Fcreate(xname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  _is_write_protected = false;
  if (_hid >= 0)
    result = true;
  else
    result = false;

  // Postconditions:

  assert(invariant());
  assert(result == is_attached());
  assert(result == exists(xname));
  assert(result == is_writable(xname));
  assert(result == is_readable(xname));
  assert(! is_write_protected());

  // Exit:

  return result;
}

bool
hdf5_file::
exists(const string& xname)
{
  bool result;

  // Preconditions:

  // Body:

  struct stat statbuf;

  if (stat(xname.c_str(), &statbuf) == 0)
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
hdf5_file::
is_readable(const string& xname)
{
  bool result;

  // Preconditions:

  // Body:

  struct stat statbuf;

  if (stat(xname.c_str(), &statbuf) == 0)
  {
    // then xname exists

    if (S_IRUSR & statbuf.st_mode)
    {
      // and we, the effective user, have read access.

      result = true;
    }
    else
    {
      // we, the effective user, don't have read access.
      // Maybe our effective group does?

      if (S_IRGRP & statbuf.st_mode)
      {
	// our effective group has read access.

	result = true;
      }
      else
      {
	// our effective group doesn't have read access.

	result = false;
      }
    }
  }
  else
  {
    // xname doesn't exist.

    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

bool
hdf5_file::
is_writable(const string& xname)
{
  bool result;

  // Preconditions:

  // Body:

  struct stat statbuf;

  if (stat(xname.c_str(), &statbuf) == 0)
  {
    // then xname exists

    if (S_IWUSR & statbuf.st_mode)
    {
      // and we, the effective user, have write access.

      result = true;
    }
    else
    {
      // we, the effective user, don't have write access.
      // Maybe our effective group does?

      if (S_IWGRP & statbuf.st_mode)
      {
	// our effective group has write access.

	result = true;
      }
      else
      {
	// our effective group doesn't have write access.

	result = false;
      }
    }
  }
  else
  {
    // xname doesn't exist.

    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

bool
hdf5_file::
can_create(const string& xname)
{
  bool result;

  // Preconditions:

  // Body:

  // ERROR, TODO
  // Obviously we can't always create a file of
  // a given name.  Put the appropriate logic in here
  // to determine the directory associated with xname
  // and to check whether that directory has w permission.

  result = true;

  // Postconditions:

  // Exit:

  return result;
}

bool
hdf5_file::
is_hdf5(const string& xname)
{
  bool result;

  // Preconditions:

  // Body:

  if (H5Fis_hdf5(xname.c_str()) > 0)
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
hdf5_file::
is_write_protected() const
{
  bool result;

  // Preconditions:

  assert(is_attached());

  // Body:

  result = _is_write_protected;

  // Postconditions:

  // Exit:

  return result;
}
