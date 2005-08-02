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
  _start.reserve(xdim);
  _stride.reserve(xdim);
  _ct.reserve(xdim);

  if (xdim > 0)
  {
    _block_size = 1;
    _ct         = 1;
    _start      = 0;
    _stride     = 1;
  }

  // Postconditions:

  assert(invariant());
  assert(d() == xdim);
  assert(d() > 0 ? block_size()[0] == 1 : true);
  assert(d() > 0 ? ct()[0] == 1 : true);
  assert(d() > 0 ? start()[0] == 0 : true);
  assert(d() > 0 ? stride()[0] == 1 : true);

  // Exit:
}

hyperslab::
hyperslab(const tuple& xstart,
	  const tuple& xstride,
	  const tuple& xct, 
	  const tuple& xblock_size)
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

  result = (_block_size.d() == _start.d());
  result = result && (_start.d() == _stride.d());
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

  xos
      << "start: "
      << xhyperslab._start
      << " stride: "
      << xhyperslab._stride
      << " ct: "
      << xhyperslab._ct
      << " block size: "
      << xhyperslab._block_size;

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
start()
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

  return _start;
}

const tuple&
hyperslab::
start() const
{
  // Preconditions:

  // Body:

  // Postconditions:

  assert(invariant());

  // Exit:

  return _start;
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
