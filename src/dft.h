#ifndef DFT_H
#define DFT_H

#include "hdf5.h"
#include "path.h"
class persistent;
#include "std_iostream.h"
#include "std_set.h"
#include "std_stack.h"
#include "std_string.h"

/*! @class dft
    @brief A depth first traverser of the (possibly cyclic) directed graph structure of
          an HDF5 file.  This is an astract class - concrete descendants must define
          what to do during traversal by supplying preorder_action() and postorder_action()
          functions.
*/

/*
  The structure of hdf5 file is a directed graph.
  References:
    Aho & Ullman, Foundations of Computer Science, C edition
    http://www.fas.harvard.edu/~libcs124/SUMMER/lec3ext.pdf

  Nodes:

    The nodes are groups, datasets, and named datatypes.

    Nodes have a reference ct > 0, which is the number of links
    pointing to them.

    Groups

      The tail of every link is a group.

      There exists exactly one root group.  It cannot be deleted, has ref ct of one
      even though it is not at the head of any link.

  Links:

    There are 2 kinds of links, hard and soft/symbolic.

    Hard links point to exactly one node.

    Soft links point to at most one node.

    Links have names.  The name of the head of a hard link is its pathname + name of the hard link

    The name of the head of a soft link is available by request from the soft link.

  Depth First Search Algorithm:

    1) mark all nodes unvisited
    2) select a node n
    3) call depth_first_search(n)
    4) depth_first_search(n)
       {
          do preorder action on n
          mark n visited

          if n is a group
          {
             for each link l emanating from n
             {
               if l->head is not visited
               {
                 depth_first_search(l->head)
               }
             }
          }
          do postorder action on n
       }

   Representation:

     HDF5 representation:

       named objects are nodes (datasets, groups, and named datatypes) and soft/symbolic links

       H5Getnum_objs(hid_t loc_id, hsize_t*  num_obj) gives the number of links whose tails are the group
       specified by loc_id.

       The nodes implied by num_obj are accessible by indices 0 <= index < num_obj

       We can get their names by H5Gget_objname_by_idx(hid_t loc_id, hsize_t idx, char *name, size_t size )
       and their types by H5Gget_objtype_by_idx

       The name of the head of a soft link can be ontained by H5Gget_linkval(hid_t loc_id, const char *name, size_t size, char *value  )
*/
/*
  A variation on the search is to search by subsets.  One way of specifying a subset is by a
  simple filtration on the type of object: dataset, attribute, named datatype, etc.

  Another, general, way is to specify a subset.  One could do this, perhaps, by first
  traversing a file and finding out what's in it, then building a set of identifiers
  of the "interesting" objects.  One could use this set of "interesting" objects as
  the subset to filter by.  The two-traversal technique seems a little clunky, though.
  Perhaps we just interpose a callback, such as a pre-or-post-order call that lets
  us examine the current object and decide whether to do something with/to it.  It
  might be a good idea to keep a stack of link names so we can generate the pathname
  to the current object on demand.
 */

class dft
{
 public:


  // Standard features:


  /// Default constructor.

  dft();

  /// Destructor.

  ~dft();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  dft(const dft& xother);

  /// Assignment operator.

  dft& operator=(const dft& xother);


  // Traversal interface:


  /// Does a depth first traversal of graph starting at xstart, which may be a group,
  /// dataset, named datatype, or soft link.

  void traverse(hid_t xstart);

  /// What to do when a node is encountered for the first time in a depth first
  /// traversal.

  virtual void preorder_action() = 0;

  /// What to do when a node is encountered for the last time in a depth first
  /// traversal.

  virtual void postorder_action() = 0;

  /// Sets initial state of traverser when traverse() is called.

  virtual void reset() = 0;


 protected:


  /// The current object, visible during the traversal.

  const persistent& current() const;

  /// The hid of the current object, visible during the traversal.

  const hid_t current_hid() const;

  /// Pathname of current object.

  const char* pathname() const;

  /// The hid of the starting object.

  const hid_t start() const;

  /// Has this node been visited before?

  bool has_been_visited(hid_t xnode) const;

  /// Is this location a graph node?

  bool is_node(hid_t xloc) const;

  /// The depth of search below start.

  int depth() const;


 private:


  /// Mark current node as having been visited.

  void mark_visited(hid_t xnode);

  /// HDF5 objects within the same file are uniquely identified by a pair
  /// of unsigned longs.  See H5Gget_objinfo() for details.  Traversing
  /// a directed graph requires having a unique identity for the objects
  /// encountered.  This will serve as the id.

  class objno
  {
   public:
     unsigned long a;
     unsigned long b;
     bool operator<(const objno& xother) const;
     objno& operator=(const objno& xother);
     objno(const H5G_stat_t& xstat);
  };

  /// Recursively begins new traversals at the heads of the links emanating from xloc.

  void follow_group_links(hid_t xloc);

  /// Recursively begins new traversals at the heads of the links to attributes.

  void traverse_attrs(hid_t xloc);


  // Data:


  hid_t         _start;             //< Starting point of traversal.
  stack<hid_t>  _current;           //< Top is current node being visited in traversal.
  path          _path;              //< Path name from _start to _current;
  set<objno>    _has_been_visited;  //< The set of nodes that has been visited during a traversal.
  hid_t         _attr_hack;         //< One item under the top of _current.  Sometimes. See HACK comment in current() implementation.
};

#endif
