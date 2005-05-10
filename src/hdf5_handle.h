#ifndef HDF5_HANDLE_H
#define HDF5_HANDLE_H

#include "hdf5.h"


/*! @class hdf5_handle
    @brief An abstract handle to some HDF5 object with an hid.

    The handle can be in one of 2 states:
    <ol>
       <li> attached:
          <p>
             When attached the handle has a non-negative hid.
          </p>
       <li> unattached:
          <p>
             When unattached the handle has a negative hid.
    </ol>

    All classes that encapsulate an HDF5 object identified
    by an hid should inherit from this class.

    To ensure that HDF5 library resources do not leak, clients and
    descendants should follow this policy:

    <ol>
       <li> Feel free to save hids in private or protected class data.

       <li> Whenever you obtain an hid directly from an HDF5 C API call,
            its reference count is set to 1 by the library.  Do not
            change the reference count when you save the hid in private
            or protected class data.

       <li> Whenever you save an hid obtained from anywhere else
            except the HDF5 C API, it's a copy of an existing hid and this
            new copy constitutes another reference to some HDF5 object.
            Thus the correct thing to do is to increment its reference count
            using H5Iinc_ref().

       <li> Whenever a destructor is called in a class storing hids, these
            references to an HDF5 object are destroyed.  The correct thing to
            do is to decrement any valid hids at the point of their destruction
            using H5Idec_ref().  Do not call any H5Xclose().  Doing so might
            invalidate other extant references to that HDF5 object.  Note that
            the reference count to the variable "_hid" introduced in this class
            is handled properly by ~hdf5_handle().  Descendants should not
            call H5Idec_ref(_hid) in their destructors.

       <li> Precondition class features that invoked H5Xclose() in their implementations
            by requiring that the reference count be 1.  hdf5_handle::close() is already
            defined correctly for the variable "_hid" introduced in this class.
	    Descendants should not redefine it.
 
       <li> Clients should not H5Xclose(), H5Iinc_ref(), or H5Idec_ref() any hid
            obtained from the test suite.  Clients are responsible for managing
            the reference counts and closure of any hids obtained directly from
            the HDF5 library calls.
    </ol>

*/


class hdf5_handle
{
 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  hdf5_handle();

  /// Destructor.  If appropriate, recycles HDF5 library resources associated with hid().

  ~hdf5_handle();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  hdf5_handle(const hdf5_handle& xother);

  /// Assignment operator.

  hdf5_handle& operator=(const hdf5_handle& xother);


  // HDF5 id interface:


  /// The HDF5 id associated with this handle.  The const qualifier on the
  /// return value hints that clients should not call H5Xclose() or H5Idec_ref()
  /// on the return value.

  const hid_t hid() const;

  /// Is this handle associated with a valid HDF5 library maintained object?

  bool is_attached() const;

  /// Decrement the reference count to hid().  The HDF5 library will close
  /// the object when it determines that the reference count is zero.

  void close();

 protected:

  hid_t  _hid;  ///< The HDF5 id associated with the underlying object.
};

#endif
