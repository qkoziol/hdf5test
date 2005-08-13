#ifndef OBJNO_H
#define OB_NO_H

#include "hdf5.h"
#include <ostream>

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

  friend std::ostream& operator<<(std::ostream& xos, const objno& xobj);

 public:

#if H5_VERS_MINOR==6
  unsigned long id[2];     ///< The object id within a given file; objno field in H5G_stat_t.
#else
  haddr_t id;     ///< The object id within a given file; objno field in H5G_stat_t.
#endif
  unsigned int  idx;       ///< Attribute index. 
  bool          is_attr;   ///< True if this identifies an attribute.

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
