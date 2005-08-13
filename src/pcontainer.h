#ifndef PCONTAINER_H
#define PCONTAINER_H

#include "container.h"
#include "persistent.h"
#include <string>

/*! @class pcontainer
    @brief Persistent containers, i.e., HDF5 datasets and attributes.

    The distinguishing characteristics of this class are...

    Hmmm...

    Well, I guess it's whatever we want to specify as the common
    behavior of datasets and attributes.  And what is that?
*/

class pcontainer : public persistent, public container
{

 public:


  // Standard features:


  /// Default constructor.  Creates a handle not attached to any underlying HDF5 object.

  pcontainer();

  /// Destructor.

  virtual ~pcontainer();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  pcontainer(const pcontainer& xother);

  /// Assignment operator.

  pcontainer& operator=(const pcontainer& xother);


  /// Description of type of container.

  virtual const std::string& type_name() const = 0;


  // hdf5_handle interface:


  /// Decrement the reference count to hid().  The HDF5 library will close
  /// the object when it determines that the reference count is zero.

  void detach();


};

#endif
