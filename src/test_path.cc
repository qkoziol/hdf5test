#include "path.h"

int
main()
{
  char* comps[4] =
  { "group",
    "supercalifragilisticexpialidocious",
    "matrix",
    "basketball"
  };

  path p;

  cout << "beginning path = `"
       << p.whole()
       << "'. ct = "
       << p.ct()
       << " top_ct = "
       << p.top_ct()
       << " ub = "
       << p.ub()
       << " depth = "
       << p.depth()
       << endl;

  for (int i = 0; i < 4; ++i)
  {
    p.push(comps[i]);

    cout << "after a push path = `"
	 << p.whole()
	 << "'. ct = "
	 << p.ct()
	 << " top component = `"
	 << p.top()
	 << "' top_ct = "
	 << p.top_ct()
	 << " ub = "
	 << p.ub()
	 << " depth = "
	 << p.depth()
	 << endl;
  }

  while(p.depth() > 1)
  {
    p.pop();

    cout << "after a pop path = `"
	 << p.whole()
	 << "'. ct = "
	 << p.ct()
	 << " top component = `"
	 << p.top()
	 << "' top_ct = "
	 << p.top_ct()
	 << " ub = "
	 << p.ub()
	 << " depth = "
	 << p.depth()
	 << endl;
  }
}
