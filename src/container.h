#ifndef CONTAINER_H
#define CONTAINER_H

#include "dataspace.h"

/*! @class container
    @brief A container of HDF5 datatypes arranged in an HDF5 dataspace.
*/

class container
{
 public:

  // Standard features are omitted.  All implementations are trivial
  // since the class is abstract and has no representation.


  /// Get the dataspace associated with this container.
 
  virtual dataspace& get_space() const = 0;

  /// Get the type stored in this container.

  virtual hid_t get_type() const = 0;


  // Access interface:


  /// True if this container can be read.

  virtual bool is_readable() const = 0;

};

#endif
