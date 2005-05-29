#include <cassert>
#include "std_iostream.h"
#include <sys/time.h>
#include "timer.h"
#include <unistd.h>

void
resolution()
{
  struct timeval tv1, tv2;

  gettimeofday(&tv1, 0);

  int ct = 0;

  do
  {
    ++ct;
    gettimeofday(&tv2, 0);
  }
  while (tv1.tv_usec == tv2.tv_usec);

  cout << "gettimeofday resolution: "
       << (tv2.tv_usec-tv1.tv_usec)
       << " us after "
       << ct
       << " calls.\n";
}

void
monotonicity(int xct)
{
  struct timeval tv1, tv2;

  int zero = 0;
  int neg  = 0;

  for (int i = 0; i < xct; ++i)
  {
    gettimeofday(&tv1, 0);
    gettimeofday(&tv2, 0);

    if (tv1.tv_usec == tv2.tv_usec)
    {
      ++zero;
    }
    else if (tv1.tv_usec > tv2.tv_usec)
    {
      ++neg;
    }
  }

  cout << "In "
       << xct
       << " back-to-back calls to gettimeofday(), the time increased monotonically "
       << xct-zero-neg
       << " times, did not change "
       << zero
       << " times, and decreased "
       << neg
       << " times.\n"; 
}

int
main()
{
  resolution();
  monotonicity(10000);

  timer t;

  t.start();
  t.stop();

  cout << "Elapsed time after a quick start & stop: "
       << t.elapsed()
       << " seconds.  Should be roughly gettimeofday() resolution.\n";

  t.start();
  sleep(1);
  t.stop();

  cout << "Elapsed time in \"RESET\" mode after a 1 sec sleep: "
       << t.elapsed()
       << " seconds.  Should be about 1 second.\n";

  t.start();
  sleep(1);
  t.stop();

  cout << "Elapsed time after another 1 sec sleep: "
       << t.elapsed()
       << " seconds.  Should be about 1 second.\n";

  cout << "Changing timer mode to \"ACCUMULATE\".\n";

  t.put_mode(timer::ACCUMULATE);

  cout << "Elapsed time after a quick start & stop: "
       << t.elapsed()
       << " seconds.  Should be the same as after previous sleep.\n";

  t.start();
  sleep(1);
  t.stop();

  cout << "Elapsed time in \"ACCUMULATE\" mode after a 1 sec sleep: "
       << t.elapsed()
       << " seconds.  Should be about 2 seconds.\n";

  t.reset();

  cout << "Elapsed time after a reset: "
       << t.elapsed()
       << " seconds.  Should be 0 seconds.\n";

  t.start();
  sleep(1);
  t.stop();

  cout << "Elapsed time in \"ACCUMULATE\" mode after a 1 sec sleep: "
       << t.elapsed()
       << " seconds.  Should be about 1 seconds.\n";

}
