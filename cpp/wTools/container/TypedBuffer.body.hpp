#ifndef _wTypedBuffer_body_hpp_
#define _wTypedBuffer_body_hpp_

#include <Body.begin.cpp>

#define Self wTypedBuffer< Element_A >
#define Template template< typename Element_A >
#ifdef _WIN32
#define Typename typename
#else
#define Typename
#endif

// --
// constructor
// --

Template
inline Self::wTypedBuffer()
{
  self._begin = NULL;
  self._end = NULL;
  self._length = 0;
}

//

Template
inline Self::wTypedBuffer( typename Self::Element* data, typename Self::SizeType length )
{
  self.use( data,length );
}

//

Template
inline Self::wTypedBuffer( Typename const Self::Class& src )
{
  self.use( src );
}

//

Template
inline
typename Self::Class&
Self::use( void* data, typename Self::SizeType size )
{
  assert_M( size % sizeof( Self::Element ) == 0 );

  size_t length = size / sizeof( Self::Element );
  self._begin = (Self::Element*)data;
  self._end = (Self::Element*)( ((char*)data) + length );
  self._length = length;
  return self;
}

//

Template
inline
typename Self::Class&
Self::use( typename Self::Element* data, typename Self::SizeType length )
{
  self._begin = data;
  self._end = data + length;
  self._length = length;
  return self;
}

//

Template
inline
typename Self::Class&
Self::use( Typename const Self::Class& src )
{
  self._begin = src._begin;
  self._end = src._end;
  self._length = src._length;
  return self;
}

//

Template
inline
typename Self::Class&
Self::clone()
{
  auto& result = new Self( self );
  return result;
}

//

Template
inline
typename Self::Class&
Self::operator=( Typename const Self::Class& src )
{
  self.use( src );
  return self;
}

// --
// caster
// --

Template
inline
typename Self::ClassStd&
Self::asStd() const
{
  ClassStd& result = *( ClassStd* )this;
  assert_M( sizeof( self ) == sizeof( result ) );
  return result;
}

//

Template
inline
Self::operator typename Self::ClassStd&() const
{
  return self.asStd();
}

// --
// accessor
// --

Template
inline
typename Self::ElementReference
Self::operator[]( typename Self::SizeType i )
{
  return self._begin[ i ];
}

//

Template
typename Self::ElementReferenceConst
inline Self::operator[]( typename Self::SizeType i ) const
{
  return self._begin[ i ];
}

//

Template
typename Self::ElementPointer
inline Self::data() const
{
  return self._begin;
}

//

Template
typename Self::ElementReferenceConst
inline Self::at( typename Self::SizeType i ) const
{
  return self._begin[ i ];
}

//

Template
typename Self::Iterator
inline Self::begin() const
{
  return self._begin;
}

//

Template
typename Self::Iterator
inline Self::end() const
{
  return self._end;
}

//

Template
typename Self::ElementReference
inline Self::front() const
{
  return *self._begin;
}

//

Template
typename Self::ElementReference
inline Self::back() const
{
  auto tmp = self._end;
  --tmp;
  return *tmp;
}

//

Template
typename Self::SizeType
inline Self::length() const
{
  return self._length;
}

//

Template
typename Self::SizeType
inline Self::size() const
{
  return self._length * sizeof( Element );
}

//

Template
bool
inline Self::operator==( const Class& other) const
{
  return std::equal( self.begin(), self.end(), other.begin(), other.end() );
}

//

Template
bool
inline Self::operator!=( const Class& other) const
{
  return !std::equal( self.begin(), self.end(), other.begin(), other.end() );
}

//

Template
typename Self::Class
inline Self::subarray( Iterator begin, Iterator end )
{
  return wTypedBuffer( begin, end - begin );
}

//

Template
typename Self::Class
inline Self::subarray( SizeType begin, SizeType end )
{
  Iterator iBegin = self._begin + begin;
  Iterator iEnd = self._begin + end;

  return wTypedBuffer( iBegin, iEnd - iBegin );
}

//

Template
void
inline Self::fill( const Element& value, const Iterator& begin, const Iterator& end )
{
  if( !begin || !end )
  std::fill( self._begin, self._end, value );
  else
  std::fill( begin, end, value );
}

//

#include <Body.end.cpp>

#endif // _wTypedBuffer_body_hpp_
