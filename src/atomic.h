#ifndef ATOMIC_H
#define ATOMIC_H

#include "datatype.h"

/*! @class atomic
    @brief A handle to an HDF5 atomic datatype.
*/

class atomic : public datatype
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  atomic();

  /// Destructor.

  ~atomic();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  atomic(const atomic& xother);

  /// Assignment operator.

  atomic& operator=(const atomic& xother);


  // Persistence interface:


  /// Open an existing atomic in file or group xhost.

  bool open(hid_t xhost, const string& xname);

};

#endif
