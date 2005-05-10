#ifndef ARRAY_H
#define ARRAY_H

#include "datatype.h"

/*! @class array
    @brief A handle to an HDF5 array datatype.
*/

class array : public datatype
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  array();

  /// Destructor.

  ~array();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  array(const array& xother);

  /// Assignment operator.

  array& operator=(const array& xother);


  // Persistence interface:


  /// Open an existing array in file or group xhost.

  bool open(hid_t xhost, const string& xname);
};

#endif
