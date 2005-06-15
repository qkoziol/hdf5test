#ifndef DFT_H
#define DFT_H

#include "std_stack.h"
#include "traverser.h"

/*! @class dft
    @brief A depth first traverser of the (possibly cyclic) directed graph structure of
           an HDF5 file.  This is an abstract class - concrete descendants must define
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

class dft : public traverser
{
 public:


  // Standard features:


  /// Default constructor.

  dft();

  /// Destructor.

  virtual ~dft();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  dft(const dft& xother);

  /// Assignment operator.

  dft& operator=(const dft& xother);


  // Traversal interface:


  /// Does a depth first traversal of graph starting at xstart, which may be a group,
  /// dataset, named datatype, or soft link.  The traversal can be filtered.  If
  /// (xfilter & DATASET), for example, then datasets will not be visited.

  void traverse(hid_t xstart, int xfilter = 0);

  /// What to do when a node is encountered for the first time in a depth first
  /// traversal.

  virtual void preorder_action() = 0;

  /// What to do when a node is encountered for the last time in a depth first
  /// traversal.

  virtual void postorder_action() = 0;


 protected:

  /// Does a depth first traversal of graph starting at xstart, which may be a group,
  /// dataset, named datatype, or soft link.  The traversal can be filtered.  If
  /// (xfilter & DATASET), for example, then datasets will not be visited.
  /// reset() is called immediately after xstart is saved.

  void traverse(const node& xstart, int xfilter);

  /// The current object, visible during the traversal.

 const persistent& current() const;

  /// The hid of the current object, visible during the traversal.

  hid_t current_hid() const;

  /// Name of current object.  Pathname if xpath is true.

  const char* name(bool xpath = false);

  /// The depth of search below start.

  int depth() const;


 private:


  /// Recursively begins new traversals at the heads of the links emanating from xnode.

  void follow_group_links(const node& xnode, int xfilter);

  /// Recursively begins new traversals at the heads of the links to attributes.

  void traverse_attrs(const node& xnode, int xfilter);


  // Data:

  stack<node>  _current;  //< Top is current node being visited in traversal.
};

#endif
