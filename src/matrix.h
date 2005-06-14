#ifndef MATRIX_H
#define MATRIX_H

#include "extent.h"
#include "hyperslab.h"
#include "std_iostream.h"

/*! @class matrix
    @brief A 2-d matrix in C (row major) storage order.
*/

class matrix
{

  /// Append a matrix to an ostream.

  friend ostream& operator<<(ostream& xos, const matrix& xmatrix);

 public:


  // Standard features:


  /// Default constructor.  Makes a C matrix with zero columns and zero rows.

  matrix();

  /// Copy constructor.

  matrix(const matrix& xother);

  /// Destructor.

  ~matrix();

  /// Assignment operator.

  matrix& operator=(const matrix& xother);

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Create a matrix with "xrow" rows and "xcol" columns.

  matrix(unsigned xrow, unsigned xcol);

  /// Test of equality.

  bool operator==(const matrix& xother) const;

  /// Change existing matrix to new dimensions.

  void reserve(unsigned xrow, unsigned xcol);

  /// Change number of rows.

  void put_row(unsigned xrow);

  /// Change number of columns.

  void put_col(unsigned xcol);

  /// Number of rows.

  unsigned row_ct() const;

  /// Number of columns.

  unsigned col_ct() const;

  /// Define a hyperslab corresponding to entire matrix.

  void select(hyperslab& xresult) const;

  /// Define a hyperslab corresponding to row xrow.

  void select_row(unsigned xrow, hyperslab& xresult) const;

  /// Define a hyperslab corresponding to rows xfirst, xfirst+1, ... xfirst+xct-1.

  void select_rows(unsigned xfirst, unsigned xct, hyperslab& xresult) const;

  /// Define a hyperslab corresponding to column xcol.

  void select_col(unsigned xcol, hyperslab& xresult) const;

  /// Define a hyperslab corresponding to columns xfirst, xfirst+1, ..., xfirst+xct-1.

  void select_cols(unsigned xfirst, unsigned xct, hyperslab& xresult) const;

  /// Get the extent of the matrix.

  void get_extent(extent& xresult) const;

 protected:

  unsigned _row_ct;  //< Number of rows.

  unsigned _col_ct;  //< Number of columns;

 private:

};

#endif
