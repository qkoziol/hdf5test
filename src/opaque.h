#ifndef OPAQUE_H
#define OPAQUE_H

#include "datatype.h"

/*! @class opaque
    @brief A handle to an HDF5 opaque datatype.
*/

class opaque : public datatype
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  opaque();

  /// Destructor.

  ~opaque();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  opaque(const opaque& xother);

  /// Assignment operator.

  opaque& operator=(const opaque& xother);


  // Persistence interface:


  /// Open an existing opaque in file or group xhost.

  bool open(hid_t xhost, const string& xname);
};

#endif
