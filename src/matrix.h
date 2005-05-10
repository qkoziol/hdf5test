#ifndef MATRIX_H
#define MATRIX_H

#include "hyperslab.h"
#include "std_iostream.h"

/*
  A 2-d matrix.
*/

class matrix : public hyperslab
{

  friend ostream& operator<<(ostream& xos, const matrix& xmatrix);

 public:

  // Canonical features:

  // Copy constructor.

  matrix(const matrix& xother);

  // Destructor.

  ~matrix();

  // Assignment operator.

  matrix& operator=(const matrix& xother);

  // Class invariant.

  bool invariant() const;

  // Style of matrix.  C is row major order and Fortran is
  // column major order.

  enum mode {C, Fortran};

  // Create a matrix with xrow rows and xcol columns.

  matrix(int xrow, int xcol, mode xmode);

 protected:
 private:

  // Default constructor.

  matrix();

};

#endif
