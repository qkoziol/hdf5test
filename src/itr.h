#ifndef ITR_H
#define ITR_H

/*
  An abstract iterator.  A machine that steps through
  some process sequentially.
*/

class itr
{
 public:


  // Standard features:


  // Default constructor.

  itr();

  // Destructor.

  virtual ~itr();

  // Class invariant.

  bool invariant() const;

  // Copy constructor.

  itr(const itr& xother);

  // Assignment.

  itr& operator=(const itr& xother);


  // Abstract iteration interface:


  // Start the iteration.

  virtual void start() = 0;

  // Continue to the next step in the iteration.

  virtual void next() = 0;

  // Is the iteration finished?

  virtual bool is_done() const = 0;

 protected:

  bool _is_done;  //  Finished?
};

#endif
