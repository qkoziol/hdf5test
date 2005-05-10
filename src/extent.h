#ifndef EXTENT_H
#define EXTENT_H

#include "hdf5.h"
#include "std_iostream.h"
#include "tuple.h"

/*
  An extent defines a bounding box around a space
  of integer valued n-dimensional tuples or coordinates
  or indices.  The box has two measures of size:

  1)  size(),     which describes how big it is in each
                  dimension, and
  2)  max_size(), which describes the maximum size it can
                  be in each dimension.
*/

class extent
{

  friend std::ostream& operator<<(std::ostream& xos, const extent& xextent);

 public:

  // Canonical features:

  // Default constructor.

  extent();

  // Copy constructor.

  extent(const extent& xother);

  // Destructor.

  ~extent();

  // Assignment operator.

  extent& operator=(const extent& xother);

  // Class invariant.

  bool invariant() const;


  // Queries:


  // Number of dimensions of box.

  int d() const;

  // Number of points in size of extent.

  hsize_t npoints() const;

  // Maximum size in a given dimension, 0 <= xdim < d().

  hsize_t max_size(int xdim) const;

  // Max size in all dimensions.

  const tuple& max_size() const;
  tuple& max_size();

  // Size in a given dimension.

  hsize_t size(int xdim) const;

  // Size in all dimensions.

  const tuple& size() const;
  tuple& size();


  // Change of extent.


  // Add another dimension.

  void add(hsize_t xsize, hsize_t xmax_size);

  // Define number and sizes of all dimensions.

  void set(int xdim, const tuple& xsize, const tuple& xmax_size);

  // Create with defined number and sizes of all dimensions.

  extent(int xdim, const tuple& xsize, const tuple& xmax_size);

  // Create with defined dimension, 0 size and max_size.

  extent(int xdim);

 protected:
 private:

  tuple  _max_size;

  tuple  _size;

};

#endif
