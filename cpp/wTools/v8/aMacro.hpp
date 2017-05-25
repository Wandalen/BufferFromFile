#ifndef _wTools_v8_Macro_hpp_ //
#define _wTools_v8_Macro_hpp_

// make v8 variable

#define _newStr_M( arg_A )               ( ::v8::String::NewFromUtf8( ::v8::Isolate::GetCurrent(), arg_A,::v8::NewStringType::kNormal ).ToLocalChecked() )
// #define vstr( ... )               ( ::v8::String::NewFromUtf8( ::v8::Isolate::GetCurrent(), wTools::str( __VA_ARGS__ ) ).ToLocalChecked() )
// #define vstr( ... )               Nan::New< ::v8::String > ( __VA_ARGS__ ).ToLocalChecked()

#define newInt_M( val )               Nan::New< ::v8::Integer > ( val )
#define newInt8_M( val )              V8_Int8( val, info )
#define newNumber_M( val )            Nan::New< ::v8::Number > ( val )
#define newBool_M( val )              Nan::val()

#define newBuffer_M( mem,size )       ::v8::ArrayBuffer::New( ::v8::Isolate::GetCurrent(), mem, size )

// #define newFunction_M( func_A )       Nan::New< FunctionTemplate >( func_A )->GetFunction()
// #define newFunction_M( func_A )       New2< ::v8::FunctionTemplate >( func_A )->GetFunction()
// #define newFunction_M( func_A )       New2( func_A )

// Isolate::GetCurrent()->GetCurrentContext()

// option

#define vOption_M( type_A,name_A ) \
  type_A name_A; \
  if( !wTools::v8::toNative( o->Get( vstr( #name_A ) ),name_A ) ) \
  return wTools::v8::errThrow( "Routine ",routineName," expects mandatory option ",#name_A ); \

#define vOptionOptional_M( type_A,name_A,default_A ) \
  type_A name_A; \
  if( !wTools::v8::toNative( o->Get( vstr( #name_A ) ),name_A ) ) \
  name_A = default_A; \

// converter

#define argsExpects_M( A_MIN,A_MAX )                                   \
  ::v8::HandleScope scope( info.GetIsolate() );                               \
  if( ( A_MIN ) > info.Length() || info.Length() > ( A_MAX ) )                           \
  {                                                     \
    if( ( A_MIN ) == ( A_MAX )  )  \
    wTools::v8::errThrow( routineName, " expects " str_M( A_MIN ) " arguments" ); \
    else \
    wTools::v8::errThrow( routineName, " expects " str_M( A_MIN ) " .. " str_M( A_MAX ) " arguments" ); \
    return;                                             \
  }

#define argDefined_M( nth ) \
  ( info.Length() >= nth + 1 && !info[ nth ]->IsNull() && !info[ nth ]->IsUndefined() )


#define argStr_M( I, A_DST )                                \
  if( !info[ I ]->IsString() )       \
  {                                                               \
    wTools::v8::errThrow( "Expects a string as " #I " argument" ); \
    return;                                                    \
  }                                                            \
  String::Utf8Value A_DST( info[ I ]->ToString() );            \


#define argArray_M( I, A_DST )                              \
  if( !info[ I ]->IsArray() )                                  \
  {                                                            \
    wTools::v8::errThrow( "Expects an array as " #I " argument" ); \
    return;                                                    \
  }                                                            \
  Local< Array > A_DST = Local< Array >::Cast( info[ I ] )

#define argUnwrap_M( Type_A, A_DST, A_SRC )                                  \
  Type_A* A_DST = Type_A::Unwrap( A_SRC );                                \
  if( A_DST == NULL )                                                     \
  {                                                                       \
    cout << "unwrap error" << endl;  \
    E_ERR_THROW_BY_CODE( Type_A::defaultErrorCodeGet() ); \
    return;                                                               \
  }

#endif // _wTools_v8_Macro_hpp_
