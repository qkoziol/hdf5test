#ifndef ROW_ITERATOR_H
#define ROW_ITERATOR_H

class matrix;
#include "slice_itr.h"

/*
  Iterator over the rows of a matrix.
*/

class row_iterator : public slice_itr
{
 public:

  virtual void start();

  virtual void next();

  virtual bool is_done() const;

  // Iterate ovet rows of xmat.

  row_iterator(const matrix& xmat);

};

#endif
