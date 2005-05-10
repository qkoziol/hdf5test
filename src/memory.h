#ifndef MEMORY_H
#define MEMORY_H

class container;
class dataspace;
#include "hdf5.h"
#include "std_cstdlib.h"

class memory
{
 public:


  // Standard features:


  // Default constructor.

  memory();

  // Destructor.

  ~memory();

  // Class invariant.

  bool invariant() const;

  // Copy constructor.

  memory(const memory& xother);

  // Assignment.

  memory& operator=(const memory & xother);

  // Resize memory buffer to be adequate to
  // contain contents of current selection
  // of xcon.

  void reserve(const container& xcon);

  // Capacity of memory in bytes.

  size_t ub() const;

  // The memory itself;

  void* mem();
  const void* mem() const;


  // Container interface:


  // Get the dataspace associated with memory.

  dataspace& get_space() const;

  // Get the type stored in memory.

  hid_t get_type() const;

 private:

  char*   _mem;   // The memory buffer.
  size_t  _ub;    // An upper bound on the number of bytes that can be stored in _mem.
  hid_t   _type;  // The HDF5 type stored in this container.
  hid_t   _space; // Associated dataspace.
};

#endif
