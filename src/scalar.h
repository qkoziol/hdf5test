#ifndef SCALAR_H
#define SCALAR_H

#include "dataspace.h"
class extent;
class hyperslab;

/*! @class scalar
    @brief Simple dataspaces of a single point.
*/

class scalar : public dataspace
{
 public:


  // Standard features:


  /// Default constructor.  Makes an attached object, since the extent is known.

  scalar();

  /// Destructor.

  ~scalar();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  scalar(const scalar& xother);

  /// Assignment operator.

  scalar& operator=(const scalar& xother);

};

#endif
