#ifndef CONTAINER_H
#define CONTAINER_H

#include "dataspace.h"

/*! @class container
    @brief A container of HDF5 datatypes arranged in an HDF5 dataspace.
*/

class container
{
 public:


  // Standard features:


  /// Default constructor.

  container();

  /// Destructor.

  virtual ~container();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  container(const container& xother);

  /// Assignment operator.

  container& operator=(const container & xother);


  /// Get the dataspace associated with this container.
 
  dataspace& get_space();
  const dataspace& get_space() const;

  /// Get the type stored in this container.  Client should *not* call
  /// H5Idec_ref() or H5Tclose() on the return value.

  hid_t get_type() const;


  // Access interface:


  /// True if this container can be read.

  virtual bool is_readable() const = 0;

 protected:

  hid_t     _type;   ///< The HDF5 datatype associated with this container.
  dataspace _space;  ///< The dataspace of this container.

};

#endif
