#ifndef TEST_H
#define TEST_H

#include "std_iostream.h"
#include "timer.h"

/*! @class test
    @brief A general interface to all performance tests.  This will
           undoubtedly include machinery (not shown) for uniform
	   invocation of the test and uniform reporting of results.
	   There may be a special interface (not shown) to deal with
	   parallel tests.
*/

class test
{
  // Write test results to xos.

  friend ostream& operator<<(ostream& xos, const test& xtest);

 public:


  // Standard features:


  /// Default constructor.

  test();

  /// Destructor.

  virtual ~test();

  /// Class invariant.  Should always return true.

  bool invariant() const;

  /// Copy constructor.

  test(const test& xother);

  /// Assignment operator.

  test& operator=(const test& xother);


  // Test interface:


  /// Run the test, compute performance, and store internally
  /// the status and performance figures.

  virtual void run_test() = 0;

  /// Possible states of the test.  If it was run, it either
  /// succeeded or failed.  But it may not have been run yet.

  enum state {SUCCESS, FAILURE, NOT_RUN};

  /// Did the test run succeed or fail?

  state status() const;

  /// The performance of this test, typically interpreted as bytes/second.

  double performance() const;

  /// Makes a name consisting of
  static string& make_test_name();

 protected:

  double _performance; // a measure of the performance of this test.
  state  _status;      // current status of this test.
  timer  _timer;       // for timing whatever the test does.

};

#endif
