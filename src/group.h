#ifndef GROUP_H
#define GROUP_H

#include "persistent.h"
#include "std_string.h"

/*! @class group
    @brief A handle to an HDF5 group.
*/

class group : public persistent
{
 public:

  // Standard features:

  /// Constructor.  Creates a handle not attached to any underlying HDF5 object.

  group();

  /// Destructor.

  ~group();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  group(const group& xother);

  /// Assignment operator.

  group& operator=(const group& xother);


  // Persistence interface:


  /// Attempt to attach handle to existing hdf5 group.

  bool open(hid_t xhost, const string& xname = "/");

};

#endif
