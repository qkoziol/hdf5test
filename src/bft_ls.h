#ifndef BFT_LS_H
#define BFT_LS_H

#include "bft.h"

/*! @class bft_ls
    @brief A breadth first traverser which lists the file contents.
*/


class bft_ls : public bft
{
 public:


  // Standard features:


  /// Default constructor.

  bft_ls();

  /// Destructor.

  ~bft_ls();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  bft_ls(const bft_ls& xother);

  /// Assignment operator.

  bft_ls& operator=(const bft_ls& xother);


  // Traversal interface:


  /// Print objects encountered.

  virtual void visit_action();

  /// Do nothing.

  void reset();


};

#endif
