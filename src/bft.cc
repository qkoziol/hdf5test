#include "bft.h"
#include "contract.h"

#include "attribute.h"
#include "dataset.h"
#include "datatype.h"
#include "group.h"
#include "hdf5.h"
#include "std_iostream.h"

bft::
bft()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

}

bft::
~bft()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
bft::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = traverser::invariant();
  // TODO
  // What else is true of this class?

  // Postconditions:

  // Exit:

  return result;
}

bft::
bft(const bft& xother)
{
  not_implemented;
}

bft&
bft::
operator=(const bft& xother)
{
  not_implemented;
  return *this;  // keeps the pgi compiler happy until this is implemented.
}

void
bft::
traverse(hid_t xloc, int xfilter)
{
  // Preconditions:

  assert(H5Iget_type(xloc) != H5I_BADID);
  assert(is_node(xloc));

  // Body:

  _filter = xfilter;

  reset();

  _start.self = xloc;

  // Increment reference count to xloc so id recycling
  // in traversal doesn't clobber xloc.

  H5Iinc_ref(_start.self);

  traverse(_start);

  // Cleanup:

  _start.self = H5I_INVALID_HID;

  _marked.erase(_marked.begin(), _marked.end());

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
bft::
traverse(const node& xnode)
{
  // Preconditions:

  assert(H5Iget_type(xnode.self) != H5I_BADID);
  assert(is_node(xnode.self));

  // Body:

  mark(xnode);

  _pending.push(xnode);

  while(!_pending.empty())
  {
    _current = _pending.front();

    if (passes(_current))
    {
      visit_action();
    }

    _pending.pop();

    // If this node is a group, there may be links emanating from it
    // that must be searched.  Investigate this possibility.

    if (H5Iget_type(_current.self) == H5I_GROUP)
    {
      follow_group_links(_current);
    }

    // Explore the attributes, if any, of this node.

    if (H5Iget_type(_current.self) != H5I_ATTR)
    {
      traverse_attrs(_current);
    }

    // Recycle the hids.  This might look like a dangerous thing to do, since
    // other nodes on the queue may also have these same hids.  However, we've
    // incremented the reference count when storing the hids in this node, so
    // this is safe to do.

    if (_current.parent >= 0)
    {
      H5Idec_ref(_current.parent);
    }
    H5Idec_ref(_current.self);
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

const persistent&
bft::
current() const
{
  persistent* ptr_to_result = 0;

  // Preconditions:

  // TODO:
  // This is a pretty ugly assertion.  Find a way to express
  // the same idea more elegantly.

  assert(H5Iget_type(_current.self) == H5I_DATASET || H5Iget_type(_current.self) == H5I_ATTR || H5Iget_type(_current.self) == H5I_DATATYPE || H5Iget_type(_current.self) == H5I_GROUP);

  // Body:

  switch(H5Iget_type(_current.self))
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

  ptr_to_result->attach(_current.self);

  // Postconditions:

  assert(ptr_to_result->is_attached());

  // Exit:

  return *ptr_to_result;
};

hid_t
bft::
current_hid() const
{
  hid_t result;

  // Preconditions:

  // Body:

  result = _current.self;

  // Postconditions:

  // Exit:

  return result;
}

void
bft::
follow_group_links(const node& xnode)
{
  // Preconditions:

  assert(H5Iget_type(xnode.self) == H5I_GROUP);

  // Body:

  hsize_t num_objs;

  herr_t err = H5Gget_num_objs(xnode.self, &num_objs);

  assert(err >= 0);

  // Loop through all the links.  If the head of the link is a node,
  // put the head on the queue of nodes to be visited.

  for (hsize_t link = 0; link < num_objs; ++link)
  {
    int head_type = H5Gget_objtype_by_idx(_current.self, link);

    // Get the name of the head, first ensuring that we have a big enough
    // buffer to hold the name.

    ssize_t nchars = H5Gget_objname_by_idx(_current.self, link, 0, 0);

    _name.reserve(nchars+1);

    H5Gget_objname_by_idx(_current.self, link, &_name[0], nchars+1);

    node head;

    switch (head_type)
    {
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

      case H5G_LINK:
	// a soft link
	break;
    }

    if (head.self >= 0)
    {
      // then head is a dataset, datatype, or group, i.e., a graph node

      head.parent = xnode.self;

      H5Iinc_ref(head.parent);

      head.index  = link;

      // Put head on pending queue of nodes to visit if it hasn't already
      // been placed on the queue.

      if (! is_marked(head))
      {
	mark(head);
	_pending.push(head);
      }
      else
      {
	// Head's been processed before; recycle resources.

	H5Idec_ref(head.parent);

	H5Idec_ref(head.self);
      }
    }
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

void
bft::
traverse_attrs(const node& xnode)
{
  // Preconditions:

  assert(H5Iget_type(xnode.self) == H5I_GROUP || H5Iget_type(xnode.self) == H5I_DATASET || H5Iget_type(xnode.self) == H5I_DATATYPE);

  // Body:

  int num_objs = H5Aget_num_attrs(xnode.self);

  // Put the attributes on the queue of objects to be visited.

  for (int link = 0; link < num_objs; ++link)
  {
    node head;

    head.self   = H5Aopen_idx(xnode.self, link);
    head.parent = xnode.self;
    head.index  = link;

    H5Iinc_ref(head.parent);

    mark(head);

    _pending.push(head);
  }

  // Postconditions:

  assert(invariant());

  // Exit:
}

const char*
bft::
name(bool xpath)
{
  char* result;

  // Preconditions:

  // Body:

  ssize_t size;
  ssize_t used;

  if (xpath)
  {
    // First get the parent's pathname.

    size = H5Iget_name(_current.parent, 0, 0);

    _name.reserve(size+2);

    H5Iget_name(_current.parent, &_name[0], size+1);

    _name[size] = '/';

    used = size+1;
  }
  else
  {
    used = 0;
  }

  // Then get the "local" name of the container.

  H5I_type_t type = H5Iget_type(_current.self);

  switch(type)
  {

    case H5I_ATTR:

      size = H5Aget_name(_current.self,
			 0,
			 0);

      _name.reserve(size+used+1);

      H5Aget_name(_current.self,
		  size+1,
		  &_name[used]);
      break;

    default:

      size = H5Gget_objname_by_idx(_current.parent,
				   _current.index,
				   0,
				   0);

      _name.reserve(size+used+1);

      H5Gget_objname_by_idx(_current.parent,
			    _current.index,
			    &_name[used],
			    size+1);
      break;
  }

  result = &_name[0];


  // Postconditions:

  assert(result != 0);

  // Exit:

  return result;
}
