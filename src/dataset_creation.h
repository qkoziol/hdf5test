#ifndef DATASET_CREATION_H
#define DATASET_CREATION_H

#include "dataset_property_list.h"

/*
  So.... what is this class?
*/

class dataset_creation : public dataset_property_list
{
 public:


  // Standard features:


  // Default constructor.

  dataset_creation();

  // Destructor.

  ~dataset_creation();

  // Class invariant.

  bool invariant() const;

  // Copy constructor.

  dataset_creation(const dataset_creation& xother);

  // Assignment.

  dataset_creation& operator=(const dataset_creation& xother);
};

#endif
