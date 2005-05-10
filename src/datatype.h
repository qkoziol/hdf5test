#ifndef DATATYPE_H
#define DATATYPE_H

#include "persistent.h"

/*! @class datatype
    @brief A handle to an HDF5 datatype.
*/

class datatype : public persistent
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  datatype();

  /// Destructor.

  ~datatype();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  datatype(const datatype& xother);

  /// Assignment operator.

  datatype& operator=(const datatype& xother);


  // Persistence interface:


  /// Open an existing datatype in file or group xhost.

  bool open(hid_t xhost, const string& xname);
};

#endif

