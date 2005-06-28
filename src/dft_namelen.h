#ifndef DFT_NAMELEN_H
#define DFT_NAMELEN_H

#include "dft.h"

/*! @class dft_namelen
    @brief A depth first traverser which determines the maximum length of any
           dataset or attribute pathname.
*/


class dft_namelen : public dft
{
 public:


  // Standard features:


  /// Default constructor.

  dft_namelen();

  /// Destructor.

  virtual ~dft_namelen();

  /// Class invariant.  Should always be true.

  bool invariant() const;

  /// Copy constructor.

  dft_namelen(const dft_namelen& xother);

  /// Assignment operator.

  dft_namelen& operator=(const dft_namelen& xother);


  // Traversal interface:


  /// Update longest name if current name is greater than current longest name.

  virtual void preorder_action();

  /// Do nothing.

  virtual void postorder_action();

  /// Do nothing.

  void reset();


  // Name length interface.


  /// In "local" mode, longest() returns longest local name.  In "path" mode, longest()
  /// returns longest pathname.

  enum mode { LOCAL, PATH };

  /// Length of longest name.

  size_t longest() const;

  /// Set the mode of operation.

  void set(mode xmode = PATH);

  /// Get the mode of operation.

  mode get_mode() const;


 protected:

  size_t  _longest;  ///< Max length of any name at current point in traversal.

  mode    _mode;     ///< Mode of operation. 


};

#endif
