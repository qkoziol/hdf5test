#ifndef MEMORY_H
#define MEMORY_H

#include <cstdlib>
#include "extent.h"
#include "handle.h"
#include <ostream>
#include "pcontainer.h"

/*! @class memory
    @brief A non-persistent container whose members are stored in memory, not on disk.
*/

class memory : public container, public handle
{
  /// Writes memory characteristics to an ostream.

  friend std::ostream& operator<<(std::ostream& xos, const memory& xmem);

 public:


  // Standard features:


  /// Default constructor.

  memory();

  /// Create and attach to a scalar buffer of HDF5 type `xtype'.  Dataspace can
  /// be redefined later to be of a different extent or dimension.

  memory(hid_t xtype);

  /// Destructor.

  virtual ~memory();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  memory(const memory& xother);

  /// Assignment operator.

  memory& operator=(const memory & xother);

  /// Resize memory buffer to be adequate to
  /// contain contents of current selection
  /// of xcon.  Memory buffer is contiguous.

  void reserve(pcontainer& xcon);

  /// Capacity of memory in bytes.

  size_t ub() const;

  /// The memory itself;

  void* mem();
  const void* mem() const;


  // Handle interface:


  /// Is this handle attached to some real memory?

  bool is_attached() const;

  /// Attach to a buffer of specified hdf5 type.  Dataspace is scalar but can be redefined
  /// afterwards to be of a different extent or dimension.

  void attach(hid_t xtype);

  /// Disconnect from and recycle underlying memory.

  void detach();


  // Access interface:


  /// True if this can be read.

  bool is_readable() const;


 protected:


  char*   _mem;   ///< The memory buffer.
  size_t  _ub;    ///< An upper bound on the number of bytes that can be stored in _mem.
};

#endif
