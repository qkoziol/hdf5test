#ifndef HANDLE_H
#define HANDLE_H


/*! @class handle
    @brief An object which can be attached or unattached to some underlying representation.
*/


class handle
{
 public:


  // Standard features are omitted.  All implementations are trivial
  // since the class is abstract and has no representation.


  // Attachment interface:


  /// Is this handle associated with an underlying representation?

  virtual bool is_attached() const = 0;

  /// Disconnect from underlying representation and recycle resources associated
  /// with representation if appropriate.

  virtual void detach() = 0;
};

#endif
