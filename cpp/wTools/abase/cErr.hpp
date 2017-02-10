#ifndef _wTools_abase_Err_hpp_ //
#define _wTools_abase_Err_hpp_

namespace wTools { //

template< typename Type1_A >
std::runtime_error
inline err( const Type1_A src1 )
{
  return std::runtime_error( str( src1 ) );
}

template<>
std::runtime_error
inline err< std::runtime_error >( const std::runtime_error src1 )
{
  return src1;
}

template< typename ... Srcs_A >
std::runtime_error
inline err( const Srcs_A ... srcs )
{
  return err( str( srcs ... ) );
}

// template< typename Type1_A, typename Type2_A >
// std::runtime_error
// inline err( const Type1_A src1, const Type2_A src2 )
// {
//   return err( str( src1,src2 ) );
// }
//
// template< typename Type1_A, typename Type2_A, typename Type3_A >
// std::runtime_error
// inline err( const Type1_A src1, const Type2_A src2, const Type3_A src3 )
// {
//   return err( str( src1,src2,src3 ) );
// }
//
// template< typename Type1_A, typename Type2_A, typename Type3_A, typename Type4_A >
// std::runtime_error
// inline err( const Type1_A src1, const Type2_A src2, const Type3_A src3, const Type4_A src4 )
// {
//   return err( str( src1,src2,src3,src4 ) );
// }
//
// template< typename Type1_A, typename Type2_A, typename Type3_A, typename Type4_A, typename Type5_A >
// std::runtime_error
// inline err( const Type1_A src1, const Type2_A src2, const Type3_A src3, const Type4_A src4, const Type5_A src5 )
// {
//   return err( str( src1,src2,src3,src4,src5 ) );
// }

} // namespace wTools //

#endif // _wTools_abase_Err_hpp_
