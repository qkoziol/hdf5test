#ifndef TUPLE_H
#define TUPLE_H

#include "hdf5.h"
#include "std_iostream.h"

/*! @class tuple
    @brief An n-tuple of hsize_ts, useful for describing extents, hyperslabs, and selections.
*/

class tuple
{

  /// Append tuple to ostream.

  friend ostream& operator<<(ostream& xos, const tuple& xtuple);

 public:


  // Standard interface:


  /// Default constructor; make a tuple of dimension 0.

  tuple();

  /// Destructor.

  ~tuple();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  tuple(const tuple& xother);

  /// Assignment operator.

  tuple& operator=(const tuple& xother);


  // Dimension and member access interface:


  /// Create a tuple of dimension xdim.

  tuple(unsigned xdim);

  /// The dimension of the tuple.

  unsigned d() const;

  /// Access to xindex element.

  hsize_t& operator[](unsigned xindex);

  /// Access to xindex element.

  const hsize_t& operator[](unsigned xindex) const;

  /// Change the dimension.

  void reserve(unsigned xdim);

  /// Assign xval to each element of this.

  tuple& operator=(const hsize_t& xval);


  // Comparison interface:


  /// True if all elements are > 0.

  bool is_positive() const;

  /// True if all elements of this > corresponding elements of xother.

  bool operator>(const tuple& xother) const;

  /// True if all elements of this >= corresponding elements of xother.

  bool operator>=(const tuple& xother) const;

  /// True if all elements of this == corresponding elements of xother.

  bool operator==(const tuple& xother) const;

  /// True if all elements of this < corresponding elements of xother.

  bool operator<(const tuple& xother) const;

  /// True if all elements of this <= corresponding elements of xother.

  bool operator<=(const tuple& xother) const;


 protected:

  unsigned  _d;   //< dimension.

  hsize_t*  _mem; //< storage for elements.
};

#endif
