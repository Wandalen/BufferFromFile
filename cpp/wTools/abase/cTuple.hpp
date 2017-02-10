#ifndef _wTools_abase_Tuple_hpp_ //
#define _wTools_abase_Tuple_hpp_

namespace wTools //
{

//

// #define tupleElementGet_M( Tuple_A, index_A ) ( boost::fusion::get< index_A >( Tuple_A ) )

// template< int index_A, typename Tuple_A >
// using TupleTypeOfElement = boost::fusion::tuple_element< index_A, Tuple_A >;

//

// template< size_t index_A = 0, typename functor_A, typename... Args_A >
// inline typename std::enable_if< index_A == sizeof...(Args_A), void>::type
// tuple_each_element( Tuple< Args_A... >&g, functor_A )
// {}
//
// template< size_t index_A = 0, typename functor_A, typename... Args_A >
// inline typename std::enable_if< index_A < sizeof...(Args_A), void>::type
// tuple_each_element( Tuple< Args_A... >& tuple, functor_A functor )
// {
//   functor( boost::fusion::get< index_A >( tuple ),index_A );
//   tuple_each_element< index_A + 1, functor_A, Args_A... >( tuple, functor );
// }
//
// //
//
// template< size_t index_A = 0, typename functor_A, typename... Args_A >
// inline
// typename std::enable_if< index_A == sizeof...( Args_A ), void >::type
// tuple_for_index( int, Tuple< Args_A... > &, functor_A )
// { }
//
// template< size_t index_A = 0, typename functor_A, typename... Args_A >
// inline
// typename std::enable_if< index_A < sizeof...(Args_A), void >::type
// tuple_for_index( int index, Tuple< Args_A...>& tuple, functor_A functor )
// {
//   if( index == 0 )
//   functor( boost::fusion::get< index_A >( tuple ),index_A );
//   tuple_for_index< index_A + 1, functor_A, Args_A... >( index-1, tuple, functor );
// }

//

template< size_t index_A = 0, typename Arg1_A, typename... Args_A >
inline
typename std::enable_if< index_A == sizeof...( Args_A )+1, Arg1_A& >::type
tupleElementGet( Tuple< Arg1_A,Args_A... >&, int )
{
  return *( Arg1_A* )( NULL );
}

template< size_t index_A = 0, typename Arg1_A, typename... Args_A >
inline
typename std::enable_if< index_A < sizeof...( Args_A )+1, Arg1_A& >::type
tupleElementGet( Tuple< Arg1_A,Args_A... >& tuple, int index )
{
  if( index == 0 )
  return tupleElementGet_M( tuple, index_A );
  return tupleElementGet< index_A + 1, Args_A... >( tuple,index-1 );
}

//

// template< size_t index_A, typename ... AllArgs_A >
// struct _tupleElementGet2
// {
//   template< typename Arg1_A, typename ... OtherArgs_A >
//   static
//   typename std::tuple_element< index_A,Tuple< AllArgs_A ... > >
//   _( Tuple< AllArgs_A ... > tuple, int left = 0 )
//   {
//     _tupleElementGet2< index_A, AllArgs_A ... >::_< OtherArgs_A >( tuple,left-1 );
//   }
// };

// template< size_t index_A = 0, typename Arg1_A, typename... Args_A >
// inline
// Arg1_A&
// tupleElementGet( Tuple< Arg1_A,Args_A... >& tuple, int index )
// {
//   if( index == 0 )
//   return tupleElementGet_M( tuple, index_A );
//   return tupleElementGet< index_A + 1, Args_A... >( tuple,index-1 );
// }
//
// template< size_t index_A, typename Arg1_A, typename... Args_A >
// inline
// void
// tupleElementGet( Tuple< Arg1_A,Args_A... >&, int )
// {
//   // return *( Arg1_A* )( NULL );
// }

//

template< size_t index_A = 0, typename Arg1_A, typename... Args_A >
inline
typename std::enable_if< index_A == sizeof...( Args_A )+1, const Arg1_A& >::type
tupleElementGet( const Tuple< Arg1_A,Args_A... >&, int )
{
  return *( Arg1_A* )( NULL );
}

template< size_t index_A = 0, typename Arg1_A, typename... Args_A >
inline
typename std::enable_if< index_A < sizeof...( Args_A )+1, const Arg1_A& >::type
tupleElementGet( const Tuple< Arg1_A,Args_A... >& tuple, int index )
{
  if( index == 0 )
  return tupleElementGet_M( tuple, index_A );
  return tupleElementGet< index_A + 1, Args_A... >( tuple,index-1 );
}

//

// template< typename Arg1_A, typename Arg2_A, typename Arg3_A >
// inline
// typename TupleTypeOfElement< 0,Tuple< Arg1_A, Arg2_A, Arg3_A > >::type
// tupleElementGet_( Tuple< Arg1_A, Arg2_A, Arg3_A >& tuple, int index )
// {
//   switch( index )
//   {
//     case 0 :
//       return tupleElementGet_M( tuple, 0 );
//     case 1 :
//       return tupleElementGet_M( tuple, 1 );
//     case 2 :
//       return tupleElementGet_M( tuple, 1 );
//   }
//   return -1;
// }
//
// template< typename Arg1_A, typename Arg2_A >
// inline
// typename TupleTypeOfElement< 0,Tuple< Arg1_A, Arg2_A > >::type
// tupleElementGet_( Tuple< Arg1_A, Arg2_A >& tuple, int index )
// {
//   switch( index )
//   {
//     case 0 :
//       return tupleElementGet_M( tuple, 0 );
//     case 1 :
//       return tupleElementGet_M( tuple, 1 );
//   }
//   return -1;
// }

} // namespace wTools //

#endif // _wTools_abase_Tuple_hpp_
