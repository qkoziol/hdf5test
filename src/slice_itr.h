#ifndef SLICE_ITERATOR_H
#define SLICE_ITERATOR_H

#include "hyperslab.h"
#include "itr.h"

class slice_itr : public itr
{
 public:

  virtual void start();

  virtual void next();

  virtual bool is_done() const;

  // Iterate over xindex of xhs.

  slice_itr(const hyperslab& xhs, int xindex);

};

#endif
