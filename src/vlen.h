#ifndef VLEN_H
#define VLEN_H

#include "datatype.h"

/*! @class vlen
    @brief A handle to an HDF5 vlen datatype.
*/

class vlen : public datatype
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  vlen();

  /// Destructor.

  ~vlen();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  vlen(const vlen& xother);

  /// Assignment.

  vlen& operator=(const vlen& xother);


  // Persistence interface:


  /// Open an existing vlen in file or group xhost.

  bool open(hid_t xhost, const string& xname);
};

#endif
