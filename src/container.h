#ifndef CONTAINER_H
#define CONTAINER_H

/*! @class container
    @brief A container of HDF5 datatypes arranged in an HDF5 dataspace.
*/

#include "dataspace.h"
#include "persistent.h"

class container : public persistent
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  container();

  /// Destructor.

  ~container();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  container(const container& xother);

  /// Assignment operator.

  container& operator=(const container& xother);

  /// Get the dataspace associated with this container.
 
  virtual dataspace& get_space() const = 0;

  /// Get the type stored in this container.

  virtual hid_t get_type() const = 0;


  // Access interface:


  /// True if this container can be read.

  virtual bool is_readable() const = 0;

};

#endif
