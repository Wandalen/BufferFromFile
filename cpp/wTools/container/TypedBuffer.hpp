#ifndef _wTypedBuffer_hpp_
#define _wTypedBuffer_hpp_

#include <iterator>
#include "../abase/aMacro.hpp"

namespace wTools //
{

template< typename Element_A = char >
class wTypedBufferStd;

template< typename Element_A = char >
class wTypedBuffer
{

public : // types of classes

  typedef wTypedBuffer< Element_A > Class;
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

  typedef ptrdiff_t PointerDifference;
  typedef size_t SizeType;



public : // constructor

  inline wTypedBuffer();
  inline wTypedBuffer( Element* data, SizeType length );
  inline wTypedBuffer( const Class& src );

  inline Class& use( void* data, SizeType size );
  inline Class& use( Element* data, SizeType length );
  inline Class& use( const Class& src );
  inline Class& clone();
  inline Class& operator=( const Class& src );


public : // caster

  inline ClassStd& asStd() const;
  inline operator ClassStd&() const;


public: //

  inline Class subarray( Iterator begin, Iterator end );
  inline Class subarray( SizeType begin, SizeType end );
  inline void fill( const Element& value, const Iterator& begin = NULL, const Iterator& end = NULL );


public : // accessor

  inline ElementReference operator[]( SizeType i );
  inline ElementReferenceConst operator[]( SizeType i ) const;

  inline ElementPointer data() const;
  inline ElementReferenceConst at( SizeType i ) const;

  inline Iterator begin() const;
  inline Iterator end() const;

  inline ElementReference front() const;
  inline ElementReference back() const;

  inline SizeType length() const;
  inline SizeType size() const;

  inline bool operator==( const Class& other ) const;
  inline bool operator!=( const Class& other ) const;


protected : // var

  Element* _begin;
  Element* _end;
  SizeType _length;

};

#include "TypedBuffer.body.hpp"

} // namespace wTools //

#include "TypedBuffer.std.hpp"

#endif // _wTypedBuffer_hpp_
