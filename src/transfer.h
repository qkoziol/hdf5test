#ifndef TRANSFER_H
#define TRANSFER_H

class container;
#include "hdf5.h"
class memory;

/*
  One can view HDF5 as a library for persistent storage and
  retrieval of the array-like information common to scientific
  number-crunching codes.  One central notion, then, is the
  transfer of information between persistent storage and
  memory.  Transfer can occur in two directions: from memory
  into persistent storage, and from persistent storage into
  memory - a symmetrical situation with memory and persistent
  storage being peers.

  However, the HDF5 library is implemented asymmetrically.
  Containers "do" i/o transfer operations and containers
  have elaborate interfaces.  But memory has no interface,
  is not an object, and doesn't "do" anything.  Its
  presence is felt as a void* argument supplied to the read
  and write operators.

  This class is an attempt at implementing symmetry between
  persistent and transient storage; between HDF5 containers
  and memory.
*/

/*
  ISSUE:

  How do we specify transfer properties?  Do we make "transfer"
  a class with those properties?  We could then inherit from
  a generic "transfer" to more specific ones with guaranteed
  built-in properties.  Or we could have various kinds of
  constructors that would guarantee properties.  Or we could
  have various commands on the class that forced it to
  have certain transfer properties.
*/

  // ISSUE:
  // Should we make the return of these functions a
  // double?  The interpretation would be the number
  // of bytes successfully transferred if >= 0 and
  // an error if < 0.

  // Attempt to transfer the current selection from memory to
  // container.  Return true if sucessful, false otherwise.

bool transfer(const memory& xfrom, container& xto, hid_t xprop_list);

  // Attempt to transfer the current selection from container
  // to memory.  Return true if sucessful, false otherwise.

bool transfer(const container& xfrom, memory& xto, hid_t xprop_list);


#endif
