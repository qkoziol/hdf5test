#ifndef HDF5_FILE_H
#define HDF5_FILE_H

#include "hdf5_handle.h"
#include "std_string.h"

/*! @class hdf5_file
    @brief A handle to an HDF5 file.

    <p>
      The original motivation behind this class was to introduce notions of
      permissions into i/o operations.  Any i/o operation can succeed only
      if the underlying file and operating systems grant one permission to
      do so.  That notion of permission is introduced in the H5F API.  The
      thought was to introduce it here as well, and to propagate the notion
      down to the contents of an HDF5 file: to datasets, attributes, and
      persistent datatypes.  But here we run into a difficulty.  While i/o
      happens at the dataset/attribute/persistent datatype level, permissions
      are specified at the file level.  While you can ask an HDF5 object
      what file it's associated with (using H5Iget_file_id()), you can't then
      ask the file what sort of permissions one has.  (Well, *maybe* you can.
      You can request copies of the creation and access plists.  From them
      you *might* be able to determine the permissions.)
    </p>
    <p>
      The only way to push the notion of permissions down to where i/o
      actually happens is to build it into this C++ test suite.  And to do
      that comprehensively requires wrapping the entire C HDF5 API with
      C++.  And that is both too big a task and takes me too far afield.
    </p>
*/

class hdf5_file : public hdf5_handle
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  hdf5_file();

  /// Destructor.

  ~hdf5_file();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  hdf5_file(const hdf5_file& xother);

  /// Assignment operator.

  hdf5_file& operator=(const hdf5_file& xother);

  /*! @enum  access
      @brief Access permissions.
  */
  enum access {RDWR, RDONLY};

  /// Attempt to open a file.  Returns true on success, false otherwise.

  bool open(const string& xname, access xacc = RDONLY);

  /// Attempt to create a new file.  Returns true on success, false otherwise.

  bool create(const string& xname);


  // Sanity tests for very basic file properties:


  /// Does file xname exist?

  static bool exists(const string& xname);

  /// Does file xname exist and can we read it?

  static bool is_readable(const string& xname);

  /// Is file xname readable and an HDF5 file?

  static bool is_hdf5(const string& xname);

  /// Does file xname exist and can we write to it?

  static bool is_writable(const string& xname);

  /// Can we create (or truncate) a file named xname?

  static bool can_create(const string& xname);


  /// Access control interface:


  /// Can we write to this file?

  bool is_write_protected() const;


 protected:

  bool _is_write_protected; ///< Can this file be modified?
};

#endif

