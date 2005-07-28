#ifndef PERF_DATA_H
#define PERF_DATA_H

#include "std_cstddef.h"

/*! @class perf_data
    @brief Just a struct to hold standard performance data.
 */

class perf_data
{
 public:

  size_t  bytes;      ///< Number of bytes transferred in an i/o operation.
  double  open_file;  ///< Time to open the hdf5 file.
  double  open;       ///< Time to open container in seconds.
  double  extend;     ///< Time to extend a chunked dataset.
  double  elapsed;    ///< Time to perform i/o operation in seconds.
  double  close;      ///< Time to close container in seconds.
  double  close_file; ///< Time to close the hdf5 file.


  // Some conveniences:


  /// Default constructor.  Initializes to zero.

  perf_data();

  /// Assigns x to each field.

  perf_data& operator=(size_t x);
};

#endif
