#ifndef DATATYPE_H
#define DATATYPE_H

#include "persistent.h"

/*! @class datatype
    @brief A handle to an HDF5 datatype.
*/

class datatype : public persistent
{

 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  datatype();

  /// Destructor.

  ~datatype();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  datatype(const datatype& xother);

  /// Assignment operator.

  datatype& operator=(const datatype& xother);


  // Additional constructors:


  /// Construct a compound type, whose members are indicated by the array xmembers,
  /// and attempt to attach to it.  Is attached if it succeeds, unattached if it fails.
  /// Member names are "0", "1", ...

  datatype(int xct, hid_t* xmembers);

  /// Construct a compound type, whose members are indicated by the array xmembers,
  /// and attempt to attach to it.  Is attached if it succeeds, unattached if it fails.
  /// Member names are "0", "1", ...

  datatype(int xct, char** xmembers);

  /// Construct a compound type, whose members are indicated by comma delimited names
  /// of predefined types and attempt to attach to it.  Is attached if it succeeds,
  /// unattached if it fails.  Member names are "0", "1", ...

  datatype(char* xname_list);

  /// Constructs a compound type, whose members are a subset of xcompound.  The subset is indicated
  /// by the list of indices in xindex.  Attaches to the subset if it succeeds; is unattached if it fails.

  datatype(hid_t xcompound, int xct, unsigned* xindex);

  /// Constructs a compound type, whose members are a subset of xcompound.  The subset is indicated
  /// by the list of member names in xmembers.  Attaches to the subset if it succeeds; is unattached
  /// if it fails.

  datatype(hid_t xcompound, int xct, char** xmembers);


  // Persistence interface:


  /// Open an existing datatype in file or group xhost.

  bool open(hid_t xhost, const std::string& xname);


  // Attachment interface:


  /// Construct a compound type, whose members are indicated by the array xmembers,
  /// and attach to it.  Returns true if it succeeds, false if it fails.
  /// Member names are "0", "1", ...

  bool attach(int xct, hid_t* xmembers);

  /// Construct a compound type, whose members are indicated by the names of the
  /// predefined types in xmembers, and attach to it.  Returns true if it succeeds
  /// and false if it fails.  Supports names of structure H5T_ARCH_BASE, where
  /// ARCH is one of those named in table 2 of http://hdf.ncsa.uiuc.edu/HDF5/doc/UG/
  /// and BASE is one of F, I, U (a subset of table 3 of http://hdf.ncsa.uiuc.edu/HDF5/doc/UG/),
  /// followed by one of those named in table 4 of http://hdf.ncsa.uiuc.edu/HDF5/doc/UG/.
  /// Member names are "0", "1", ...

  bool attach(int xct, char** xmembers);

  /// Returns the predefined HDF5 datatype hid for many type names, typically ints, floats, and bitfields
  /// of different sizes.  See .../html/PredefDTypes.html in the HDF5 source distribution for typical names.

  static hid_t predefined(const char* xname);

  /// Construct a compound type, whose members are indicated by comma delimited names
  /// of predefined types, and attach to it.  Returns true if it succeeds, false if it fails.
  /// Member names are "0", "1", ...

  bool attach(char* xname_list);

  using hdf5_handle::attach;

  /// Constructs a compound type, whose members are a subset of xcompound.  The subset is indicated
  /// by the list of indices in xindex.  Returns true and attaches to the subset if it succeeds,
  /// false and unattached if it fails.

  bool attach(hid_t xcompound, int xct, unsigned* xindex);

  /// Constructs a compound type, whose members are a subset of xcompound.  The subset is indicated
  /// by the list of member names in xmembers.  Returns true and attaches to the subset if it succeeds,
  /// false and unattached if it fails.

  bool attach(hid_t xcompound, int xct, char** xmembers);


  // Creation of new compound types:


  /// Construct a compound type, whose members are indicated by the array xmembers,
  /// and return the associated HDF5 hid.  Member names are "0", "1", ...  A return value < 0
  /// indicates failure.

  static hid_t create(int xct, hid_t* xmembers);

  /// Construct a compound type, whose members are indicated by the names of the
  /// predefined types in xmembers, and return the associated HDF5 hid.  Supports names of
  /// structure H5T_ARCH_BASE, where ARCH is one of those named in table 2 of http://hdf.ncsa.uiuc.edu/HDF5/doc/UG/
  /// and BASE is one of F, I, U (a subset of table 3 of http://hdf.ncsa.uiuc.edu/HDF5/doc/UG/),
  /// followed by one of those named in table 4 of http://hdf.ncsa.uiuc.edu/HDF5/doc/UG/.
  /// Member names are "0", "1", ...  A return value < 0 indicates failure.

  static hid_t create(int xct, char** xmembers);

  /// Construct a compound type, whose members are indicated by comma delimited names
  /// of predefined types, and return the associated hid.  A return value < 0 indicates failure.
  /// Member names are "0", "1", ...

  static hid_t create(char* xname_list);

  /// Constructs a compound type, whose members are a subset of xcompound.  The subset is indicated
  /// by the list of indices in xindex.  Returns the associated HDF5 hid.  A return value < 0
  /// indicates failure.

  static hid_t create(hid_t xcompound, int xct, unsigned* xindex);

  /// Constructs a compound type, whose members are a subset of xcompound.  The subset is indicated
  /// by the list of member names in xmembers.  Returns the associated HDF5 hid.  A return value < 0
  /// indicates failure.

  static hid_t create(hid_t xcompound, int xct, char** xmembers);

  /// Constructs a compound type, whose members are a subset of xcompound.  The subset is indicated
  /// by the list of member names in xmembers.  Returns the associated HDF5 hid.  A return value < 0
  /// indicates failure.

  static hid_t create(hid_t xcompound, char* xname_list);

  /// Returns true if xname_list contains non-negative digits separated by commas.

  static bool names_are_indices(char* xname_list);
};

#endif

