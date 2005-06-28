
#ifndef DATASPACE_H
#define DATASPACE_H

#include "extent.h"
#include "hdf5_handle.h"
class hyperslab;

/*! @class dataspace
    @brief A handle to a "simple" HDF5 dataspace.

    <p>
        The HDF5 library
	documentation alludes to 3 different kinds of dataspaces:
	scalar, simple, and complex.  In practice, scalar dataspaces
	are just 0 dimensional dataspaces, and complex dataspaces
	are an idea that has not been implemented.  This class implements
	simple dataspaces of dimension >= 0, thereby representing all
	dataspaces currently support by the HDF5 library.
    </p>
*/

class dataspace : public hdf5_handle
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  dataspace();

  /// Destructor.

  virtual ~dataspace();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  dataspace(const dataspace& xother);

  /// Copy constructor.

  dataspace(hid_t xother);

  /// Assignment operator.

  dataspace& operator=(const dataspace& xother);


  // hdf5_handle interface:


  /// Attach to xhid.  Stores xhid internally and increments the reference count
  /// to it, so client is still responsible for disposing of xhid.

  void attach(hid_t xhid);


  // Other constructors:


  /// Make a dataspace of given size and max size.

  dataspace(const tuple& xsize, const tuple& xmax_size);

  /// Select a subset of this dataspace.

  void select(const hyperslab& xsubset, H5S_seloper_t xop = H5S_SELECT_SET);

  /// Select all of this dataspace.

  void select_all();

  /// Select none of this dataspace.

  void select_none();

  /// Get the extent of this dataspace.

  const extent& get_extent();

  /// The dimension of the dataspace.

  unsigned d() const;


 protected:


  extent _ext;  ///< The extent of this dataspace.

  /// Define the extent.

  void set_extent();

};

#endif

