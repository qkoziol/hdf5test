#ifndef IO_PERF_H
#define IO_PERF_H

class memory;
class pcontainer;
#include "perf_data.h"
#include <ostream>
#include "test.h"

/*! @class io_perf
    @brief A test in which the operation tested is just a single read or write.
*/

class io_perf : public test
{
 public:


  // Standard features:


  /// Default constructor.

  io_perf();

  /// Destructor.

  virtual ~io_perf();

  /// Class invariant.  Should alwats be true.

  bool invariant() const;

  /// Copy constructor.

  io_perf(const io_perf& xother);

  /// Assignment operator.

  io_perf& operator=(const io_perf& xother);


  // Testing interface:


  /// Test data transfer from xmem to xcon.

  void run_test(const memory& xmem, pcontainer& xcon);

  /// Test data transfer from xcon to xmem.

  void run_test(pcontainer& xcon, memory& xmem);

  /// Execute run_io_perf_test() and measure the performance in bytes/second.

  virtual bool run_test();

  /// Number of bytes transferred during test.

  double bytes() const;

  /// Elapsed time in seconds for test.

  double elapsed() const;


 protected:


  perf_data  _results;  ///< Results of test;
};

#endif
