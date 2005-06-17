#ifndef PLIST_H
#define PLIST_H

/*! @class plist
    @brief A handle to an HDF5 property list.

  This is the property_list hierarchy that appears in
  Elena's web pages on the hdf5 abstract data model.
  The HDF5 C API reference page for the H5P interface
  collects all the property manipulation functions
  under these headings:
              General Property List Operations
                    * H5Pcreate
		    * H5Pget_class
		    * H5Pcopy
		    * H5Pclose 
	      Generic Properties
                    * H5Pcreate_class
		    * H5Pcreate_list
		    * H5Pregister
		    * H5Pinsert
		    * H5Pset
		    * H5Pexist
		    * H5Pget_size
		    * H5Pget_nprops
		    * H5Pget_class_name
		    * H5Pget_class_parent
		    * H5Pisa_class
		    * H5Pget
		    * H5Pequal
		    * H5Piterate
		    * H5Pcopy_prop
		    * H5Premove
		    * H5Punregister
		    * H5Pclose_list
		    * H5Pclose_class 
	      File Creation Properties
                    *  H5Pget_version
		    * H5Pset_userblock
		    * H5Pget_userblock
		    * H5Pset_sizes
		    * H5Pget_sizes
		    * H5Pset_sym_k
		    * H5Pget_sym_k
		    * H5Pset_istore_k
		    * H5Pget_istore_k 
	      File Access Properties
                    # H5Pset_fclose_degree
		    # H5Pget_fclose_degree
		    # H5Pset_fapl_core
		    # H5Pget_fapl_core
		    # H5Pset_fapl_family
		    # H5Pget_fapl_family
		    # H5Pset_family_offset
		    # H5Pget_family_offset
		    # H5Pset_fapl_log
		    # H5Pset_fapl_mpio   ||
		    # H5Pget_fapl_mpio   ||
		    # H5Pset_fapl_mpiposix   ||
		    # H5Pget_fapl_mpiposix   ||
		    # H5Pset_fapl_multi
		    # H5Pget_fapl_multi
		    # H5Pset_multi_type
		    # H5Pget_multi_type
		    # H5Pset_fapl_split
		    # H5Pset_fapl_sec2
		    # H5Pset_fapl_stdio
		    # H5Pset_fapl_stream
		    # H5Pget_fapl_stream
		    # H5Pget_driver
		    # H5Pset_meta_block_size
		    # H5Pget_meta_block_size
		    # H5Pset_sieve_buf_size
		    # H5Pget_sieve_buf_size
		    # H5Pset_alignment
		    # H5Pget_alignment
		    # H5Pset_cache
		    # H5Pget_cache
		    # H5Pset_gc_references
		    # H5Pget_gc_references
		    # H5Pset_fapl_gass
		    # H5Pget_fapl_gass
		    # H5Pset_fapl_srb
		    # H5Pget_fapl_srb
	      Dataset Creation Properties
                    * H5Pset_layout
		    * H5Pget_layout
		    * H5Pset_chunk
		    * H5Pget_chunk
		    * H5Pset_deflate
		    * H5Pset_fill_value
		    * H5Pget_fill_value
		    * H5Pfill_value_defined
		    * H5Pset_fill_time
		    * H5Pget_fill_time
		    * H5Pset_alloc_time
		    * H5Pget_alloc_time
		    * H5Pset_filter
		    * H5Pall_filters_avail
		    * H5Pget_nfilters
		    * H5Pget_filter
		    * H5Pget_filter_by_id
		    * H5Pmodify_filter
		    * H5Premove_filter
		    * H5Pset_fletcher32
		    * H5Pset_shuffle
		    * H5Pset_szip
		    * H5Pset_external
		    * H5Pget_external_count
		    * H5Pget_external 
	      Dataset Access, Memory, and Transfer Properties
	            * H5Pset_buffer
		    * H5Pget_buffer
		    * H5Pset_preserve
		    * H5Pget_preserve
		    * H5Pset_edc_check
		    * H5Pget_edc_check
		    * H5Pset_filter_callback
		    * H5Pset_hyper_cache *
		    * H5Pget_hyper_cache *
		    * H5Pset_hyper_vector_size
		    * H5Pget_hyper_vector_size
		    * H5Pset_btree_ratios
		    * H5Pget_btree_ratios
		    * H5Pset_vlen_mem_manager
		    * H5Pget_vlen_mem_manager
		    * H5Pset_dxpl_mpio   ||
		    * H5Pget_dxpl_mpio   ||
		    * H5Pset_dxpl_multi
		    * H5Pget_dxpl_multi
		    * H5Pset_multi_type
		    * H5Pget_multi_type
		    * H5Pset_small_data_block_size
		    * H5Pget_small_data_block_size 
  Are both of these categorizations the same?

  futhermore, H5Pcreate declares that there are these classes of property lists:
  H5P_FILE_CREATE, H5P_FILE_ACCESS, H5P_DATASET_CREATE, H5P_DATASET_XFER, H5P_MOUNT
 */

#include "hdf5_handle.h"
#include "std_iostream.h"

class plist : public hdf5_handle
{

  friend ostream& operator<<(ostream& xos, const plist& xlist);

 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  plist();

  /// Destructor.

  ~plist();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  plist(const plist& xother);

  /// Assignment operator.

  plist& operator=(const plist& xother);

  /// Copy constructor.

  plist(const hid_t xother);


  // Queries:


  /*
    ISSUE
    Implementing these would be a very thin layer over the HDF5 api.
    Perhaps not worth the effort.

  static bool is_file_create(hid_t xplist);

  static bool is_file_access(hid_t xplist);

  static bool is_dataset_create(hid_t xplist);

  static bool is_dataset_transfer(hid_t xplist);

  static bool _is_mount(hid_t xplist);
  */

  /// Is this property list a chunked dataset creation property list?

  static bool is_chunked(hid_t xplist);

  /// Is this property list a contiguous dataset creation property list?

  static bool is_contiguous(hid_t xplist);

  /// Is this property list a compact dataset creation property list?

  static bool is_compact(hid_t xplist);

  /// Is this property list an external dataset creation property list?

  static bool is_external(hid_t xplist);


  /// Write a property list to cout.

  static void write(hid_t xhid);

 protected:
 private:

  /// Writes individual property.  Called by write().

  static herr_t write_property(hid_t xhid, const char* xname, void* xiter_data);
};

#endif
