#ifndef _wTypedBuffer_std_hpp_
#define _wTypedBuffer_std_hpp_

#include "./TypedBuffer.hpp"

namespace wTools //
{

template< typename Element_A >
class wTypedBufferStd : public wTypedBuffer< Element_A >
{

public : // types of classes

  using Parent = wTypedBuffer< Element_A >;
  typedef Parent* ParentPointer;
  typedef const Parent* ParentPointerConst;
  typedef Parent& ParentReference;
  typedef const Parent& ParentReferenceConst;

  typedef wTypedBufferStd< Element_A > Class;
  typedef Class* ClassPointer;
  typedef const Class* ClassPointerConst;
  typedef Class& ClassReference;
  typedef const Class& ClassReferenceConst;

  typedef wTypedBufferStd< Element_A > ClassStd;
  typedef ClassStd* ClassStdPointer;
  typedef const ClassStd* ClassStdPointerConst;
  typedef ClassStd& ClassStdReference;
  typedef const ClassStd& ClassStdReferenceConst;

  typedef wTypedBuffer< Element_A > ClassPure;
  typedef ClassPure* ClassPurePointer;
  typedef const ClassPure* ClassPurePointerConst;
  typedef ClassPure& ClassPureReference;
  typedef const ClassPure& ClassPureReferenceConst;


  public : // types of elements

  typedef Element_A Element;
  typedef Element& ElementReference;
  typedef const Element& ElementReferenceConst;
  typedef Element* ElementPointer;
  typedef const Element* ElementPointerConst;

  typedef Element* Iterator;
  typedef const Element* IteratorConst;
  typedef std::reverse_iterator< Iterator > IteratorReverse;
  typedef std::reverse_iterator< IteratorConst > IteratorReverseConst;

  typedef typename Parent::PointerDifference PointerDifference;
  typedef typename Parent::SizeType SizeType;


public : // types of std aliases

  typedef Element value_type;
  typedef ElementReference reference;
  typedef ElementReferenceConst const_reference;
  typedef ElementPointer pointer;
  typedef ElementPointerConst const_pointer;
  typedef Iterator iterator;
  typedef IteratorConst const_iterator;
  typedef IteratorReverse reverse_iterator;
  typedef IteratorReverseConst const_reverse_iterator;
  typedef PointerDifference difference_type;
  typedef SizeType size_type;


public : // constructor

  inline wTypedBufferStd();
  inline wTypedBufferStd( Element* data, SizeType length );
  inline wTypedBufferStd( Element* begin, Element* end );
  inline wTypedBufferStd( const Class& src );

  inline Class& useDataOfSize( void* data, SizeType size );
  inline Class& use( Element* data, SizeType length );
  inline Class& use( Element* begin, Element* end );
  inline Class& use( const Class& src );
  inline Class& clone();
  inline Class& operator=( const Class& src );

  // inline wTypedBufferStd() : Parent::wTypedBuffer(){};
  // inline wTypedBufferStd( Element* data, SizeType length ) : Parent::wTypedBuffer( data,length ){};
  // inline wTypedBufferStd( const Class& src ) : Parent::wTypedBuffer( src ){};
  //
  // inline Class& use( void* data, SizeType size );
  // inline Class& use( Element* data, SizeType length );
  // inline Class& use( const Class& src );
  // inline Class& clone();
  // inline Class& operator=( const Class& src );

public : // caster

  inline ClassPure& asPure();
  inline operator ClassPure&() const;


public : // etc

  inline bool empty() const;
  inline SizeType size() const;

};

#include "TypedBuffer.std.body.hpp"

} // namespace wTools //

#endif // _wTypedBuffer_std_hpp_
