#ifndef MATRIX_WRITER_H
#define MATRIX_WRITER_H

#include "dataset.h"
#include "hdf5.h"
#include "matrix.h"
#include "memory.h"
#include "partial.h"
#include "temp_file.h"

/*! @class matrix_writer
    @brief A class that writes a matrix to a dataset.
*/

class matrix_writer : public partial
{
 public:
  

  // Standard features:


  /// Default constructor.  Makes a dataset in xfile of HDF5 datatype `xtype' using
  /// creation properties `xcreate_plist' as the destination for the matrix write.  To
  /// exercise chunking, specify chunking and the chunk size in xcreate_plist through
  /// the HDF5 API calls H5Pset_layout() and H5Pset_chunk(). 

  matrix_writer(const temp_file& xfile,
	        const matrix&    xmat,
	        hid_t            xtype = H5T_NATIVE_INT,
	        hid_t            xcreate_plist = H5P_DEFAULT);

  /// Destructor.

  ~matrix_writer();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  matrix_writer(const matrix_writer& xother);

  /// Assignment operator.

  matrix_writer& operator=(const matrix_writer& xother);


  // Partial i/o interface:


  /// Initialize state of matrix_writer for first partial write.

  virtual void start();

  /// Increment dataspaces for next partial write.

  virtual void next();

  /// Returns true when entire matrix is written.

  virtual bool is_done() const;

  /// Write a portion of the matrix in a single operation.

  virtual bool do_partial_io();

  /// Each partial write accesses either a contiguous set of rows or
  /// a contiguous set of columns.

  enum access {BY_ROWS, BY_COLUMNS};

  /// Set the matrix access pattern: xct rows or columns at each partial write,
  /// and by rows or by columns.

  void set_access(unsigned xper_write_ct = 1, access xaccess = BY_ROWS);

  /// The number of rows or columns written thus far.

  unsigned ct() const;

  /// The number of columns or rows to write at each interation.

  unsigned per_write_ct() const;

 protected:

  dataset  _dataset;       ///< The dataset used as the destination of the write.
  unsigned _max_write_ct;  ///< The max number of rows or columns to write at each iteration.
  unsigned _cur_write_ct;  ///< The number of rows or columns to write at this iteration.
  unsigned _accum_ct;      ///< The number of rows or columns written thus far.
  access   _access;        ///< Are partial writes by rows or columns?
  matrix   _mat;           ///< The dimensions of the matrix being written.
  memory   _mem;           ///< The memory buffer.
  hid_t    _type;          ///< Datatype transferred.
};

#endif
