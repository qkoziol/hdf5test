#ifndef COL_ITR_H
#define COL_ITR_H

#include "matrix.h"
#include "slice_itr.h"

/*
  Iterator over the columns of a matrix.
*/

class col_itr : public slice_itr
{

  friend ostream& operator<<(ostream& xos, const col_itr& xcol_itr);

 public:

  virtual void start();

  virtual void next();

  virtual bool is_done() const;

  // Iterate over columns of xmat.

  col_itr(const matrix& xmat);

};

#endif
