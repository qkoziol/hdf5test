#ifndef TUPLE_H
#define TUPLE_H

#include "hdf5.h"

/*
  An n-tuple of hsize_ts, useful in describing
  extents and hyperslabs and selections and
  such things.
*/

class tuple
{
 public:


  // Canonical interface:


  // Default constructor; make a tuple of dimension 0.

  tuple();

  // Destructor.

  ~tuple();

  // Class invariant.

  bool invariant() const;

  // Copy constructor.

  tuple(const tuple& xother);

  // Assignment.

  tuple& operator=(const tuple& xother);


  // Dimension and member access interface:


  // Create a tuple of dimension xdim.

  tuple(int xdim);

  // The dimension of the tuple.

  int d() const;

  // Access to xindex element.

  hsize_t& operator[](int xindex);
  const hsize_t& operator[](int xindex) const;

  // Change dimension.

  void reserve(int xdim);

  // Assign xval to each element of this.

  tuple& operator=(const hsize_t& xval);


  // Comparison interface:


  // All elements are > 0.

  bool is_positive() const;

  // True if all elements of this > corresponding elements of xother.

  bool operator>(const tuple& xother) const;

  // True if all elements of this >= corresponding elements of xother.

  bool operator>=(const tuple& xother) const;

  // True if all elements of this == corresponding elements of xother.

  bool operator==(const tuple& xother) const;

  // True if all elements of this < corresponding elements of xother.

  bool operator<(const tuple& xother) const;

  // True if all elements of this <= corresponding elements of xother.

  bool operator<=(const tuple& xother) const;



 private:

  int      _d;   // dimension.

  hsize_t* _mem; // storage for elements.
};

#endif
