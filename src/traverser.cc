#include "traverser.h"
#include "contract.h"

#include "attribute.h"
#include "dataset.h"
#include "datatype.h"
#include "group.h"
#include "hdf5.h"
#include "std_iostream.h"

traverser::
traverser()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

}

traverser::
~traverser()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
traverser::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = (_start.self < 0 ? _marked.size() == 0 : true);

  // Postconditions:

  // Exit:

  return result;
}

traverser::
traverser(const traverser& xother)
{
  not_implemented;
}

traverser&
traverser::
operator=(const traverser& xother)
{
  not_implemented;
  return *this;  // keeps pgi compiler happy until this is implemented.
}


bool
traverser::
is_node(hid_t xnode) const
{
  bool result;

  // Preconditions:

  // Body:

  H5I_type_t type = H5Iget_type(xnode);

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
traverser::
mark(const node& xnode)
{
  // Preconditions:

  assert(H5Iget_type(xnode.self) == H5I_DATASET || H5Iget_type(xnode.self) == H5I_ATTR || H5Iget_type(xnode.self) == H5I_DATATYPE || H5Iget_type(xnode.self) == H5I_GROUP);

  // Body:

  objno unique(xnode.self);

  _marked.insert(unique);

  // Postconditions:

  assert(is_marked(xnode));

  // Exit:
}

bool
traverser::
is_marked(const node& xnode) const
{
  bool result;

  // Preconditions:

  assert(H5Iget_type(xnode.self) == H5I_DATASET || H5Iget_type(xnode.self) == H5I_ATTR || H5Iget_type(xnode.self) == H5I_DATATYPE || H5Iget_type(xnode.self) == H5I_GROUP);

  // Body:

  objno unique_id(xnode.self);

  if (_marked.find(unique_id) == _marked.end())
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
traverser::
traverse(hid_t xloc, int xfilter)
{
  // Preconditions:

  assert(H5Iget_type(xloc) != H5I_BADID);
  assert(is_node(xloc));

  // Body:

  reset();

  _start.self = xloc;

  traverse(_start, xfilter);

  _start.self = H5I_INVALID_HID;

  _marked.erase(_marked.begin(), _marked.end());

  // Postconditions:

  assert(invariant());

  // Exit:
}


const persistent&
traverser::
obj(hid_t xhid) const
{
  persistent* ptr_to_result;

  // Preconditions:

  assert(H5Iget_type(xhid) == H5I_DATASET || H5Iget_type(xhid) == H5I_ATTR || H5Iget_type(xhid) == H5I_DATATYPE || H5Iget_type(xhid) == H5I_GROUP);

  // Body:

  switch(H5Iget_type(xhid))
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
      ptr_to_result = 0;
      bool is_a_node = false;
      assert(is_a_node);
      break;
  }

  ptr_to_result->attach(xhid);

  // Postconditions:

  assert(ptr_to_result->is_attached());

  // Exit:

  return *ptr_to_result;
};

const char*
traverser::
name(const node& xnode, bool xpath)
{
  char* result;

  // Preconditions:

  // Body:

  ssize_t size;  // size of parent's pathname, not including trailing '\0'.
  ssize_t used;  // number of chars used to represent parent's pathname plus
                 // trailing '/' and '\0'.  Note that used is not necessarily
                 // size+2, since the parent's pathname might be just '/' and
                 // we don't append an extra '/' to that.

  if (xpath)
  {
    // First get the parent's pathname.

    if (xnode.parent >= 0)
    {
      size = H5Iget_name(xnode.parent, 0, 0);

      _name.reserve(size+2);

      H5Iget_name(xnode.parent, &_name[0], size+1);
      /*
      cout << "inside traverser::name, size = "
	   << size
	   << " name is `"
	   << &_name[0]
	   << "'.  Gonna add a / to it."
	   << endl;
      */
      if (_name[size-1] == '/')
      {
	used = size;
      }
      else
      {
	_name[size] = '/';
	used = size+1;
      }
    }
    else
    {
      used = 0;
    }
  }
  else
  {
    used = 0;
  }

  // Then get the "local" name of the container.

  H5I_type_t type = H5Iget_type(xnode.self);

  switch(type)
  {

    case H5I_ATTR:

      size = H5Aget_name(xnode.self, 0, 0);

      _name.reserve(size+used+1);

      H5Aget_name(xnode.self, size+1, &_name[used]);
      break;

    default:

      size = H5Gget_objname_by_idx(xnode.parent, xnode.index, 0, 0);

      _name.reserve(size+used+1);

      H5Gget_objname_by_idx(xnode.parent, xnode.index, &_name[used], size+1);
      break;
  }

  result = &_name[0];


  // Postconditions:

  assert(result != 0);

  // Exit:

  return result;
}

void
traverser::
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

bool
traverser::
passes(const node& xnode, int xfilter) const
{
  bool result;

  // Preconditions:

  // Body:

  H5I_type_t type = H5Iget_type(xnode.self);

  switch (type)
  {
    case H5I_DATASET:
      result =!(xfilter & DATASET);
      break;
    case H5I_DATATYPE:
      result = !(xfilter & DATATYPE);
      break;
    case H5I_ATTR:
      result = !(xfilter & ATTRIBUTE);
      break;
    default:
      result = false;
      break;
  }

  // Postconditions:

  // Exit:

  return result;
}
