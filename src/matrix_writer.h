#ifndef MATRIX_WRITER_H
#define MATRIX_WRITER_H

#include "dataset.h"
#include "hdf5.h"
#include "matrix.h"
#include "memory.h"
#include "partial.h"
#include "tuple.h"

/*! @class matrix_writer
    @brief A class that writes a matrix to a dataset.
*/

class matrix_writer : public partial
{
 public:
  

  // Standard features:


  /// Default constructor.

  matrix_writer();

  /// Destructor.

  ~matrix_writer();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  matrix_writer(const matrix_writer& xother);

  /// Assignment operator.

  matrix_writer& operator=(const matrix_writer& xother);


  // Partial i/o interface:


  bool run_test(matrix& xmat, memory& xsrc, dataset& xdest);


  /// Initialize state of matrix_writer for first partial write.

  virtual void start();

  /// Increment dataspaces for next partial write.

  virtual void next();

  /// Returns true when entire matrix is written.

  virtual bool is_done() const;

  /// Write a portion of the matrix in a single operation.

  virtual bool do_partial_io();


  /// Set the matrix access pattern: xct rows or columns at each partial write,
  /// and by rows if xby_rows, or by columns otherwise.

  void set_access(unsigned xper_write_ct = 1, bool xby_rows = true);

  /// Is access by rows or by columns?

  bool access_is_by_rows() const;

  /// The number of rows or columns written thus far.

  unsigned ct() const;

  /// The number of columns or rows to write at each interation.

  unsigned per_write_ct() const;

  /// The number of matrix elements written at each iteration.

  unsigned per_write_npoints() const;

 protected:

  unsigned _max_write_ct;  ///< The max number of rows or columns to write at each iteration.
  unsigned _cur_write_ct;  ///< The number of rows or columns to write at this iteration.
  unsigned _accum_ct;      ///< The number of rows or columns written thus far.
  tuple    _size;          ///< The size of the matrix on disk after this iteration's write.
  tuple    _max_size;      ///< The maximum size of the matrix on disk.
  bool     _by_rows;       ///< Are partial writes by rows or columns?
  matrix*  _mat;           ///< The matrix being written.
  dataset* _dest;          ///< The dataset acting as the destination of each write.
  memory*  _src;           ///< The memory buffer acting as the source of each write.

};

#endif
