#ifndef _wTools_v8_ToV8_hpp //
#define _wTools_v8_ToV8_hpp

namespace wTools { //

namespace v8 { //

// template< typename TypeDst_A, typename TypeSrc_A >
// TypeDst_A New2( TypeSrc_A src );

//

// template<>
// v8::Function inline New2< v8::Function,v8::Function >( v8::Function src )
// {
//   return src;
// };

//template<>
// v8::Function inline New2< v8::FunctionTemplate,v8::FunctionCallback >( v8::FunctionCallback src )

v8::Local< v8::Function >
inline toV8( v8::FunctionCallback src )
{
  // v8::Isolate::GetCurrent()
  // v8::Local< v8::FunctionTemplate > t = v8::FunctionTemplate::New( v8::Isolate::GetCurrent(),src );
  // v8::Local< v8::Function > f = t->GetFunction();
  // v8::FunctionTemplate result( src );
  v8::Local< v8::Function > result = v8::Function::New( v8::Isolate::GetCurrent(),src );
  return result;
};

template< typename V8Element_A >
v8::Local< V8Element_A >
inline toV8( v8::Local< V8Element_A > src )
{
  return src;
};

//

v8::Local< v8::Uint32 >
inline toV8( Wrd1 src )
{
  v8::Local< v8::Uint32 > result = v8::Uint32::New( v8::Isolate::GetCurrent(),src ).As< v8::Uint32 >();
  return result;
};

v8::Local< v8::Int32 >
inline toV8( Int1 src )
{
  v8::Local< v8::Int32 > result = v8::Int32::New( v8::Isolate::GetCurrent(),src ).As< v8::Int32 >();
  return result;
};

//

v8::Local< v8::Uint32 >
inline toV8( Wrd2 src )
{
  v8::Local< v8::Uint32 > result = v8::Uint32::New( v8::Isolate::GetCurrent(),src ).As< v8::Uint32 >();
  return result;
};

v8::Local< v8::Int32 >
inline toV8( Int2 src )
{
  v8::Local< v8::Int32 > result = v8::Int32::New( v8::Isolate::GetCurrent(),src ).As< v8::Int32 >();
  return result;
};

//

v8::Local< v8::Uint32 >
inline toV8( Wrd4Maybe2 src )
{
  v8::Local< v8::Uint32 > result = v8::Uint32::New( v8::Isolate::GetCurrent(),src ).As< v8::Uint32 >();
  return result;
};

v8::Local< v8::Int32 >
inline toV8( Int4Maybe2 src )
{
  v8::Local< v8::Int32 > result = v8::Int32::New( v8::Isolate::GetCurrent(),src ).As< v8::Int32 >();
  return result;
};

#if Wrd8Maybe4Size == 4 //

v8::Local< v8::Uint32 >
inline toV8( Wrd8Maybe4 src )
{
  v8::Local< v8::Uint32 > result = v8::Uint32::New( v8::Isolate::GetCurrent(),src ).As< v8::Uint32 >();
  return result;
};

v8::Local< v8::Int32 >
inline toV8( Int8Maybe4 src )
{
  v8::Local< v8::Int32 > result = v8::Int32::New( v8::Isolate::GetCurrent(),src ).As< v8::Int32 >();
  return result;
};

#elif Wrd8Maybe4Size == 8 //

v8::Local< v8::Number >
inline toV8( Wrd8Maybe4 src )
{
  v8::Local< v8::Number > result = v8::Number::New( v8::Isolate::GetCurrent(),src );
  return result;
};

v8::Local< v8::Number >
inline toV8( Int8Maybe4 src )
{
  v8::Local< v8::Number > result = v8::Number::New( v8::Isolate::GetCurrent(),src );
  return result;
};

#endif // Wrd8Maybe4Size == 8

v8::Local< v8::BigInt >
inline toV8( Wrd8 src )
{
  v8::Local< v8::BigInt > result = v8::BigInt::New( v8::Isolate::GetCurrent(),src );
  return result;
};

v8::Local< v8::BigInt >
inline toV8( Int8 src )
{
  v8::Local< v8::BigInt > result = v8::BigInt::New( v8::Isolate::GetCurrent(),src );
  return result;
};

// str

v8::Local< v8::String >
inline toV8( const std::string src )
{
  v8::Local< v8::String > result = vstr( src );
  return result;
};

v8::Local< v8::String >
inline toV8( const char* src )
{
  v8::Local< v8::String > result = vstr( src );
  return result;
};

//

// v8::Local< v8::Number >
// inline toV8( int64_t src )
// {
//   v8::Local< v8::Number > result = v8::Number::New( v8::Isolate::GetCurrent(),src );
//   return result;
// };
//
// v8::Local< v8::Number >
// inline toV8( uint64_t src )
// {
//   v8::Local< v8::Number > result = v8::Number::New( v8::Isolate::GetCurrent(),src );
//   return result;
// };

} // namespace v8 //

} // namespace wTools //

#endif // _wTools_v8_ToV8_hpp
