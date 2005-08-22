#ifndef MATRIX_IO_H
#define MATRIX_IO_H

#include "dataset.h"
#include "hdf5.h"
#include "matrix.h"
#include "memory.h"
#include "partial.h"
#include "perf_data.h"
#include "tuple.h"

/*! @class matrix_io
    @brief A class supporting matrix i/o between memory and a dataset.
*/

class matrix_io : public partial
{
 public:
  

  // Standard features:


  /// Default constructor.

  matrix_io();

  /// Destructor.

  ~matrix_io();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  matrix_io(const matrix_io& xother);

  /// Assignment operator.

  matrix_io& operator=(const matrix_io& xother);


  // Partial i/o interface:


  /// Write a matrix from memory into an HDF5 dataset.

  bool run_test(matrix& xmat, memory& xsrc, dataset& xdest);

  /// Read a matrix from an HDF5 dataset into memory.

  bool run_test(matrix& xmat, dataset& xsrc, memory& xdest);

  using partial::run_test;


  /// Initialize state of matrix_io for first partial i/o operation.

  virtual void start();

  /// Increment dataspaces for next partial i/o operation.

  virtual void next();

  /// Returns true when entire matrix is transferred.

  virtual bool is_done() const;

  /// Write a portion of the matrix in a single operation.

  virtual bool do_partial_io();

  /// Write results of partial i/o op to stdout.

  virtual void write_results() = 0;


  /// Set the matrix access pattern: xct rows or columns at each partial i/o operation,
  /// and by rows if xby_rows, or by columns otherwise.

  void set_access(unsigned xper_transfer_ct = 1, bool xby_rows = true);

  /// Is access by rows or by columns?

  bool access_is_by_rows() const;

  /// The number of rows or columns transferreed thus far.

  unsigned ct() const;

  /// The number of columns or rows to transfer at each interation.

  unsigned per_transfer_ct() const;

  /// The number of matrix elements transferred at each iteration.

  unsigned per_transfer_npoints() const;

 protected:

  /// Select elements in source and destination dataspaces for current
  /// write.  Extend dataset if nececssary.

  void select_extend();

  /// Write a heading for the columns of performance data reported.

  virtual void write_col_heading() const = 0;

  // Data members:

  unsigned  _max_ct;    ///< The max number of rows or columns to transfer at each iteration.
  unsigned  _cur_ct;    ///< The number of rows or columns to transfer at this iteration.
  unsigned  _accum_ct;  ///< The number of rows or columns transferred thus far.
  tuple     _size;      ///< The size of the submatrix on disk after this iteration's transfer.
  tuple     _max_size;  ///< The maximum size of the matrix on disk.
  bool      _by_rows;   ///< Is partial i/o by rows or columns?
  matrix*   _mat;       ///< The matrix being transferred.
  dataset*  _dset;      ///< The dataset involved in each transfer.
  memory*   _mem;       ///< The memory buffer involved in each transfer.
  perf_data _perf;      ///< The performance of each part of the test.
  bool      _is_writer; ///< If true, writes a matrix.  If false, reads a matrix.

};

#endif
