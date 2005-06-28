#ifndef HYPERSLAB_H
#define HYPERSLAB_H

class extent;
#include "std_iostream.h"
#include "tuple.h"

/*! @class hyperslab
    @brief An HDF5 hyperslab.

    <p>
       A hyperslab is a set of regularly spaced contiguous blocks of discrete
       points in an n-dimensional space.
    </p>
    <p>
       The size of a contiguous block of points is given by its size in each
       dimension of the space.
    </p>
    <p>
       The arrangement of blocks in space is described by an origin
       somewhere in the space, by the number of blocks in each dimension,
       and by the separation between blocks, or stride, in each dimension.
    </p>
*/

class hyperslab
{

  friend ostream& operator<<(ostream& xos, const hyperslab& xhyperslab);

 public:


  // Standard features:


  /// Default constructor; makes hyperslab of dimension 0.

  hyperslab();

  /// Copy constructor.

  hyperslab(const hyperslab& xother);

  /// Destructor.

  ~hyperslab();

  /// Assignment operator.

  hyperslab& operator=(const hyperslab& xother);

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Create a hyperslab of given block size, origin, stride, and ct block spacing.

  hyperslab(const tuple& xblock_size,
            const tuple& xorigin,
	    const tuple& xstride,
	    const tuple& xct);

  /// Create a hyperslab of dimension xdim and given block size, origin, stride, and ct block spacing.

  hyperslab(int* xblock_size,
            int* xorigin,
	    int* xstride,
	    int* xct,
	    int  xdim);

  /// Create a hyperslab of dimension xdim.  Block size is 1, origin is 0,
  /// stride is 1, and ct is 1 in all dimensions.

  hyperslab(unsigned xdim);

  /// It would be convenient to be able to make a hyperslab that is
  /// in some sense compatible with the extent of a dataset's dataspace.
  /// This makes a hyperslab whose block size is 1, whose origin is 0,
  /// and whose stride and ct in all dimensions matches the size of
  /// the extent (not its max_size).

  hyperslab(const extent& xextent);


  // Queries and Commands:


  /// The number of dimensions in a hyperslab.

  unsigned d() const;

  /// The origin of dimension xdim.

  hsize_t origin(unsigned xdim) const;

  /// The origin in all dimensions.

  tuple& origin();
  const tuple& origin() const;

  /// The stride in dimension xdim.

  hsize_t stride(unsigned xdim) const;

  /// The stride in all dimensions.

  tuple& stride();
  const tuple& stride() const;

  /// The number of strides from the origin in dimension xdim.

  int ct(unsigned xdim) const;

  /// The number of strides from the origin in all dimensions.

  tuple& ct();
  const tuple& ct() const;

  /// Block size in dimension xdim.

  hsize_t block_size(unsigned xdim) const;

  /// The block size in all dimensions.

  tuple& block_size();
  const tuple& block_size() const;


  // Restriction interface:


 protected:

  // Data members:

  tuple _block_size;  ///< block size in each dimension
  tuple _origin;      ///< coordinates of origin of blocks
  tuple _stride;      ///< separation of blocks in each dimension
  tuple _ct;          ///< number of blocks in each dimension

 private:


};

#endif
