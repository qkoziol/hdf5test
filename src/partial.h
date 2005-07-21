#ifndef PARTIAL_H
#define PARTIAL_H

#include "test.h"

/*! @class partial
    @brief An abstract class that can measure the performance of
           iterated partial i/o operations.
*/

class partial : public test
{
 public:


  // Standard features:


  /// Default constructor.

  partial();

  /// Destructor.

  ~partial();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  partial(const partial& xother);

  /// Assignment operator.

  partial& operator=(const partial& xother);


  // Partial i/o interface:


  /// Performs all necessary work before the first partial i/o step is executed.

  virtual void start() = 0;

  /// Performs any necessary work before the next partial i/o step is executed.
  /// This typically consists of changing a dataspace selection, but might also
  /// involve changing the extent of a chunked dataset.

  virtual void next() = 0;

  /// Should return true after the last partial i/o step is executed and false
  /// before that.

  virtual bool is_done() const = 0;

  /// Attempts a partil i/o operation.  Returns true if the attempt succeeded, false
  /// if it failed.

  virtual bool do_partial_io() = 0;


  // Test interface:


  /// Runs all of the partial i/o operations from start() to is_done().

  virtual bool run_test();


 protected:


};

#endif
