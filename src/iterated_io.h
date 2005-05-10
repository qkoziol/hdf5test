#ifndef SEQUENTIAL_TEST_H
#define SEQUENTIAL_TEST_H

#include "iterator.h"
#include "test.h"

/*
  A test in which the operation tested occurs in a sequence
  of atomic steps.  Between each step, control is returned
  to the client.  The client can then change some aspect of
  the test environment in preparation for the next atomic step.
*/

class sequential_test : public iterator, public test
{
 public:
};

#endif
