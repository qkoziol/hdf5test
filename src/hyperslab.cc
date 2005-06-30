#include "hyperslab.h"
#include "contract.h"

hyperslab::
hyperslab()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());
  assert(d() == 0);

  // Exit:
}

hyperslab::
hyperslab(unsigned xdim)
{
  // Preconditions:

  // Body:

  _block_size.reserve(xdim);
  _origin.reserve(xdim);
  _stride.reserve(xdim);
  _ct.reserve(xdim);

  if (xdim > 0)
  {
    _block_size[0] = 1;
    _ct[0]         = 1;
    _origin[0]     = 0;
    _stride[0]     = 1;
  }

  // Postconditions:

  assert(invariant());
  assert(d() == xdim);
  assert(d() > 0 ? block_size()[0] == 1 : true);
  assert(d() > 0 ? ct()[0] == 1 : true);
  assert(d() > 0 ? origin()[0] == 0 : true);
  assert(d() > 0 ? stride()[0] == 1 : true);

  // Exit:
}

hyperslab::
hyperslab(const tuple& xblock_size,
	  const tuple& xorigin,
	  const tuple& xstride,
	  const tuple& xct)
{
  not_implemented;
}

hyperslab::
hyperslab(const extent& xextent)
{
  not_implemented;
}

hyperslab::
~hyperslab()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

unsigned
hyperslab::
d() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _block_size.d();

  // Postconditions:

  // Exit:

  return result;
}

bool
hyperslab::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = (_block_size.d() == _origin.d());
  result = result && (_origin.d() == _stride.d());
  result = result && (_stride.d() == _ct.d());

  // Postconditions:

  // Exit:

  return result;
}

ostream&
operator<<(ostream& xos, const hyperslab& xhyperslab)
{
  // Preconditions:

  assert(xos.good());

  // Body:

  xos << "block size: "
      << xhyperslab._block_size
      << " origin: "
      << xhyperslab._origin
      << " stride: "
      << xhyperslab._stride
      << " ct: "
      << xhyperslab._ct;

  // Postconditions:

  // Exit:

  return xos;
}

const tuple&
hyperslab::
block_size() const
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

  return _block_size;
}

tuple&
hyperslab::
block_size()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

  return _block_size;
}

tuple&
hyperslab::
origin()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

  return _origin;
}

const tuple&
hyperslab::
origin() const
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

  return _origin;
}

tuple&
hyperslab::
ct()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

  return _ct;
}

const tuple&
hyperslab::
ct() const
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

  return _ct;
}

tuple&
hyperslab::
stride()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

  return _stride;
}

const tuple&
hyperslab::
stride() const
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

  return _stride;
}

hsize_t
hyperslab::
npoints() const
{
  hsize_t result;

  // Preconditions:

  // Body:

  result = 1;

  int dim = d();

  for (int i = 0; i < dim; ++i)
  {
    // ISSUE:
    // What prevents overflow in this computation?

    result *= _ct[i]*_block_size[i];
  }

  // Postconditions:

  // Exit:

  return result;
}
