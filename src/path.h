#ifndef PATH_H
#define PATH_H

#include "std_iostream.h"

/*
  A pathname with stack-like behavior for adding and
  removing segments of the name.
*/

class path
{
  friend ostream& operator<<(ostream& xos, const path& xpath);

 public:


  // Standard features:


  // Default constructor.

  path();

  // Destructor.

  ~path();

  // Class invariant.

  bool invariant() const;

  // Copy constructor.

  path(const path& xother);

  // Assignment.

  path& operator=(const path& xother);


  // Stack features:


  // Max allowable number of chars in a component name.

  static const size_t MAX_COMPONENT_SIZE = 4096;

  // Push a component onto the stack.  This function examines
  // xcomponent up to the first MAX_COMPONENT_SIZE chars or a
  // terminating '\0', whichever comes first.

  void push(const char* xcomponent);

  // Get the top component.  A terminating '\0'
  // will be supplied.

  const char* top() const;

  // Pop the stack.

  void pop();

  // Size of the whole pathname, including the terminating '\0'.

  size_t ct() const;

  // Size of the top component, including the terminating '\0'.

  size_t top_ct() const;

  // Capacity of the internal buffer.

  size_t ub() const;

  // Depth of stack, i.e., number of path components.

  int depth() const;


  // Non-stack features:


  // The whole pathname.  A terminating '\0' will be
  // supplied.

  const char* whole() const;

 protected:

  char*   _buf;   // The buffer holding all components of the pathname.
  size_t  _ub;    // The capacity of the buffer.
  size_t  _ct;    // Number of chars of _buf currently in use;
  int     _depth; // Number of items on stack.

 private:
};

#endif
