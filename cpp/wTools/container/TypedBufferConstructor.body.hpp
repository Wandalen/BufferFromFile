
// --
// constructor
// --

Template
inline Self::SelfConstructor()
{
  self._begin = NULL;
  self._end = NULL;
  self._length = 0;
}

//

Template
inline Self::SelfConstructor( typename Self::Element* data, typename Self::SizeType length )
{
  self.use( data,length );
}

//

Template
inline Self::SelfConstructor( typename Self::Element* begin, typename Self::Element* end )
{
  self.use( begin,end );
}

//

Template
inline Self::SelfConstructor( typename const Self::Class& src )
{
  self.use( src );
}

//

Template
inline
typename Self::Class&
Self::useDataOfSize( void* data, typename Self::SizeType size )
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
Self::use( typename Self::Element* begin, typename Self::Element* end )
{
  self._begin = begin;
  self._end = end;
  self._length = ( end-begin );
  return self;
}

//

Template
inline
typename Self::Class&
Self::use( typename const Self::Class& src )
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
Self::operator=( typename const Self::Class& src )
{
  self.use( src );
  return self;
}
