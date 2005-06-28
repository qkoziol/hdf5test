#ifndef TRAVERSER_H
#define TRAVERSER_H

#include "array_of.h"
#include "hdf5.h"
#include "node.h"
#include "objno.h"
class persistent;
#include "std_iostream.h"
#include "std_set.h"
#include "std_string.h"

/*! @class traverser
    @brief Implements common functions for descendants dft and bft.
*/


class traverser
{
 public:


  // Standard features:


  /// Default constructor.

  traverser();

  /// Destructor.

  virtual ~traverser();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  traverser(const traverser& xother);

  /// Assignment operator.

  traverser& operator=(const traverser& xother);


  // Traversal interface:

  /// Simple filters on graph nodes.

  enum filter {NONE = 0, DATASET = 1, DATATYPE = 2, ATTRIBUTE = 4, ALL = 7};

  /// Traverses graph starting at xstart, which may be a group,
  /// dataset, named datatype, or soft link.  The traversal can be filtered.  If
  /// (xfilter & DATASET), for example, then datasets will not be visited.

  virtual void traverse(hid_t xstart, int xfilter = 0) = 0;

  /// Sets initial state of traverser when traverse() is called.

  virtual void reset() = 0;

  /// Returns the starting point of the traversal.

  hid_t start() const;


 protected:


  /// Traverses graph starting at xstart, which may be a group,
  /// dataset, named datatype, or soft link.  The traversal can be filtered.  If
  /// (xfilter & DATASET), for example, then datasets will not be visited.

  virtual void traverse(const node& xstart) = 0;

  /// An object corresponding to xhid, of the right dynamic type.

  const persistent& obj(hid_t xhid) const;

  /// The hid of the current object, visible during the traversal.

  virtual hid_t current_hid() const = 0;

  /// Name of xnode.  Pathname if xpath is true.

  const char* name(const node& xnode, bool xpath = false);

  /// Is xloc a graph node?

  bool is_node(hid_t xloc) const;

  /// Mark xnode.

  void mark(const node& xnode);

  /// Has xnode been marked?

  bool is_marked(const node& xnode) const;

  /// Continues traversal at the heads of the links emanating from xnode.

  virtual void follow_group_links(const node& xnode) = 0;

  /// Continues traversals at the heads of the links to attributes.

  virtual void traverse_attrs(const node& xnode) = 0;

  /// Decides whether xnode passes through filter.

  bool passes(const node& xnode) const;


  // Data:


  node            _start;   ///< Starting point of traversal.
  set<objno>      _marked;  ///< The set of nodes that have been marked.
  array_of<char>  _name;    ///< Buffer to hold object names in various class operators.
  int             _filter;  ///< Which classes of nodes are filtered from visitation.
};

#endif
