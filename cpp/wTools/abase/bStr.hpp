#ifndef _wTools_abase_Str_hpp_ //
#define _wTools_abase_Str_hpp_

namespace wTools { //

template< typename Type_A >
std::string
inline toStr( const Type_A* src )
{
  return std::to_string( src );
}

std::string
inline toStr( const char* src )
{
  std::string result = src;
  return result;
}

std::string
inline toStr( char* src )
{
  std::string result = src;
  return result;
}

//

template< typename Type_A >
const std::string
inline toStr( const Type_A& src )
{
  return std::to_string( src );
}

template< typename Type_A >
std::string
inline toStr( Type_A& src )
{
  return std::to_string( src );
}

//

template<>
const std::string
inline toStr< std::string >( const std::string& src )
{
  return src;
}


template<>
std::string
inline toStr< std::string >( std::string& src )
{
  return src;
}

//

template< typename Type1_A >
std::string
inline str( const Type1_A src1 )
{
  return toStr( src1 );
}

template< typename Src1_A, typename ... Srcs_A >
std::string
inline str( const Src1_A src1, const Srcs_A ... srcs )
{
  return toStr( src1 ) + str( srcs ... );
}

// template< typename Type1_A, typename Type2_A >
// std::string
// inline str( const Type1_A src1, const Type2_A src2 )
// {
//   return toStr( src1 ) + toStr( src2 );
// }
//
// template< typename Type1_A, typename Type2_A, typename Type3_A >
// std::string
// inline str( const Type1_A src1, const Type2_A src2, const Type3_A src3 )
// {
//   return toStr( src1 ) + toStr( src2 ) + toStr( src3 );
// }
//
// template< typename Type1_A, typename Type2_A, typename Type3_A, typename Type4_A >
// std::string
// inline str( const Type1_A src1, const Type2_A src2, const Type3_A src3, const Type4_A src4 )
// {
//   return toStr( src1 ) + toStr( src2 ) + toStr( src3 ) + toStr( src4 );
// }
//
// template< typename Type1_A, typename Type2_A, typename Type3_A, typename Type4_A, typename Type5_A >
// std::string
// inline str( const Type1_A src1, const Type2_A src2, const Type3_A src3, const Type4_A src4, const Type5_A src5 )
// {
//   return toStr( src1 ) + toStr( src2 ) + toStr( src3 ) + toStr( src4 ) + toStr( src5 );
// }

} // namespace wTools //

#endif // _wTools_abase_Str_hpp_
