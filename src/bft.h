#ifndef BFT_H
#define BFT_H

#include "std_queue.h"
#include "traverser.h"

/*! @class bft
    @brief A breadth first traverser of the (possibly cyclic) directed graph structure of
          an HDF5 file.  This is an abstract class - concrete descendants must define
          what to do during traversal by supplying a visit_action() function.
*/


class bft : public traverser
{
 public:


  // Standard features:


  /// Default constructor.

  bft();

  /// Destructor.

  ~bft();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  bft(const bft& xother);

  /// Assignment operator.

  bft& operator=(const bft& xother);


  // Traversal interface:


  /// Does a breadth first traversal of graph starting at xstart, which may be a group,
  /// dataset, named datatype, or soft link.  The traversal can be filtered.  If
  /// (xfilter & DATASET), for example, then datasets will not be visited.

  void traverse(hid_t xstart, int xfilter = 0);

  /// What to do when a node is visited.

  virtual void visit_action() = 0;


 protected:

  /// Does a breadth first traversal of graph starting at xstart, which may be a group,
  /// dataset, named datatype, or soft link.  The traversal can be filtered.  If
  /// (xfilter & DATASET), for example, then datasets will not be visited.

  void traverse(const node& xstart, int xfilter);


  /// The current object, visible during the traversal.

  const persistent& current() const;

  /// The hid of the current object, visible during the traversal.

  const hid_t current_hid() const;

  /// Name of current object.  Pathname if xpath is true.

  const char* name(bool xpath = false);


 private:


  /// Adds heads of the links emanating from xnode to queue of nodes to visit.

  void follow_group_links(const node& xnode, int xfilter);

  /// Adds heads of the links emanating from xnode to queue of nodes to visit.

  void traverse_attrs(const node& xnode, int xfilter);


  // Data:

  node          _current;  //< The node currently being visited.
  queue<node>  _pending;   //< The queue of nodes encountered but not yet visited.
};

#endif
