#ifndef MATRIX_WRITER_H
#define MATRIX_WRITER_H

#include "matrix_io.h"

/*! @class matrix_writer
    @brief A class that writes a matrix to a dataset.
*/

class matrix_writer : public matrix_io
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


 protected:

  /// Write a heading for the columns of performance data reported.

  void write_col_heading() const;

  /// Write results of partial i/o op to stdout.

  virtual void write_results();
};

#endif
