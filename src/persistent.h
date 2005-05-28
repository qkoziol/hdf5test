#ifndef PERSISTENT_H
#define PERSISTENT_H

#include "hdf5_handle.h"
#include "std_string.h"

/*! @class persistent
    @brief Objects with an open() operation.

    <p>
      Persistent objects can attempt to attach themselves
      to an existing representation stored in an HDF5 file.
    </p>
*/

class persistent : public hdf5_handle
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  persistent();

  /// Destructor.

  virtual ~persistent();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  persistent(const persistent& xother);

  /// Assignment operator.

  persistent& operator=(const persistent& xother);


  // Persistence interface:


  /// Attempt to open an existing object identified by xname associated
  /// with a host object.  Depending on circumstances, xhost may indicate
  /// an HDF5 file, group, dataset, or datatype.  The return value indicates
  /// success or failure.

  virtual bool open(hid_t xhost, const string& xname) = 0;

 protected:

};

#endif
