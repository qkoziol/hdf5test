#include "matrix.h"
#include "contract.h"

matrix::
matrix(unsigned xrow, unsigned xcol)
{
  // Preconditions:

  // Body:

  _row_ct = xrow;
  _col_ct = xcol;

  // Postconditions:

  assert(invariant());
  assert(row_ct() == xrow);
  assert(col_ct() == xcol);

  // Exit:
}

matrix::
matrix(const matrix& xother)
{
  // Preconditions:

  // Body:

  _row_ct = xother._row_ct;
  _col_ct = xother._col_ct;

  // Postconditions:

  assert(invariant());
  assert(*this == xother);

  // Exit:
}

matrix::
~matrix()
{
  // Preconditions:

  // Body:

  // Postconditions:

  // Exit:
}

bool
matrix::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  // ISSUE:
  // Do we really want to allow matrices with no columns or
  // no rows?  If not, we need a different invariant.

  result = true;

  // Postconditions:

  // Exit:

  return result;
}

bool
matrix::
operator==(const matrix& xother) const
{
  bool result;

  // Preconditions:

  // Body:

  result = _row_ct == xother._row_ct && _col_ct == xother._col_ct;

  // Postconditions:

  // Exit:

  return result;
}

ostream&
operator<<(ostream& xos, const matrix& xmatrix)
{
  // Preconditions:

  assert(xos.good());

  // Body:

  xos << xmatrix._row_ct
      << ", "
      << xmatrix._col_ct;

  // Postconditions:

  // Exit:

  return xos;
}

unsigned
matrix::
row_ct() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _row_ct;

  // Postconditions:

  // Exit:

  return result;
}

unsigned
matrix::
col_ct() const
{
  unsigned result;

  // Preconditions:

  // Body:

  result = _col_ct;

  // Postconditions:

  // Exit:

  return result;
}

void
matrix::
put_row(unsigned xrow)
{
  // Preconditions:

  // Body:

  _row_ct = xrow;

  // Postconditions:

  assert(invariant());
  assert(row_ct() == xrow);

  // Exit:

}

void
matrix::
put_col(unsigned xcol)
{
  // Preconditions:

  // Body:

  _col_ct = xcol;

  // Postconditions:

  assert(invariant());
  assert(col_ct() == xcol);

  // Exit:

}

void
matrix::
reserve(unsigned xrow, unsigned xcol)
{
  // Preconditions:

  // Body:

  _row_ct = xrow;
  _col_ct = xcol;

  // Postconditions:

  assert(invariant());
  assert(row_ct() == xrow);
  assert(col_ct() == xcol);

  // Exit:

}

void
matrix::
select(hyperslab& xresult) const
{
  // Preconditions:

  assert(xresult.d() == 2);

  // Body:

  xresult.origin()        = 0;
  xresult.stride()        = 1;
  xresult.ct()            = 1;
  xresult.block_size()[0] = _row_ct;
  xresult.block_size()[1] = _col_ct;

  // Postconditions:

  // Exit:
}

void
matrix::
select_row(unsigned xrow, hyperslab& xresult) const
{
  // Preconditions:

  assert(xrow < row_ct());
  assert(xresult.d() == 2);

  // Body:

  xresult.origin()[0]     = xrow;
  xresult.origin()[1]     = 0;
  xresult.stride()        = 1;
  xresult.ct()            = 1;
  xresult.block_size()[0] = 1;
  xresult.block_size()[1] = _col_ct;

  // Postconditions:

  // Exit:
}

void
matrix::
select_rows(unsigned xfirst, unsigned xct, hyperslab& xresult) const
{
  // Preconditions:

  assert(xfirst < row_ct());
  assert(xct > 0);
  assert(xfirst+xct <= row_ct());
  assert(xresult.d() == 2);

  // Body:

  xresult.origin()[0]     = xfirst;
  xresult.origin()[1]     = 0;
  xresult.stride()        = 1;
  xresult.ct()            = 1;
  xresult.block_size()[0] = xct;
  xresult.block_size()[1] = _col_ct;

  // Postconditions:

  // Exit:
}

void
matrix::
select_col(unsigned xcol, hyperslab& xresult) const
{
  // Preconditions:

  assert(xcol < col_ct());
  assert(xresult.d() == 2);

  // Body:

  xresult.origin()[0]     = 0;
  xresult.origin()[1]     = xcol;
  xresult.stride()        = 1;
  xresult.ct()            = 1;
  xresult.block_size()[0] = _row_ct;
  xresult.block_size()[1] = 1;

  // Postconditions:

  // Exit:
}

void
matrix::
select_cols(unsigned xfirst, unsigned xct, hyperslab& xresult) const
{
  // Preconditions:

  assert(xfirst < col_ct());
  assert(xct > 0);
  assert(xfirst+xct <= col_ct());
  assert(xresult.d() == 2);

  // Body:

  xresult.origin()[0]     = 0;
  xresult.origin()[1]     = xfirst;
  xresult.stride()        = 1;
  xresult.ct()            = 1;
  xresult.block_size()[0] = _row_ct;
  xresult.block_size()[1] = xct;

  // Postconditions:

  // Exit:
}

void
matrix::
get_extent(extent& xresult) const
{
  // Preconditions:

  assert(xresult.d() == 2);

  // Body:

  xresult.size()[0] = row_ct();
  xresult.size()[1] = col_ct();
  xresult.max_size()[0] = row_ct();
  xresult.max_size()[1] = col_ct();

  // Postconditions:

  // Exit:
}
