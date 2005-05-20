#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "pcontainer.h"

/*! @class attribute
    @brief A handle to an HDF5 attribute.

    <p>
      An attribute is a persistent container whose size is
      fixed at creation.
    </p>
    <p>
      One can consider it to be a node in the directed graph of
      persistent objects in an HDF5 file. It is "linked" to some
      other HDF5 object.  A dataset, datatype, or group is at the tail
      and the attribute is at the head.  Such a link is similar to the
      hard and soft links supported in the H5G API, in that
      H5Aget_num_attrs() and H5Aopen_idx() and other functions allow
      one to explore the directed graph of persistent objects in an
      HDF5 file to nodes that are attributes.  No links of any kind
      emanate from an attribute.
   </p>
*/

class attribute : public pcontainer
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  attribute();

  /// Destructor.

  ~attribute();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  attribute(const attribute& xother);

  /// Assignment operator.

  attribute& operator=(const attribute& xother);


  /// The hdf5 object to which this is bound.

  hdf5_handle& tail();

  /// Get the dataspace associated with this attribute.
 
  dataspace& get_space() const;

  /// The type stored in this attribute.

  hid_t get_type() const;


  // Persistence interface:


  /// Open an existing object identified by xname.  The semantics of this
  /// open() are a union of the semantics of H5[GDT]open() and H5Aopen_name().
  /// Like H5[GDT]open(), xloc can be a file or group id, and xname is the
  /// pathname to the attribute, treating the association between the attribute
  /// and its host as an HDF5 link.  Or, like H5Aopen_name(), xloc is the
  /// name of the attribute and xloc is the hid of its host.

  bool open(hid_t xloc, const string& xname);


  // Access interface:


  /// True if this attribute can be read.

  bool is_readable() const;

  /// Type of container is "attribute".

  const string& type() const;


 protected:


  static const string _type; //< "attribute".

};

#endif
