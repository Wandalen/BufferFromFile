#ifndef _wTools_v8_Err_hpp_ //
#define _wTools_v8_Err_hpp_

namespace wTools { //

namespace v8 { //

// #define errThrow_M( A_ERR_TEXT )                            \
//   {                                                         \
//     return Nan::ThrowError( _newStr_M( A_ERR_TEXT ) );      \
//   }

template< typename ... Srcs_A >
void errThrow( const Srcs_A ... srcs )
{
  v8::Isolate::GetCurrent()->ThrowException( vstr( srcs ... ) );
}

} // namespace v8 //

} // namespace wTools //

#endif // _wTools_v8_Err_hpp_
