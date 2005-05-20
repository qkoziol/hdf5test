#ifndef NODE_H
#define NODE_H

#include "hdf5.h"
#include "std_iostream.h"

/*! @class node
    @brief A node in the directed graph of HDF5 objects, with references
           to self, a group or other object to which self is linked (its
           parent), and the index of self in that parent.  This information
	   allows attributes as well as the "first class" HDF5 objects:
	   datasets, groups, and datatypes to get acquire full pathnames
	   from the HDF5 library.
 */

class node
{

  /// Write xnode to xos.

  friend ostream& operator<<(ostream& xos, const node& xnode);

 public:


  // Standard features:


  /// Default constructor.

  node();

  /// Equality operator.

  bool operator==(const node& xother) const;

  /// Inequality operator.

  bool operator!=(const node& xother) const;

  // Data:

  hid_t self;
  hid_t parent;
  int   index;
};

#endif
