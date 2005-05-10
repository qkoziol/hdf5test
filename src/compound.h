#ifndef COMPOUND_H
#define COMPOUND_H

#include "datatype.h"

/*! @class compound
    @brief A handle to an HDF5 compound datatype.
*/

class compound : public datatype
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  compound();

  /// Destructor.

  ~compound();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  compound(const compound& xother);

  /// Assignment operator.

  compound& operator=(const compound& xother);


  // Persistence interface:


  /// Open an existing compound in file or group xhost.

  bool open(hid_t xhost, const string& xname);
};

#endif
