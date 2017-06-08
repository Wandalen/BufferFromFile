#ifndef _wTypedBuffer_std_body_hpp_
#define _wTypedBuffer_std_body_hpp_

#include "Body.begin.cpp"

#define Self wTypedBufferStd< Element_A >
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
inline
typename Self::Class&
Self::use( Typename Self::Element* data, Typename Self::SizeType length )
{
  auto& result = Parent::use( data,length );
  return self;
}

//

Template
inline
typename Self::Class&
Self::use( void* data, Typename Self::SizeType size )
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
Self::use( Typename const Self::Class& src )
{
  auto& result = Parent::use( src );
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
typename Self::ClassPure&
Self::asPure()
{
  ClassPure& result = *( ClassPure* )this;
  assert_M( sizeof( self ) == sizeof( result ) );
  return result;
}

//

Template
inline
Self::operator typename Self::ClassPure&() const
{
  return self.asPure();
}

// --
// etc
// --

Template
bool
inline Self::empty() const
{
  return self._length == 0;
}

//

Template
typename Self::SizeType
inline Self::size() const
{
  return self._length;
}

//

#include "Body.end.cpp"

#endif // _wTypedBuffer_std_body_hpp_
