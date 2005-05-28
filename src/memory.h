#ifndef MEMORY_H
#define MEMORY_H

#include "handle.h"
#include "pcontainer.h"
#include "std_cstdlib.h"

class memory : public container, public handle
{
 public:


  // Standard features:


  // Default constructor.

  memory();

  // Destructor.

  virtual ~memory();

  // Class invariant.

  bool invariant() const;

  // Copy constructor.

  memory(const memory& xother);

  // Assignment.

  memory& operator=(const memory & xother);

  // Resize memory buffer to be adequate to
  // contain contents of current selection
  // of xcon.

  void reserve(const pcontainer& xcon);

  // Capacity of memory in bytes.

  size_t ub() const;

  // The memory itself;

  void* mem();
  const void* mem() const;


  // Handle interface:


  /// Is this handle attached to some real memory?

  bool is_attached() const;

  /// Disconnect from and recycle underlying memory.

  void detach();


  // Container interface:


  // Get the dataspace associated with memory.

  dataspace& get_space() const;

  // Get the type stored in memory.

  hid_t get_type() const;


  // Access interface:


  /// True if this holder can be read.

  bool is_readable() const;

 private:

  char*   _mem;   // The memory buffer.
  size_t  _ub;    // An upper bound on the number of bytes that can be stored in _mem.
  hid_t   _type;  // The HDF5 type stored in this container.
  hid_t   _space; // Associated dataspace.
};

#endif
