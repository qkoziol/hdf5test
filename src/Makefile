# parallel:
#HDF5DIR=/usr/local/hdf5/parallel
#CC=$(HDF5DIR)/bin/h5pcc
# serial:
HDF5DIR=/usr/local/hdf5/serial
CC=$(HDF5DIR)/bin/h5cc
CXX=$(HDF5DIR)/bin/h5c++
#
# Note that h5cc and h5c++ will append CFLAGS and CXXFLAGS
# to the options they already impose.  In particular, h5cc
# and h5c++ define -O2 by default, which makes using a
# debugger confusing.  So, you may have to edit h5cc and
# h5c++ to get what you want.
#
CXXFLAGS=-g -pipe
CFLAGS=$(CXXFLAGS)

lib_sources =	attribute.cc \
		col_itr.cc \
		container.cc \
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
		itr.cc \
		matrix.cc \
		memory.cc \
		path.cc \
		persistent.cc \
		property_list.cc \
		scalar.cc \
		slice_itr.cc \
		test.cc \
		timer.cc \
		transfer.cc \
		tuple.cc

test_sources =	matrix_t.cc \
		test_access_patterns.cc \
		test_basic_io_perf.cc \
		test_dft_io_perf.cc \
		test_dft_ls.cc \
		test_iterated_io.cc \
		test_path.cc

OBJ = $(lib_sources:.cc=.o)
BIN = $(test_sources:.cc=)

%.d: %.cc
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o : %.cc
	$(CXX) -c $(CXXFLAGS) -prefix=$(HDF5DIR) $< -o $@

% : %.cc
	$(CXX) -o $@ $(CXXFLAGS) -prefix=$(HDF5DIR) $< -g $(OBJ)

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

all: bin doc test

$(BIN): libperf.a

libperf.a: $(OBJ)
	ar r libperf.a $(OBJ)

ifneq ($(MAKECMDGOALS),clean)
include $(lib_sources:.cc=.d)
include $(test_sources:.cc=.d)
endif

clean:
	rm -f *~ *.d *.o $(BIN) *.hdf *.h5 *.a \#* core .\#*
	rm -rf ../doc
