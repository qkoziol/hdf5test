#ifndef MATRIX_READER_H
#define MATRIX_READER_H

#include "matrix_io.h"

/*! @class matrix_reader
    @brief A class that reports the performance of reading a matrix from dataset.
*/

class matrix_reader : public matrix_io
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


 protected:

  /// Write a heading for the columns of performance data reported.

  void write_col_heading() const;

  /// Write results of partial i/o op to stdout.

  virtual void write_results();
};

#endif
