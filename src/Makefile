# Note on editing this file:
#
# You should provide a definition for HDFDIR, a
# directory containing the bin/, doc/, include/,
# and lib/ directories into which your version
# of HDF5 is installed.  I've provided a number
# of such definitions for various NCSA machines
# and compilers, although most of them are my own
# personal builds of hdf5 and not available to
# others.  Make sure that there's only one definition
# of HDF5DIR!
#
# This makefile uses the hdfc++ compiler wrapper
# $(HDF5DIR)/bin/h5c++.  You can append compiler
# options to those contained in h5c++ by defining
# CXXFLAGS.  There is a mandatory CXXFLAG option for
# aix's xlC compiler.  See the example for "copper"
# below.
#
# Note on compiler options:
#
# h5cc and h5c++ will append CFLAGS and CXXFLAGS
# to the options they already impose.  So you may
# have to edit h5cc and h5c++ to delete options
# that you may not want.  For example, I delete
# the -O2 in h5c++ on my home machine because it's
# way too confusing trying to follow optimized code
# with gdb.
#
# On lotus (my x86 linux development machine at home - John Blinka)
#
# parallel, gcc-3.3.5, mpich 1.2.5.2:
#
#HDF5DIR=/usr/local/hdf5/parallel
#CC=$(HDF5DIR)/bin/h5pcc
#
# On lotus (my x86 linux development machine at home - John Blinka)
#
# serial, gcc-3.3.5:
#
HDF5DIR=/usr/local/hdf5/serial
# the following flags are optional:
CXXFLAGS=-g -pipe -Wall
#
# On copper, aix, xlC
#
#HDF5DIR=/u/ncsa/jblinka
# the following flag is mandatory (or can substitute rtti=all)
#CXXFLAGS=-q rtti=dyna
#
# On shanti, solaris,
#
#HDF5DIR=/mnt/sdt/jblinka/shanti/hdf5-1.6.4-SUNWspro
#
# On mir, 64 bit amd linux, gcc-3.3.5
#
#HDF5DIR=/mnt/sdt/jblinka/mir/hdf5-1.6.4-gcc-3.3.5
# the following flags are optional:
#CXXFLAGS=-g -pipe -Wall
#
# On mir, 64 bit amd linux, pgi compiler
#
# make sure you don't inadvertently use the compiler
# option instantiate=local.  It might be hidden in
# h5c++.
#HDF5DIR=/mnt/sdt/jblinka/mir/hdf5-1.6.4-pgi
#
# On sleipnir, x86 FreeBSD, gcc-3.3.5:
#
#HDF5DIR=/mnt/sdt/jblinka/sleipnir/hdf5-1.6.4-gcc-3.3.5
# the following flags are optional:
#CXXFLAGS=-g -pipe -Wall
#
# On heping, smp x86 linux, pgi compiler
#
# make sure you don't inadvertently use the compiler
# option instantiate=local.  It might be hidden in
# h5c++.
#HDF5DIR=/afs/ncsa/packages/hdf5/5-1.6.4-linux-pgi
#
# On heping, smp x86 linux, gcc-3.3.5:
#
#HDF5DIR=/mnt/sdt/jblinka/heping/hdf5-1.6.4-gcc-3.3.5
# the following flags are optional:
#CXXFLAGS=-g -pipe -Wall
#
CC=$(HDF5DIR)/bin/h5cc
CXX=$(HDF5DIR)/bin/h5c++

lib_sources =	attribute.cc \
		bft.cc \
		bft_ls.cc \
		contract.cc \
		dataset.cc \
		dataspace.cc \
		datatype.cc \
		dft.cc \
		dft_io_perf.cc \
		dft_ls.cc \
		extent.cc \
		group.cc \
		hdf5_file.cc \
		hdf5_handle.cc \
		hyperslab.cc \
		io_perf.cc \
		matrix.cc \
		matrix_writer.cc \
		memory.cc \
		node.cc \
		objno.cc \
		partial.cc \
		pcontainer.cc \
		persistent.cc \
		property_list.cc \
		scalar.cc \
		temp_file.cc \
		test.cc \
		timer.cc \
		transfer.cc \
		traverser.cc \
		tuple.cc

test_sources =	test_access_patterns.cc \
		test_attr.cc \
		test_basic_io_perf.cc \
		test_bft_ls.cc \
		test_dataspace.cc \
		test_dft_io_perf.cc \
		test_dft_ls.cc \
		test_iterated_io.cc \
		test_matrix.cc \
		test_matrix_writer.cc \
		test_timer.cc

OBJ = $(lib_sources:.cc=.o)
BIN = $(test_sources:.cc=)

# The following doesn't work on copper.
%.d: %.cc
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o : %.cc
	$(CXX) -c $(CXXFLAGS) -prefix=$(HDF5DIR) $< -o $@

% : %.cc
	$(CXX) -o $@ $(CXXFLAGS) -prefix=$(HDF5DIR) $< -g $(OBJ)

obj: $(OBJ)

bin: $(BIN)

test: $(BIN)
	for i in $(BIN); \
	do \
		echo ""; \
		echo "Running test $$i..."; \
		echo ""; \
		./$$i; \
	done

doc:
	doxygen Doxyfile

all: obj bin doc test

$(BIN): libperf.a

libperf.a: $(OBJ)
	ar r libperf.a $(OBJ)

#
# ISSUE:
# The dependencies are useful in development to ensure that necessary files
# are recompiled whenever changes are made.  They're probably not useful
# in "production" and they present porting problems.  Aix's xlC, for example,
# behaves somewhat differently from gcc wrt -M, and the %.d: %.cc rule
# produces all kinds of errors on aix.  So, I only generate the dependencies
# on my development machine, lotus.
#
ifeq ($(HOSTNAME),lotus)
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),doc)
include $(lib_sources:.cc=.d)
include $(test_sources:.cc=.d)
endif
endif
endif

clean:
	rm -f *~ *.d *.o $(BIN) *.hdf *.h5 *.a \#* core.* .\#* *.u *.s
	rm -rf ../doc/html ../doc/latex
