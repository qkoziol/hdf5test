#ifndef MATRIX_READER_H
#define MATRIX_READER_H

#include "dataset.h"
#include "hdf5.h"
#include "matrix.h"
#include "memory.h"
#include "partial.h"
#include "perf_data.h"
#include "tuple.h"

/*! @class matrix_reader
    @brief A class that readss a matrix from dataset into memory.
*/

class matrix_reader : public partial
{
 public:
  

  // Standard features:


  /// Default constructor.

  matrix_reader();

  /// Destructor.

  ~matrix_reader();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  matrix_reader(const matrix_reader& xother);

  /// Assignment operator.

  matrix_reader& operator=(const matrix_reader& xother);


  // Partial i/o interface:


  /// Read a matrix from an HDF5 dataset into memory.

  bool run_test(matrix& xmat, dataset& xsrc, memory& xdest);

  using partial::run_test;


  /// Initialize state of matrix_reader for first partial read.

  virtual void start();

  /// Increment dataspaces for next partial read.

  virtual void next();

  /// Returns true when entire matrix is read.

  virtual bool is_done() const;

  /// Read a portion of the matrix in a single operation.

  virtual bool do_partial_io();

  /// Write results of partial i/o op to stdout.

  virtual void write_results();


  /// Set the matrix access pattern: xct rows or columns at each partial read,
  /// and by rows if xby_rows, or by columns otherwise.

  void set_access(unsigned xper_read_ct = 1, bool xby_rows = true);

  /// Is access by rows or by columns?

  bool access_is_by_rows() const;

  /// The number of rows or columns written thus far.

  unsigned ct() const;

  /// The number of columns or rows to read at each interation.

  unsigned per_read_ct() const;

  /// The number of matrix elements read at each iteration.

  unsigned per_read_npoints() const;

 protected:

  /// Select elements in source and destination dataspaces for current
 /// write.  Extend dataset if nececssary.

  void select_extend();

  // Data members:

  unsigned  _max_read_ct;  ///< The max number of rows or columns to read at each iteration.
  unsigned  _cur_read_ct;  ///< The number of rows or columns to read at this iteration.
  unsigned  _accum_ct;     ///< The number of rows or columns read thus far.
  tuple     _size;         ///< The size of the matrix on disk after this iteration's read.
  tuple     _max_size;     ///< The maximum size of the matrix on disk.
  bool      _by_rows;      ///< Are partial reads by rows or columns?
  matrix*   _mat;          ///< The matrix being read.
  dataset*  _src;          ///< The dataset acting as the source of each read.
  memory*   _dest;         ///< The memory buffer acting as the destination of each read.
  perf_data _perf;         ///< The performance of each part of the test.

};

#endif
