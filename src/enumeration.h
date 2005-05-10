#ifndef ENUMERATION_H
#define ENUMERATION_H

#include "datatype.h"

/*! @class enumeration
    @brief A handle to an HDF5 enumeration datatype.
*/

class enumeration : public datatype
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  enumeration();

  /// Destructor.

  ~enumeration();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  enumeration(const enumeration& xother);

  /// Assignment operator.

  enumeration& operator=(const enumeration& xother);


  // Persistence interface:


  /// Open an existing enumeration in file or group xhost.

  bool open(hid_t xhost, const string& xname);
};

#endif
