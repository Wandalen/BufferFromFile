#ifndef _wTools_v8_ToNative_hpp //
#define _wTools_v8_ToNative_hpp

#include "./fIsTypeOf.hpp"

namespace wTools { //

namespace v8 { //

//

template< typename Dst_A >
bool inline toNative( const LocalValue src, Dst_A& dst )
{
  std::cout << "Error : None toNative<> found for given input arguments" << endl;
  errThrow( "Error : None toNative<> found for given input arguments" );
  return false;
};

#if defined( _wTypedBuffer_hpp_ ) && defined( NAN_H_ ) //

template< typename Element_A, wTypedBuffer< Element_A > >
bool inline toNative( const LocalValue src, wTypedBuffer< Element_A >& dst )
{
  bool result = isTypeOf( src,dst );
  if( !result )
  return result;

  Local<Object> object = src->ToObject();
  Element_A* data = node::Buffer::Data( object );
  size_t length = node::Buffer::Length( object ) / sizeof( Element_A );
  dst.use( data,length );

  return result;
};

#endif // defined( _wTypedBuffer_hpp_ ) && defined( NAN_H_ ) //

#if defined( _wTypedBuffer_hpp_ ) && !defined( NAN_H_ ) //
/*
template< typename Element_A, wTypedBuffer< Element_A > >
bool inline toNative( const LocalValue src, wTypedBuffer< Element_A >& dst )
{
  bool result = isTypeOf( src,dst );
  if( !result )
  return result;

  Local<Object> object = src->ToObject();
  Local<ArrayBuffer> buffer = v8::ArrayBufferView::Buffer( object );

  // Element_A* data = node::Buffer::Data( object );
  // size_t length = node::Buffer::Length( object ) / sizeof( Element_A );
  // dst.use( data,length );

  return result;
};
*/

template< typename Element_A, wTypedBuffer< Element_A > & >
bool inline toNative( const Local< ArrayBuffer > src, wTypedBuffer< Element_A >& dst )
{
  bool result = isTypeOf( src,dst );
  if( !result )
  return result;

  Element_A* data = src->GetContents().Data();
  size_t length = src->GetContents().ByteLength();
  dst.use( data,length );

  return result;
};

#endif // defined( _wTypedBuffer_hpp_ ) && !defined( NAN_H_ ) //

//

template< typename Velement_A >
bool inline toNative( const LocalValue src, v8::Local< Velement_A >& dst )
{
  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  // dst = src->ToObject(););

  // std::cout << ".IsArrayBuffer : " << src->IsArrayBuffer() << std::endl;
  // std::cout << ".IsUint32Array : " << src->IsUint32Array() << std::endl;

  dst = Local< Velement_A >::Cast( src );

  return result;
};

//

// template< typename Element_A >
// bool inline toNative( const LocalValue src, v8::Local< v8::Uint32Array >& dst )
// {
//   // bool result = isTypeOf( src,dst );
//   // if( !result )
//   // return result;
//
//   bool result = true;
//
//   Local< Object > object = src->ToObject();
//
//   std::cout << "IsArrayBuffer : " << src->IsArrayBuffer() << std::endl;
//   std::cout << "IsUint32Array : " << src->IsUint32Array() << std::endl;
//
//   // v8::Local< v8::ArrayBuffer > typed = src.As< v8::ArrayBuffer >();
//   dst = src.As< v8::Uint32Array >();
//
//   // void *data = view->Buffer()->GetContents().Data();
//
//   // Local<ArrayBuffer> buffer = v8::ArrayBufferView::Buffer( object );
//
//   // Element_A* data = node::Buffer::Data( object );
//   // size_t length = node::Buffer::Length( object ) / sizeof( Element_A );
//   // dst.use( data,length );
//
//   return result;
// };

//

template<>
bool inline toNative< Wrd1 >( const LocalValue src, Wrd1& dst )
{
  // std::cout << "toNative< Wrd1 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->Uint32Value();
  return result;
};

template<>
bool inline toNative< Int1 >( const LocalValue src, Int1& dst )
{
  // std::cout << "toNative< Int1 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->Int32Value();
  return result;
};

//

template<>
bool inline toNative< Wrd2 >( const LocalValue src, Wrd2& dst )
{
  // std::cout << "toNative< Wrd2 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->Uint32Value();
  return result;
};

template<>
bool inline toNative< Int2 >( const LocalValue src, Int2& dst )
{
  // std::cout << "toNative< Int2 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->Int32Value();
  return result;
};

//

template<>
bool inline toNative< Wrd4Maybe2 >( const LocalValue src, Wrd4Maybe2& dst )
{
  std::cout << "toNative< Wrd4Maybe2 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->Uint32Value();
  return result;
};

template<>
bool inline toNative< Int4Maybe2 >( const LocalValue src, Int4Maybe2& dst )
{

  // std::cout << "toNative< Int4Maybe2 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->Int32Value();
  return result;
};

#if Wrd8Maybe4Size == 4 //

template<>
bool inline toNative< Wrd8Maybe4 >( const LocalValue src, Wrd8Maybe4& dst )
{
  // std::cout << "toNative< Wrd8Maybe4 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->Uint32Value();
  return result;
};

template<>
bool inline toNative< Int8Maybe4 >( const LocalValue src, Int8Maybe4& dst )
{
  // std::cout << "toNative< Int8Maybe4 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->Int32Value();
  return result;
};

#elif Wrd8Maybe4Size == 8 //

template<>
bool inline toNative< Wrd8Maybe4 >( const LocalValue src, Wrd8Maybe4& dst )
{
  // std::cout << "toNative< Wrd8Maybe4 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->IntegerValue();
  return result;
};

template<>
bool inline toNative< Int8Maybe4 >( const LocalValue src, Int8Maybe4& dst )
{
  // std::cout << "toNative< Int8Maybe4 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->IntegerValue();
  return result;
};

#endif // Wrd8Maybe4Size == 8 //

template<>
bool inline toNative< Wrd8 >( const LocalValue src, Wrd8& dst )
{
  // std::cout << "toNative< Wrd8 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->IntegerValue();
  return result;
};

template<>
bool inline toNative< Int8 >( const LocalValue src, Int8& dst )
{
  // std::cout << "toNative< Int8 >" << endl;

  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->IntegerValue();
  return result;
};

//

template<>
bool inline toNative< bool >( const LocalValue src, bool& dst )
{
  // std::cout << "toNative< bool >" << endl;
  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  dst = src->Int32Value();
  return result;
};

//

template<>
bool inline toNative< std::string >( const LocalValue src, std::string& dst )
{
  bool result = isTypeOf( src,dst );
  if( !result )
  return result;
  // std::cout << "toNative< std::string >" << endl;
  dst = v8::toStr( src );
  return result;
};

} // namespace v8 //

} // namespace wTools //

#endif // _wTools_v8_ToNative_hpp
