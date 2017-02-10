#ifndef _wTools_v8_aErr_hpp_ //
#define _wTools_v8_aErr_hpp_

namespace wTools { //

namespace v8 { //

template< typename Type1_A >
LocalString
vstr( const Type1_A src1 )
{
  auto src = wTools::str( src1 );
  return _newStr_M( src.c_str() );
}

template< typename ... Srcs_A >
LocalString
vstr( Srcs_A ... srcs )
{
  auto src = wTools::str( srcs ... );
  return _newStr_M( src.c_str() );
}

//

std::string
inline toStr( Local< v8::String > src )
{
  v8::String::Utf8Value value( src );
  std::string result = std::string( *value );
  return result;
}

std::string
inline toStr( Local< v8::Value > src )
{
  v8::String::Utf8Value value( src );
  std::string result = std::string( *value );
  return result;
}

} // namespace v8 //

} // namespace wTools //

#endif // _wTools_v8_aErr_hpp_
