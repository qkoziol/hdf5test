#ifndef DFT_LS_H
#define DFT_LS_H

#include "dft.h"

/*! @class dft_ls
    @brief A depth first traverser which lists the file contents in preorder.
*/


class dft_ls : public dft
{
 public:


  // Standard features:


  /// Default constructor.

  dft_ls();

  /// Destructor.

  ~dft_ls();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  dft_ls(const dft_ls& xother);

  /// Assignment operator.

  dft_ls& operator=(const dft_ls& xother);


  // Traversal interface:


  /// Print objects encountered.

  virtual void preorder_action();

  /// Do nothing.

  virtual void postorder_action();

  /// Do nothing.

  void reset();


};

#endif
