#ifndef EXTENT_H
#define EXTENT_H

#include "hdf5.h"
#include "std_iostream.h"
#include "tuple.h"

/*! @class extent
    @brief A bounding box around a space of integer valued n-dimensional
           tuples or coordinates or indices.
    <p>
       The bounding box has two measures of size:
       <ol>
           <li>
               size(), which describes how big it is in each dimension, and
           </li>
           <li>
               max_size(), which describes the maximum size it can be in each dimension.
           </li>
        </ol>
     </p>
*/

class extent
{

  friend std::ostream& operator<<(std::ostream& xos, const extent& xextent);

 public:

  // Standard features:

  /// Default constructor.  Makes an extent of dimension 0.

  extent();

  /// Copy constructor.

  extent(const extent& xother);

  /// Destructor.

  ~extent();

  /// Assignment operator.

  extent& operator=(const extent& xother);

  /// Class invariant.  Should always be true.

  bool invariant() const;


  // Queries:


  /// Equality operator.

  bool operator==(const extent& xother) const;

  /// Number of dimensions of bounding box.

  unsigned d() const;

  /// Number of points in size of extent.

  hsize_t npoints() const;

  /// Maximum size in a given dimension, 0 <= xdim < d().

  hsize_t max_size(unsigned xdim) const;

  /// Max size in all dimensions.

  const tuple& max_size() const;

  /// Max size in all dimensions.

  tuple& max_size();

  /// Size in a given dimension.

  hsize_t size(unsigned xdim) const;

  /// Size in all dimensions.

  const tuple& size() const;

  /// Size in all dimensions.

  tuple& size();


  // Change of extent.


  /// Add another dimension.

  void add(hsize_t xsize, hsize_t xmax_size);

  /// Define number and sizes of all dimensions.

  void set(const tuple& xsize, const tuple& xmax_size);

  /// Create with defined number and sizes of all dimensions.

  extent(const tuple& xsize, const tuple& xmax_size);

  /// Define dimension, set size and max_size to zero.

  void reserve(unsigned xdim);

  /// Create with defined dimension, 0 size and max_size.

  extent(unsigned xdim);

 protected:
 private:

  tuple  _max_size;

  tuple  _size;

};

#endif
