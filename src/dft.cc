#include "dft.h"
#include "contract.h"

#include "attribute.h"
#include "dataset.h"
#include "datatype.h"
#include "group.h"
#include "hdf5.h"
#include "std_iostream.h"

dft::
dft()
{
  // Preconditions:

  // Body:

  _start     = H5I_INVALID_HID;
  _attr_hack = H5I_INVALID_HID;

  // Postconditions:

  // TODO:
  // change reference to internal variable to reference to
  // public interface.
  assert(_start <= 0);
  assert(_path.depth() == 1);
  assert(invariant());

  // Exit:

}

dft::
~dft()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
dft::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = (_start < 0 == _current.empty());
  result = result && (_current.empty() ? true : is_node(_current.top()));
  result = result && (_path.depth() == _current.size()+1);

  // Postconditions:

  // Exit:

  return result;
}

dft::
dft(const dft& xother)
{
  not_implemented;
}

dft&
dft::
operator=(const dft& xother)
{
  not_implemented;
}


bool
dft::
is_node(hid_t xloc) const
{
  bool result;

  // Preconditions:

  // Body:

  H5I_type_t type = H5Iget_type(xloc);

  switch(type)
  {
    case H5I_DATASET:
    case H5I_DATATYPE:
    case H5I_GROUP:
    case H5I_ATTR:
      result = true;
      break;
    default:
      result = false;
      break;
  }

  // Postconditions:

  // Exit:

  return result;
}

void
dft::
mark_visited(hid_t xloc)
{
  // Preconditions:

  // Body:

  H5G_stat_t statbuf;

  herr_t err = H5Gget_objinfo(_start, _path.whole(), 0, &statbuf);

  assert(err >= 0);

  objno unique(statbuf);

  _has_been_visited.insert(unique);

  // Postconditions:

  assert(has_been_visited(xloc));

  // Exit:
}

bool
dft::
has_been_visited(hid_t xloc) const
{
  bool result;

  // Preconditions:

  // Body:

  H5G_stat_t statbuf;

  herr_t err = H5Gget_objinfo(_start, _path.whole(), 0, &statbuf);

  assert(err >= 0);

  objno unique_id(statbuf);

  if (_has_been_visited.find(unique_id) == _has_been_visited.end())
  {
    result = false;
  }
  else
  {
    result = true;
  }

  // Postconditions:

  // Exit:

  return result;
}

void
dft::
traverse(hid_t xloc)
{
  // Preconditions:

  assert(H5Iget_type(xloc) != H5I_BADID);
  assert(is_node(xloc));

  // Body:

  // On the first call to this function, _start is undefined.
  // Define it, and, now that we know this is the start of the traversal,
  // do whatever reset() actions are defined in descendants.

  if (_start == H5I_INVALID_HID)
  {
    reset();
    _start = xloc;
  }

  // If we've already visited this node, do nothing except return.
  // Otherwise, do pre- and post-vist actions, and, if possible,
  // explore the graph accessible from links emanating from this node.

  if (! has_been_visited(xloc))
  {
    // This is the first time we've encountered this node.
    // Mark it as visited and do the preorder operation on it.

    _current.push(xloc);
    mark_visited(xloc);
    preorder_action();

    // If this node is a group, there may be links emanating from it
    // that must be searched.  Investigate this possibility.

    if (H5Iget_type(xloc) == H5I_GROUP)
    {
      follow_group_links(xloc);
    }

    // Explore the attributes, if any, of this node.

    traverse_attrs(xloc);

    // We encounter this node for the last time; do postorder operation here.

    postorder_action();

    // We won't see xloc again, because it's been visited.  Pop it
    // off the stack so that "dft" forgets it, and decrement its
    // HDF5 reference count.  Be careful not to decrement the count
    // of _start, since that was supplied externally.

    assert(_current.top() == xloc);

    if (xloc != _start)
    {
      H5Idec_ref(xloc);
    }

    _current.pop();

    // Restore the invariant condition.

    if (_current.empty())
    {
      _start = H5I_INVALID_HID;
      _has_been_visited.erase(_has_been_visited.begin(), _has_been_visited.end());
    }
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

bool
dft::
objno::
operator<(const objno& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  if (a < xother.a)
  {
    result = true;
  }
  else if (a == xother.a)
  {
    if (b < xother.b)
    {
      result = true;
    }
    else
    {
      result = false;
    }
  }
  else
  {
    result = false;
  }

  // Postconditions:

  // Exit:

  return result;
}

dft::objno&
dft::
objno::
operator=(const objno& xother)
{
  objno* ptr_to_result;

  // Preconditions:

  // Body:

  a = xother.a;
  b = xother.b;
  ptr_to_result = this;

  // Postconditions:

  // Exit:

  return *ptr_to_result;
}

dft::
objno::
objno(const H5G_stat_t& xstat)
{
  a = xstat.objno[0];
  b = xstat.objno[1];
}

const persistent&
dft::
current() const
{
  persistent* ptr_to_result;

  // Preconditions:

  // TODO:
  // This is a pretty ugly assertion.  Find a way to express
  // the same idea more elegantly.
  assert(H5Iget_type(_current.top()) == H5I_DATASET || H5Iget_type(_current.top()) == H5I_ATTR || H5Iget_type(_current.top()) == H5I_DATATYPE || H5Iget_type(_current.top()) == H5I_GROUP);

  // Body:

  hid_t id = _current.top();

  switch(H5Iget_type(id))
  {
    case H5I_DATASET:
      ptr_to_result = new dataset();
      break;
    case H5I_GROUP:
      ptr_to_result = new group();
      break;
    case H5I_DATATYPE:
      ptr_to_result = new datatype();
      break;
    case H5I_ATTR:
      ptr_to_result = new attribute();
      break;
  }

  switch(H5Iget_type(id))
  {
    case H5I_DATASET:
    case H5I_GROUP:
    case H5I_DATATYPE:
      ptr_to_result->open(_start, _path.whole());
      break;
    case H5I_ATTR:
    {
      // HACK
      // A big hack.  H5Aopen_name() has different behavior
      // from H5[DGT]open().  The latter accept any path name
      // from any group and that group's id as valid arguments.
      // The former accepts only the path name from the attribute's
      // host object and that host object's id as valid arguments.
      // The host object's id is just underneath the attribute's
      // id on the stack.  We can't do the hack of popping the
      // stack to look underneath, then pushing the former top
      // back on, because this function is declared "const",
      // and those stack manipulations alter the state of "this".
      // So, instead, we keep a copy of the previous top and
      // use that.  This whole construct ought to be accomplished
      // automatically somehow without inspecting types and
      // doing special operations for certain types.

      ptr_to_result->open(_attr_hack, _path.top());
    }
    break;
  }

  // Postconditions:

  assert(ptr_to_result->is_attached());

  // Exit:

  return *ptr_to_result;
};

const char*
dft::
pathname() const
{
  const char* result;

  // Preconditions:

  // Body:

  result = _path.whole();

  // Postconditions:

  assert(result != 0);

  // Exit:

  return result;
}

int
dft::
depth() const
{
  int result;

  // Preconditions:

  // Body:

  result = _current.size();

  // Postconditions:

  assert(result >= 0);

  // Exit:

  return result;
}

const hid_t
dft::
start() const
{
  hid_t result;

  // Preconditions:

  // Body:

  result = _start;

  // Postconditions:

  // Exit:

  return result;
}

const hid_t
dft::
current_hid() const
{
  hid_t result;

  // Preconditions:

  // Body:

  result = _current.top();

  // Postconditions:

  // Exit:

  return result;
}

void
dft::
follow_group_links(hid_t xloc)
{
  // Preconditions:

  assert(H5Iget_type(xloc) == H5I_GROUP);

  // Body:

  const size_t max_name_len = path::MAX_COMPONENT_SIZE;

  // ISSUE:
  // This is a recursive routine.  We'll have depth-of-call-stack copies
  // of this buffer in memory at a time.  The depth of the stack is
  // roughly the size of the directed graph associated with the
  // HDF5 file.
  // Depth    mbytes (@4k/call)
  // O(10)     0.04
  // O(1000)   4
  // O(10000) 40
  // Probably not a huge concern at the moment.

  char name[max_name_len];

  hsize_t num_objs;

  herr_t err = H5Gget_num_objs(xloc, &num_objs);

  assert(err >= 0);

  // Loop through all the links.  If the head of the link is a node
  // start another depth first traversal from that point.

  for (hsize_t link = 0; link < num_objs; ++link)
  {
    int head_type = H5Gget_objtype_by_idx(xloc, link);

    // How big is the name of the head?
    // ISSUE
    // path::MAX_COMPONENT_SIZE may be smaller than this which
    // results in silent truncation of the pathname and a subsequent
    // inability to communicate to HDF5 a correct pathname.  Does the
    // HDF5 API have a query that allows us to determine how big such
    // a name might be?  In the absence of anything better, I've made
    // path::MAX_COMPONENT_SIZE something fairly big.

    ssize_t nchars = H5Gget_objname_by_idx(xloc, link, name, max_name_len);

    hid_t head = H5I_INVALID_HID;

    switch (head_type)
    {
      case H5G_LINK:
	break;

      case H5G_GROUP:
	head = H5Gopen(xloc, name);
	assert(head >= 0);
	break;

      case H5G_DATASET:
	head = H5Dopen(xloc, name);
	assert(head >= 0);
	break;

      case H5G_TYPE:
	head = H5Topen(xloc, name);
	assert(head >= 0);
	break;
    }

    // We may have found a new node at the head of a link emanating from xloc.
    // Start a new depth first search starting at this new node.

    if (is_node(head))
    {
      _path.push(name);

      // It's unusual to check the class invariant in the middle of a function,
      // but we're leaving the function (temporarily) and we always check
      // the invariant at function exits.

      assert(invariant());

      traverse(head);

      // We're done with "name".  Pop it off the stack.

      _path.pop();
    }
  }

  // Postconditions:

  // ISSUE
  // The invariant check fails here, apparently because we're in the middle
  // of a traversal.  That means that the invariant isn't written properly.
  //assert(invariant());

  // Exit:
}

void
dft::
traverse_attrs(hid_t xloc)
{
  // Preconditions:

  assert(H5Iget_type(xloc) == H5I_GROUP || H5Iget_type(xloc) == H5I_DATASET || H5Iget_type(xloc) == H5I_DATATYPE);

  // Body:

  const size_t max_name_len = path::MAX_COMPONENT_SIZE;

  // Unlike follow_group_links(), this is not a recursive routine, since
  // attributes don't have any links emanating from them.  So there's
  // no concern about having lots of large name buffers on a stack.

  char name[max_name_len];

  int num_objs = H5Aget_num_attrs(xloc);

  _attr_hack = xloc;

  // Visit the attributes one by one.

  for (int link = 0; link < num_objs; ++link)
  {

    hid_t head = H5Aopen_idx(xloc, link);

    // How big is the name of the head?
    // ISSUE
    // path::MAX_COMPONENT_SIZE may be smaller than this which
    // results in silent truncation of the pathname and a subsequent
    // inability to communicate to HDF5 a correct pathname.  Does the
    // HDF5 API have a query that allows us to determine how big such
    // a name might be?  In the absence of anything better, I've made
    // path::MAX_COMPONENT_SIZE something fairly big.

    ssize_t nchars = H5Aget_name(head, max_name_len, name);

    // Start a new depth first search starting at the head.

    _path.push(name);

    _current.push(head);
    preorder_action();
    postorder_action();
    _current.pop();

    // We're done with "name".  Pop it off the stack.

    _path.pop();
  }

  _attr_hack = H5I_INVALID_HID;

  // Postconditions:

  // ISSUE
  // The invariant check fails here, apparently because we're in the middle
  // of a traversal.  That means that the invariant isn't written properly.
  //assert(invariant());

  // Exit:
}
