#ifndef TEMP_FILE_H
#define TEMP_FILE_H

#include "hdf5_file.h"
#include <string>

class temp_file : public hdf5_file
{
 public:
/*! @class temp_file
    @brief A temporary, writable HDF5 file that is deleted by the class
           destructor.
*/

  // Standard features:


  /// Default constructor.  Creates a new HDF5 file.

 temp_file(const std::string& xname, hid_t xcreation_plist = H5P_DEFAULT, hid_t xaccess_plist = H5P_DEFAULT);

  /// Destructor.

  ~temp_file();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  temp_file(const temp_file& xother);

  /// Assignment operator.

  temp_file& operator=(const temp_file& xother);

 protected:

  std::string _name; ///< File name.

};


#endif
