#ifndef IO_PERF_H
#define IO_PERF_H

class memory;
class pcontainer;
#include "std_iostream.h"
#include "test.h"

/*
  A test in which the operation tested is just a single
  read or write.
*/

class io_perf : public test
{

  friend ostream& operator<<(ostream& xos, const io_perf& xiot);

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

  void run_test(const pcontainer& xcon, memory& xmem);

  // Execute run_io_perf_test() and measure the performance in bytes/second.

  virtual void run_test();

 protected:

  size_t _bytes;    //< Bytes transferred.

  double _elapsed;  //< Seconds elapsed during i/o operation.
};

#endif
