#ifndef SLICE_H
#define SLICE_H

/*
  A 1-d subset of a hyperslab.
*/

class slice
{
 public:

  // Make a new slice along dimension xindex of xbasis.

  slice(const hyperslab& xbasis, size_t xindex);

  // Return the basis of this slice.

  const hyperslab& basis() const;

  // Return the index of the slice.

  size_t index() const;
};

#endif
