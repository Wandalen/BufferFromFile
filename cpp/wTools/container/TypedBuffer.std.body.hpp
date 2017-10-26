#ifndef _wTypedBuffer_std_body_hpp_
#define _wTypedBuffer_std_body_hpp_

#include "Body.begin.cpp"

#define SelfConstructor wTypedBufferStd
#define Self SelfConstructor< Element_A >
#define Template template< typename Element_A >

// --
// constructor
// --

#include "TypedBufferConstructor.body.hpp"

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

#undef SelfConstructor

#include "Body.end.cpp"

#endif // _wTypedBuffer_std_body_hpp_
