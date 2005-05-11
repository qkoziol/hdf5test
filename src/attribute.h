#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "container.h"
class dataspace;

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

class attribute : public container
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


  /// Open an existing object identified by xname associated with a
  /// group, dataset, or named datatype identified by xhost.

  bool open(hid_t xhost, const string& xname);


  // Access interface:


  /// True if this attribute can be read.

  bool is_readable() const;

};

#endif
