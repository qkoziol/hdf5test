#ifndef DFT_READ_PERF_H
#define DFT_READ_PERF_H

#include "dft.h"
#include "io_perf.h"
#include "memory.h"

/*! @class dft_read_perf
    @brief A depth first traverser which runs an io performance test (read only)
           on every container it finds (in preorder).
*/


class dft_read_perf : public dft
{
 public:


  // Standard features:


  /// Default constructor.

  dft_read_perf();

  /// Destructor.

  ~dft_read_perf();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  dft_read_perf(const dft_read_perf& xother);

  /// Assignment operator.

  dft_read_perf& operator=(const dft_read_perf& xother);


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

  /// Turns verbose output on (if x is true) and off (if x is false).  Verbose
  /// output prints the dataset/attribute dataspace, storage mode, etc.

  void  set_verbose(bool x);

  /// Is output mode verbose?

  bool is_verbose() const;

 protected:

  io_perf             _tester;      ///< The test to be run on every container.
  memory              _dest;        ///< Memory buffer used as destination for reads of containers.
  unsigned            _success_ct;  ///< Number of containers whose i/o test succeeded.
  unsigned            _failure_ct;  ///< Number of containers whose i/o test failed.
  size_t              _longest;     ///< The size of the longest name encountered.  Aids formatting of output.
  std::stack<double>  _open;        ///< Elapsed time to open current container.
  std::stack<double>  _close;       ///< Elapsed time to close current container.
  bool                _verbose;     ///< Output mode.
};

#endif
