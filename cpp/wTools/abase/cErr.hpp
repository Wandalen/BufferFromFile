#ifndef _wTools_abase_Err_hpp_ //
#define _wTools_abase_Err_hpp_

namespace wTools { //

template< typename Type1_A >
std::runtime_error
inline
err( const Type1_A src1 )
{
  return std::runtime_error( str( src1 ) );
}

template<>
std::runtime_error
inline
err< std::runtime_error >( const std::runtime_error src1 )
{
  return src1;
}

template< typename ... Srcs_A >
std::runtime_error
inline
err( const Srcs_A ... srcs )
{
  return err( str( srcs ... ) );
}

template< typename ... Srcs_A >
std::runtime_error
inline
errThrow( const Srcs_A ... srcs )
{
  throw err( srcs ... );
}

} // namespace wTools //

#endif // _wTools_abase_Err_hpp_
