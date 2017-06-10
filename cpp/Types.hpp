#ifndef _ABF_Types_hpp_ //
#define _ABF_Types_hpp_

#include <cassert>
#include <tuple>

// typedef std::string string;

template< typename ... Types_A >
using SharedPtr = std::shared_ptr< Types_A ... >;

//

template< typename ... Types_A >
using _Tuple = std::tuple< Types_A ... >;

template< typename ... Types_A >
using Tuple = _Tuple< Types_A ... >;

#define tupleElementGet_M( tuple_A, index_A ) ( std::get< index_A >( tuple_A ) )
#define tupleElementTypeGet_M( Tuple_A, index_A ) std::tuple_element< index_A,Tuple_A >::type

#define assert_M assert
//#define assert_M BOOST_ASSERT

// template< size_t index_A, typename ... Types_A >
// void operator[]( Tuple< Types_A ... >& tuple,size_t xxx )
// {
//   return std::get< index_A >( tuple );
// }

//

// template< typename ... Types_A >
// struct Tuple : public _Tuple< Types_A ... >
// {
//
//   typedef _Tuple< Types_A ... > TupleStd;
//
//   template< size_t index_A >
//   using Element = typename std::tuple_element< index_A,TupleStd >::type;
//
//   template< size_t index_A >
//   Element< index_A >
//   element(){ return std::get< index_A >( this ); }
//
//   inline operator TupleStd&() const
//   {
//     TupleStd& result = *( TupleStd* )this;
//     assert_M( sizeof( *cthis ) == sizeof( result ) );
//     return result;
//   }
//
//   // template< size_t index_A >
//   // using Element = typename tupleElementTypeGet_M( std,index_A );
//
// };

#endif // _ABF_Types_hpp_
