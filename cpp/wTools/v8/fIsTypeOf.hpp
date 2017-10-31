#ifndef _wTools_v8_IsTypeOf_hpp //
#define _wTools_v8_IsTypeOf_hpp

namespace wTools { //

namespace v8 { //

//template< typename ... Args_A, typename Target_A >
template< typename Target_A >
bool
inline isTypeOf( const LocalValue src, const Target_A target )
{
  // std::cout << "Error : None isTypeOf<> found for given input arguments" << endl;
  errThrow( "Error : None isTypeOf<> found for given input arguments" );

  return false;
};

//

#ifdef _wTypedBuffer_std_hpp_

template< typename Element_A, wTypedBuffer< Element_A >& >
bool
inline isTypeOf( const LocalValue src, const wTypedBuffer< Element_A >& target )
{
  return src->IsObject();
};

#endif

//

template<>
bool
inline isTypeOf< v8::Local< v8::Object > >( const LocalValue src, const v8::Local< v8::Object > target )
{
  // std::cout << "isTypeOf< v8::Local< v8::Object > >" << endl;
  return src->IsObject();
};

//

template<>
bool
inline isTypeOf< v8::Local< v8::ArrayBuffer > >( const LocalValue src, const v8::Local< v8::ArrayBuffer > target )
{
  // std::cout << "isTypeOf< v8::Local< v8::Object > >" << endl;
  return src->IsArrayBuffer();
};

//

template<>
bool
inline isTypeOf< v8::Local< v8::Uint32Array > >( const LocalValue src, const v8::Local< v8::Uint32Array > target )
{
  // std::cout << "isTypeOf< v8::Local< v8::Object > >" << endl;
  return src->IsUint32Array();
};

//

template<>
bool
inline isTypeOf< Chr >( const LocalValue src, const Chr target )
{
  // std::cout << "isTypeOf< Chr >" << endl;
  return src->IsUint32();
};

template<>
bool
inline isTypeOf< Wrd1 >( const LocalValue src, const Wrd1 target )
{
  // std::cout << "isTypeOf< Wrd1 >" << endl;
  return src->IsUint32();
};

template<>
bool
inline isTypeOf< Int1 >( const LocalValue src, const Int1 target )
{
  // std::cout << "isTypeOf< Int1 >" << endl;
  return src->IsInt32();
};

//

template<>
bool
inline isTypeOf< Wrd2 >( const LocalValue src, const Wrd2 target )
{
  // std::cout << "isTypeOf< Wrd2 >" << endl;
  return src->IsUint32();
};

template<>
bool
inline isTypeOf< Int2 >( const LocalValue src, const Int2 target )
{
  // std::cout << "isTypeOf< Int2 >" << endl;
  return src->IsInt32();
};

//

template<>
bool
inline isTypeOf< Wrd4Maybe2 >( const LocalValue src, const Wrd4Maybe2 target )
{
  // std::cout << "isTypeOf< Wrd4Maybe2 >" << endl;
  return src->IsUint32();
};

template<>
bool
inline isTypeOf< Int4Maybe2 >( const LocalValue src, const Int4Maybe2 target )
{
  // std::cout << "isTypeOf< Int4Maybe2 >" << endl;
  return src->IsInt32();
};

#if Wrd8Maybe4Size == 4 //

template<>
bool
inline isTypeOf< Wrd8Maybe4 >( const LocalValue src, const Wrd8Maybe4 target )
{
  // std::cout << "isTypeOf< Wrd8Maybe4 >" << endl;
  return src->IsUint32();
};

template<>
bool
inline isTypeOf< Int8Maybe4 >( const LocalValue src, const Int8Maybe4 target )
{
  // std::cout << "isTypeOf< Int8Maybe4 >" << endl;
  return src->IsInt32();
};

#elif Wrd8Maybe4Size == 8 //

template<>
bool
inline isTypeOf< Wrd8Maybe4 >( const LocalValue src, const Wrd8Maybe4 target )
{
  // std::cout << "isTypeOf< Wrd8Maybe4 >" << endl;
  return src->IsNumber();
};

template<>
bool
inline isTypeOf< Int8Maybe4 >( const LocalValue src, const Int8Maybe4 target )
{
  // std::cout << "isTypeOf< Int8Maybe4 >" << endl;
  return src->IsNumber();
};

#endif // Wrd8Maybe4Size == 8 //

template<>
bool
inline isTypeOf< Wrd8 >( const LocalValue src, const Wrd8 target )
{
  // std::cout << "isTypeOf< Wrd8 >" << endl;
  return src->IsNumber();
};

template<>
bool
inline isTypeOf< Int8 >( const LocalValue src, const Int8 target )
{
  // std::cout << "isTypeOf< Int8 > : " << src->IsNumber() << endl;

  // std::cout << "src->IsNumber() :" << src->IsNumber() << endl;
  // std::cout << "src->IsInt32() :" << src->IsInt32() << endl;
  // std::cout << "src->IsUint32() :" << src->IsUint32() << endl;
  // std::cout << "src->IsNull() :" << src->IsNull() << endl;
  // std::cout << "src->IsUndefined() :" << src->IsUndefined() << endl;

  return src->IsNumber();
};

//

template<>
bool
inline isTypeOf< bool >( const LocalValue src, const bool target )
{
  // std::cout << "isTypeOf< bool >" << endl;
  // return src->IsBoolean() || src->IsNumber();
  return true;
};

//

template<>
bool
inline isTypeOf< std::string >( const LocalValue src, const std::string target )
{
  // std::cout << "isTypeOf< string > : " << src->IsString() << endl;
  return src->IsString();
};

} // namespace v8 //

} // namespace wTools //

#endif // _wTools_v8_IsTypeOf_hpp
