#ifndef _wTools_abase_Macro_hpp_ //
#define _wTools_abase_Macro_hpp_

//

#define length_M( the_var )           sizeof( the_var ) / sizeof( *the_var )

#define _str_M( A_STR )               #A_STR
#define str_M( A_STR )                _str_M( A_STR )

#define fromScope_M( name_A )         ( #name_A ),( name_A )

//

#define functionAlias_M( newName,originalName ) \
  template< typename ... Args > \
  auto newName( Args&& ... args ) \
  -> decltype( originalName( ::std::forward< Args >( args ) ... ) ) \
  { \
    return originalName( ::std::forward< Args >( args ) ... ); \
  } \

  //

#define varMaybeNull_M( varName_A,assign_A ) \
  auto BOOST_PP_CAT( varName_A, Ptr ) = assign_A; \
  auto& varName_A = *( BOOST_PP_CAT( varName_A, Ptr ).get() ); \

#endif // _wTools_abase_Macro_hpp_
