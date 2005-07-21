/*
  Tests basic io operations on various
  kinds of containers.
*/

#include "config.h"
#include "contract.h"
#include "dataset.h"
#include "group.h"
#include "hdf5_file.h"
#include "io_perf.h"
#include "memory.h"
#include "scalar.h"
#include "tuple.h"

string&
test_name()
{
  // Strips off all characters starting with the last . in
  // the name of this file and returns the resulting string.
  // Useful for generating an HDF5 file name specific to this
  // test driver.

  string* result = new string(__FILE__);
  result->erase(result->find_last_of("."), result->size());
  return *result;
}

void
do_read_write_tests(io_perf& xio_t, pcontainer& xcon)
{
  // Make a memory buffer for i/o operations.

  memory m;

  // Allocate sufficient memory for xcon's
  // current selection and datatype.

  m.reserve(xcon);

  // Run a write test and report results.
  // ISSUE:
  // Memory hasn't been initialized.  What's being
  // written?
  // NOTE
  // H5Dfill() is probably what we want to do this.

  xio_t.run_test(m, xcon);

  if (xio_t.status() == test::SUCCESS)
  {
    double mb      = xio_t.bytes()/((double)(BYTES_PER_KB*BYTES_PER_KB));
    double elapsed = xio_t.elapsed();

    cout << "\tWrite test succeeded.  "
	 << mb
	 << " mb transferred in "
	 << elapsed
	 << " seconds = "
	 << mb/elapsed
	 << " mb/s.\n";
  }
  else
  {
    cout << "\tWrite test failed.\n";
  }

  // Read back what we just wrote and report
  // performance.

  xio_t.run_test(xcon, m);

  if (xio_t.status() == test::SUCCESS)
  {
    double mb      = xio_t.bytes()/((double)(BYTES_PER_KB*BYTES_PER_KB));
    double elapsed = xio_t.elapsed();

    cout << "\tRead test succeeded.  "
	 << mb
	 << " mb transferred in "
	 << elapsed
	 << " seconds = "
	 << mb/elapsed
	 << " mb/s.\n";
  }
  else
  {
    cout << "\tRead test failed.\n";
  }
}

void
make_test_objects()
{
  hdf5_file f;

  assert(f.create(test_name() + ".h5"));

  group root;

  root.open(f.hid());

  // Create a contiguous dataset in f.

  {
    dataset cg;

    scalar s;

    assert(cg.create(f, root.hid(), "contiguous", H5T_NATIVE_INT, s.hid()));

    cg.detach();
  }

  // Create a compact dataset in f.

  {
    dataset c;

    scalar s;

    hid_t plist = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_layout(plist, H5D_COMPACT);

    assert(c.create(f, root.hid(), "compact", H5T_NATIVE_INT, s.hid(), plist));

    H5Pclose(plist);

    c.detach();
  }

  // Create an external dataset "in" f.

  {
    dataset e;

    scalar s;

    hid_t plist = H5Pcreate(H5P_DATASET_CREATE);

    string name = test_name() + "_external.h5";
    assert(H5Pset_external(plist, name.c_str(), 0, 4096) >= 0);

    assert(e.create(f, root.hid(), "external", H5T_NATIVE_INT, s.hid(), plist));

    H5Pclose(plist);

    e.detach();
  }

  root.detach();
  f.detach();
}

int
main()
{

  // Make the HDF5 file, datasets, and attributes necessary
  // for the remainder of this program.

  make_test_objects();

  io_perf    io_t; // A basic io tester.
  hdf5_file  f;    // An hdf5 file.

  // Open the file with write access.

  f.open(test_name() + ".h5", hdf5_file::RDWR);

  // Test a contiguous dataset.

  {
    // Open a contiguous dataset.  We have write
    // access to cg because we have write access to f.

    dataset cg;

    cg.open(f.hid(), "contiguous"); 

    // Test contiguous io.

    cout << "Contiguous dataset test: "
         << cg
         << endl;

    do_read_write_tests(io_t, cg);

    cg.detach();
  }

  // Test a chunked dataset.

  {
    // Make a tuple to describe chunk size.

    tuple chunk_size(3);

    // Be careful not to specify a huge chunk size, since we'll have to allocate
    // a memory buffer of similar size to do i/o.  A 512*512*512 chunk size may
    // be moderate on disk, but that's 134,217,728 elements.  At 4 bytes apiece
    // for H5T_NATIVE_INT on 32 bit linux, that's 536,870,912 bytes - more
    // memory than my laptop has even with swap.

    chunk_size = 4;

    // Create a dataset in f with chunk size "chunk_size".

    dataset ch;

    ch.create(f, "chunked", H5T_NATIVE_INT, chunk_size);
    assert(ch.is_attached());

    // Test chunked io.

    cout << "Chunked dataset test: "
         << ch
         << endl;

    do_read_write_tests(io_t, ch);

    ch.detach();
  }

  // Test a compact dataset.

  {
    // Open a compact dataset to read it.

    dataset c;

    assert(c.open(f.hid(), "compact")); 

    // Test compact io.

    cout << "Compact dataset test: "
         << c
         << endl;

    do_read_write_tests(io_t, c);

    c.detach();
  }

  // Test an external dataset.

  {
    // Open an external dataset to read it.

    dataset e;

    assert(e.open(f.hid(), "external")); 

    // Test external io.

    cout << "External dataset test: "
         << e
         << endl;

    do_read_write_tests(io_t, e);

    e.detach();
  }

  // Delete hdf5 file.

  f.rm();

  // And delete external dataset file.

  string name = test_name() + "_external.h5";

  unlink(name.c_str());
}
