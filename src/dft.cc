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

  // Postconditions:

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

  result = traverser::invariant();
  result = result && (_current.empty() ? true : is_node(_current.top().self));

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
  return *this;
}

void
dft::
traverse(hid_t xloc, int xfilter)
{
  // Preconditions:

  assert(H5Iget_type(xloc) != H5I_BADID);
  assert(is_node(xloc));

  // Body:

  _start.self = xloc;

  reset();

  traverse(_start, xfilter);

  _start.self = H5I_INVALID_HID;

  _marked.erase(_marked.begin(), _marked.end());

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
dft::
traverse(const node& xnode, int xfilter)
{
  // Preconditions:

  assert(H5Iget_type(xnode.self) != H5I_BADID);
  assert(is_node(xnode.self));

  // Body:

  // If we've already visited this node, do nothing except return.
  // Otherwise, do pre- and post-vist actions, and, if possible,
  // explore the graph accessible from links emanating from this node.

  if (! is_marked(xnode))
  {
    // This is the first time we've encountered this node.
    // Mark it as visited and do the preorder operation on it.

    _current.push(xnode);

    mark(xnode);

    if (passes(xnode, xfilter))
    {
      preorder_action();
    }

    // If this node is a group, there may be links emanating from it
    // that must be searched.  Investigate this possibility.

    if (H5Iget_type(xnode.self) == H5I_GROUP)
    {
      follow_group_links(xnode, xfilter);
    }

    // Explore the attributes, if any, of this node.

    traverse_attrs(xnode, xfilter);

    // We encounter this node for the last time; do postorder operation here.

    if (passes(xnode, xfilter))
    {
      postorder_action();
    }

    // We won't see xnode again, because it's been visited.  Pop it
    // off the stack so that "dft" forgets it, and decrement its
    // HDF5 reference count.  Be careful not to decrement the count
    // of _start, since that was supplied externally.

    assert(xnode == _current.top());

    if (xnode != _start)
    {
      H5Idec_ref(xnode.self);
    }

    _current.pop();
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}


persistent&
dft::
current() const
{
  persistent* ptr_to_result = 0;

  // Preconditions:

  // TODO:
  // This is a pretty ugly assertion.  Find a way to express
  // the same idea more elegantly.
  assert(H5Iget_type(_current.top().self) == H5I_DATASET || H5Iget_type(_current.top().self) == H5I_ATTR || H5Iget_type(_current.top().self) == H5I_DATATYPE || H5Iget_type(_current.top().self) == H5I_GROUP);

  // Body:

  hid_t id = _current.top().self;

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
    default:
      bool is_a_node = false;
      assert(is_a_node);
      break;
  }

  ptr_to_result->attach(id);

  // Postconditions:

  assert(ptr_to_result->is_attached());

  // Exit:

  return *ptr_to_result;
};


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

hid_t
dft::
current_hid() const
{
  hid_t result;

  // Preconditions:

  // Body:

  result = _current.top().self;

  // Postconditions:

  // Exit:

  return result;
}

void
dft::
follow_group_links(const node& xnode, int xfilter)
{
  // Preconditions:

  assert(H5Iget_type(xnode.self) == H5I_GROUP);

  // Body:

  hsize_t num_objs;

  herr_t err = H5Gget_num_objs(xnode.self, &num_objs);

  assert(err >= 0);

  // Loop through all the links.  If the head of the link is a node
  // start another depth first traversal from that point.

  for (hsize_t link = 0; link < num_objs; ++link)
  {
    int head_type = H5Gget_objtype_by_idx(xnode.self, link);

    // Get the name of the head, first ensuring that we have a big enough
    // buffer to hold the name.

    ssize_t nchars = H5Gget_objname_by_idx(xnode.self, link, 0, 0);

    _name.reserve(nchars+1);

    H5Gget_objname_by_idx(xnode.self, link, &_name[0], nchars+1);

    node head;

    switch (head_type)
    {
      case H5G_LINK:
	break;

      case H5G_GROUP:
	head.self = H5Gopen(xnode.self, &_name[0]);
	assert(head.self >= 0);
	break;

      case H5G_DATASET:
	head.self = H5Dopen(xnode.self, &_name[0]);
	assert(head.self >= 0);
	break;

      case H5G_TYPE:
	head.self = H5Topen(xnode.self, &_name[0]);
	assert(head.self >= 0);
	break;
    }

    head.parent = xnode.self;
    head.index = link;

    // We may have found a new node at the head of a link emanating from xnode.
    // Start a new depth first search starting at this new node.

    if (is_node(head.self))
    {
      // It's unusual to check the class invariant in the middle of a function,
      // but we're leaving the function (temporarily) and we always check
      // the invariant at function exits.

      assert(invariant());

      traverse(head, xfilter);
    }
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
dft::
traverse_attrs(const node& xnode, int xfilter)
{
  // Preconditions:

  assert(H5Iget_type(xnode.self) == H5I_GROUP || H5Iget_type(xnode.self) == H5I_DATASET || H5Iget_type(xnode.self) == H5I_DATATYPE);

  // Body:

  int num_objs = H5Aget_num_attrs(xnode.self);

  // Visit the attributes one by one.

  for (int link = 0; link < num_objs; ++link)
  {

    node head;

    head.self   = H5Aopen_idx(xnode.self, link);
    head.parent = xnode.self;
    head.index  = link;

    if (passes(head, xfilter))
    {
      // Start a new depth first search starting at the head.

      _current.push(head);

      preorder_action();

      postorder_action();

      _current.pop();
    }

    H5Idec_ref(head.self);
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

const char*
dft::
name(bool xpath)
{
  const char* result;

  // Preconditions:

  // Body:

  result = traverser::name(_current.top(), xpath);

  // Postconditions:

  assert(result != 0);

  // Exit:

  return result;
}
