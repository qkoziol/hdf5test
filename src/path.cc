#include "path.h"
#include "contract.h"

path::
path()
{
  // Preconditions:

  // Body:

  _buf = new char[MAX_COMPONENT_SIZE];
  _buf[0] = '.';
  _buf[1] = '\0';
  _ub = MAX_COMPONENT_SIZE;
  _ct = 2;
  _depth = 1;

  // Postconditions:

  assert(invariant());
  assert(ct() == 2);
  assert(top_ct() == 2);
  assert(ub() == MAX_COMPONENT_SIZE);
  assert(depth() == 1);

  // Exit:
}

path::
~path()
{
  // Preconditions:

  // Body:

  delete [] _buf;

  // Postconditions:

  // Exit:
}

bool
path::
invariant() const
{
  bool result;

  // Preconditions:

  // Body:

  result = (_buf != 0);
  result = result && (_ub >= MAX_COMPONENT_SIZE);
  result = result && (_ct <= _ub);
  result = result && (MAX_COMPONENT_SIZE > 1);
  result = result && (_buf[_ct-1] == '\0');
  result = result && (_buf[0] == '.');
  result = result && (depth() > 0);

  // Postconditions:

  // Exit:

  return result;
}

path::
path(const path& xother)
{
  not_implemented;
}

path&
path::
operator=(const path& xother)
{
  not_implemented;
}

void
path::
push(const char* xcomponent)
{
  int old_depth = depth();
  size_t old_ub = ub();
  size_t old_ct = ct();

  // Preconditions:

  assert(xcomponent != 0);

  // Body:

  const char* src = xcomponent;

  // Search through xcomponent until the end or a '\0',
  // whichever comes first.

  for (; src-xcomponent < MAX_COMPONENT_SIZE && *src != '\0'; ++src)
    ;

  // We'll also append a separator between the existing pathname
  // and this component: a '/', hence the +1.

  size_t append_ct = src-xcomponent+1;
  size_t new_ct    = _ct+append_ct;

  // Increase the buffer size if necessary.

  if (new_ct > _ub)
  {
    int n = new_ct/_ub;

    if (new_ct%_ub > 0)
      ++n;

    _ub *= (n+1);

    char* newbuf = new char[_ub];

    memcpy(newbuf, _buf, _ct);

    delete [] _buf;

    _buf = newbuf;
  }

  // Add the component separator '/' and whatever is required
  // of xcomponent to the current path, terminating with a '/0'.

  src = xcomponent;
  char* dest = _buf+_ct-1;

  *dest++ = '/';

  for (size_t i = 1; i < append_ct; ++i, ++src, ++dest)
    *dest = *src;

  *dest = '\0';

  _ct += append_ct;

  ++_depth;

  // Postconditions:

  assert(invariant());
  assert(depth() == old_depth+1);
  assert(ub() >= old_ub);
  assert(ct() >= old_ct);

  // Exit:
}

void
path::
pop()
{
  int old_depth = depth();
  size_t old_ct = ct();
  size_t old_ub = ub();

  // Preconditions:

  assert(depth() > 1);

  // Body:

  char* c = const_cast<char*>(top());
  size_t ltop_ct = top_ct();

  *(c-1) = '\0';

  --_depth;
  _ct -= ltop_ct;

  // Postconditions:

  assert(invariant());
  assert(depth() == old_depth-1);
  assert(ct() < old_ct);
  assert(ub() == old_ub);

  // Exit:
}

const char*
path::
top() const
{
  const char* result;

  // Preconditions:

  // Body:

  // c points to last used char in _buf.

  const char* c = _buf+_ct-1;

  // Search backwards until c points to last '/'
  // in _buf. or beginning of buf.

  for (; *c != '/' && c != _buf; --c)
    ;
  if (c != _buf)
    result = c+1;
  else
    result = _buf;

  // Postconditions:

  assert(result != 0);

  // Exit:

  return result;
}

const char*
path::
whole() const
{
  const char* result;

  // Preconditions:

  // Body:

  result = _buf;

  // Postconditions:

  assert(result != 0);

  // Exit:

  return result;
}

ostream&
operator<<(ostream& xos, const path& xpath)
{
  not_implemented;
}

size_t
path::
ct() const
{
  size_t result;

  // Preconditions:

  // Body:

  result = _ct;

  // Postconditions:

  assert(result <= ub());

  // Exit:

  return result;
}

size_t
path::
ub() const
{
  size_t result;

  // Preconditions:

  // Body:

  result = _ub;

  // Postconditions:

  assert(result >= MAX_COMPONENT_SIZE);

  // Exit:

  return result;
}

size_t
path::
top_ct() const
{
  size_t result;

  // Preconditions:

  // Body:

  // c points to last used char in _buf.

  const char* c = _buf+_ct-1;

  // Search backwards until c points to last '/'
  // in _buf or beginning of _buf, whichever comes
  // first.

  for (; c-_buf >= 0; --c)
    if (*c == '/')
      break;

  // c-_buf+1 is the number of chars used to represent
  // the pathname without the last component and its
  // trailing '\0'.
  // _ct is the number of chars used to represent the
  // whole pathname.

  result = _ct-(c-_buf+1);

  // Postconditions:

  assert(result <= ub());

  // Exit:

  return result;
}

int
path::
depth() const
{
  int result;

  // Preconditions:

  // Body:

  result = _depth;

  // Postconditions:

  assert(result > 0);

  // Exit:

  return result;
}
