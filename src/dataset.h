#ifndef DATASET_H
#define DATASET_H

#include "container.h"
class dataset_creation;
class dataspace;
class group;
class hdf5_file;
#include "std_string.h"
class tuple;

/*! @class dataset
    @brief A handle to an HDF5 dataset.

    <p>
      This class represents whatever it is
      that is unique about datasets in general and
      distinguishes them from attributes.  Datasets
      differ from attributes in several ways:
      <ol>
         <li> They can be "large".
         <li> They can be chunked, external, or compact.
         <li> One can do partial i/o on them.
      </ol>
    </p>
*/

class dataset : public container
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  dataset();

  /// Destructor.

  ~dataset();

  // Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  dataset(const dataset& xother);

  /// Assignment operator.

  dataset& operator=(const dataset& xother);


  // Container interface:


  /// Get the dataspace associated with this dataset.
 
  dataspace& get_space() const;

  /// The type stored in this dataset.

  hid_t get_type() const;


  // Persistence interface:


  /// Open an existing dataset identified by xname associated with a
  /// file or group identified by xhost.

  bool open(hid_t xhost, const string& xname);

  /// Create a new dataset identified by xname associated with a
  /// file or group identified by xhost in xfile.

  bool create(const hdf5_file& xfile,
	      hid_t            xhost,
	      const string&    xname,
	      hid_t            xtype,
	      hid_t            xspace,
	      hid_t            xcreate_plist = H5P_DEFAULT);

  /// Create a new dataset identified by xname in group xgroup
  /// in file xfile.

  bool create(const hdf5_file& xfile,
	      const string&    xgroup,
	      const string&    xname,
	      hid_t            xtype,
	      hid_t            xspace,
	      hid_t            xcreate_plist = H5P_DEFAULT);

  /// Create a new chunked dataset with indicated chunk size.
  /// The chunk_size will be added to whatever creation_plist
  /// is supplied.

  bool create(const hdf5_file& xfile,
	      const string&    xname,
	      hid_t            xtype,
	      const tuple&     xchunk_size,
	      hid_t            xcreate_plist = H5P_DEFAULT);


  // Dataset type interface:


  /// True if this is a ccontiguous dataset.

  bool is_contiguous() const;

  /// True if this is a chunked dataset.

  bool is_chunked() const;

  /// True if this is an external dataset.

  bool is_external() const;

  /// True if this is a compact dataset.

  bool is_compact() const;

 protected:


};
#endif
