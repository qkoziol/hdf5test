#ifndef DFT_IO_TEST_H
#define DFT_IO_TEST_H

#include "dft.h"
#include "io_perf.h"

/*! @class dft_io_perf
    @brief A depth first traverser which runs an io performance test (read only)
           on every container it finds (in preorder).
*/


class dft_io_perf : public dft
{
 public:


  // Standard features:


  /// Default constructor.

  dft_io_perf();

  /// Destructor.

  ~dft_io_perf();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  dft_io_perf(const dft_io_perf& xother);

  /// Assignment operator.

  dft_io_perf& operator=(const dft_io_perf& xother);


  // Traversal interface:


  /// Run i/o (read only) test on containers encountered and print performance to stdout.

  virtual void preorder_action();

  /// Do nothing.

  virtual void postorder_action();

  /// Number of containers whose i/o test succeeded.

  unsigned success_ct() const;

  /// Number of containers whose i/o test failed.

  unsigned failure_ct() const;

  /// Resets success and failure counts to zero.

  void reset();

 protected:

  io_perf  _tester;      //< The test to be run on every container.
  unsigned _success_ct;  //< Number of containers whose i/o test succeeded.
  unsigned _failure_ct;  //< Number of containers whose i/o test failed.
};

#endif
