#ifndef OBJNO_H
#define OB_NO_H

#include "hdf5.h"
#include "std_iostream.h"

/*! @class objno
    @brief HDF5 objects within the same file are uniquely identified by a pair
           of unsigned longs.  See H5Gget_objinfo() for details.  Traversing
           a directed graph requires having a unique identity for the objects
           encountered.  This will serve as the id.  This class is just a glorified
	   struct.
*/

class objno
{

  /// Write an objno to xos.

  friend ostream& operator<<(ostream& xos, const objno& xobj);

 public:

  unsigned long a;
  unsigned long b;

  /// Comparison operator required by STL's sorted associative container.

  bool operator<(const objno& xother) const;

  /// Assignment operator.

  objno& operator=(const objno& xother);

  /// Constructor. deprecated.

  objno(const H5G_stat_t& xstat);

  /// Constructor.

  objno(const hid_t xhid);

};

#endif
