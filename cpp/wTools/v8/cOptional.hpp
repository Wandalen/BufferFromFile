#ifndef _wTools_v8_eOptional_hpp_ //
#define _wTools_v8_eOptional_hpp_

#include "Body.begin.cpp"

#include "./cOptional.hpp"

namespace wTools { //

namespace v8 { //
//
// // template< std::size_t I, class T >
// // struct ;
// //
// // // recursive case
// // template< std::size_t I, class Head, class... Tail >
// // struct tuple_element<I, std::tuple<Head, Tail...>>
// // : std::tuple_element<I-1, std::tuple<Tail...>> { };
// //
// // // base case
// // template< class Head, class... Tail >
// // struct tuple_element<0, std::tuple<Head, Tail...>>
// // {
// //    typedef Head type;
// // };
//
// template< typename Element_A >
// struct Optional
// {
//   typedef Element_A Element;
//   Element element;
// };
//
// //
//
// template< typename Element_A > struct StripOptionalElement { typedef Element_A Element; };
// template< typename Element_A > struct StripOptionalElement< Optional< Element_A > > { typedef Element_A Element; };
//
// template< typename Element_A > struct IsOptional { static const bool Value = false; };
// template< typename Element_A > struct IsOptional< Optional< Element_A > > { static const bool Value = true; };
//
// //
//
// template< size_t index_A, typename ... Elements_A >
// struct _StripOptionalElements{};
//
// template< size_t index_A, typename Element1_A, typename ... Elements_A >
// struct _StripOptionalElements< index_A, Element1_A, Elements_A ... > : _StripOptionalElements< index_A+1, Elements_A ... >
// {
//
//   // _StripOptionalElements( T t, Elements_A... elements ) : _StripOptionalElements<Ts...>( elements... ), tail(t) {}
//
//   typedef typename StripOptionalElement< Element1_A >::Element Element;
//   // static const bool isOptional = IsOptional< Element1_A >::value;
//
//   // typedef Tuple
//   // <
//   //   typename _StripOptionalElements< Element1_A >::Element
//   // >
//   // Elements;
//   //
//   // typedef Tuple
//   // <
//   //   typename IsOptional< Element1_A >::Value
//   // >
//   // IsOptional;
//   //
//   // Elements elements;
//
// };
//
// // template< size_t index_A, typename Element1_A, typename ... Elements_A >
// // struct _StripOptionalElements< 0, Element1_A > : _StripOptionalElements< index_A+1, Elements_A ... >
// // {
// // };
//
// template< typename ... Elements_A >
// struct StripOptionalElements : _StripOptionalElements< 0, Elements_A ... >
// {};
//
// //
//
// // template< size_t index_A, typename ... Elements_A >
// // struct _TupleElementGet;
// //
// // template< typename Element1_A, typename ... Elements_A >
// // struct _TupleElementGet< 0, _StripOptionalElements< 0,Element1_A, Elements_A ... > >
// // {
// //   typedef Element1_A Element;
// // };
// //
// // template< size_t index_A, typename Element1_A, typename ... Elements_A >
// // struct _TupleElementGet< index_A, _StripOptionalElements< index_A,Element1_A, Elements_A ... > >
// // {
// //   typedef typename _TupleElementGet< index_A - 1, _StripOptionalElements< index_A,Elements_A ... > >::Element Element;
// // };
//
// // template< size_t index_A, typename ... Elements_A >
// // struct _TupleElementGet;
// //
// // template< typename Element1_A, typename ... Elements_A >
// // struct _TupleElementGet< 0, _StripOptionalElements< 0,Element1_A, Elements_A ... > >
// // {
// //   typedef Element1_A Element;
// // };
//
// // template< size_t index_A, typename Element1_A, typename ... Elements_A >
// // struct StripOptionalElementType;
// //
// // template< size_t index_A, typename Element1_A, typename ... Elements_A >
// // struct StripOptionalElementType< index_A, StripOptionalElements< Element1_A,Elements_A ... > >
// // {
// //
// //   // typename std::enable_if< k == 0, typename elem_type_holder<0, tuple<Ts...>>::type&>::type get(tuple<Ts...>& t) {
// //
// //   typedef typename StripOptionalElementType< index_A-1, Elements_A ... >::Element Element;
// //   // typedef typename _StripOptionalElements< index_A, Elements_A ... >::Element Element;
// // };
// //
// // template< typename Element1_A, typename ... Elements_A >
// // struct StripOptionalElementType< 0, StripOptionalElements< Element1_A, Elements_A ... > >
// // {
// //   // typedef typename StripOptionalElementType< index_A-1, Elements_A ... >::Element Element;
// //   typedef Element1_A Element;
// // };
//
// template< size_t index_A, class T, class... Ts >
// struct StripOptionalElementType;
//
// template< class T, class... Ts >
// struct StripOptionalElementType< 0, StripOptionalElements< T, Ts... > >
// {
//   typedef T Element;
// };
//
// template< size_t index_A, class T, class... Ts >
// struct StripOptionalElementType< index_A, StripOptionalElements< T, Ts... > >
// {
//   typedef typename StripOptionalElementType< index_A - 1, StripOptionalElements< Ts... > >::Element Element;
// };
//
// // typedef StripOptionalElements< int,std::string > SomeTuple;
// // const StripOptionalElementType< 1,SomeTuple >::Element x = "13";
//
// // template< size_t index_A, typename ... Elements_A >
// // function()
// //
// // struct StripOptionalElementType
// // {
// //   typedef typename _StripOptionalElements< index_A, Elements_A ... >::Element Element;
// // };
//
// // template< index_A >
// // struct _StripOptionalElements<  >
// // {
// //
// // };
//
// // template< typename ... Elements_A >
// // struct StripOptionalElements;
// //
// //
// // template< typename Element1_A >
// // struct StripOptionalElements< Element1_A >
// // {
// //
// //   typedef Tuple
// //   <
// //     typename StripOptionalElement< Element1_A >::Element
// //   >
// //   Elements;
// //
// //   typedef Tuple
// //   <
// //     typename IsOptional< Element1_A >::Value
// //   >
// //   IsOptional;
// //
// //   Elements elements;
// //
// // };
// //
// // //
// //
// // template< typename Element1_A, typename Element2_A >
// // struct StripOptionalElements< Element1_A,Element2_A >
// // {
// //
// //   typedef Tuple
// //   <
// //     typename StripOptionalElement< Element1_A >::Element,
// //     typename StripOptionalElement< Element2_A >::Element
// //   >
// //   Elements;
// //
// //   typedef Tuple
// //   <
// //     typename IsOptional< Element1_A >::Value,
// //     typename IsOptional< Element2_A >::Value
// //   >
// //   IsOptional;
// //
// //   Elements elements;
// //
// // };
// //
// // //
// //
// // template< typename Element1_A, typename Element2_A, typename Element3_A >
// // struct StripOptionalElements< Element1_A,Element2_A,Element3_A >
// // {
// //
// //   typedef Tuple
// //   <
// //     typename StripOptionalElement< Element1_A >::Element,
// //     typename StripOptionalElement< Element2_A >::Element,
// //     typename StripOptionalElement< Element3_A >::Element
// //   >
// //   Elements;
// //
// //   typedef Tuple
// //   <
// //     typename IsOptional< Element1_A >::Value,
// //     typename IsOptional< Element2_A >::Value,
// //     typename IsOptional< Element3_A >::Value
// //   >
// //   IsOptional;
// //
// //   Elements elements;
// //
// // };
// //
// // //
// //
// // template< typename Element1_A, typename Element2_A, typename Element3_A, typename Element4_A >
// // struct StripOptionalElements< Element1_A,Element2_A,Element3_A,Element4_A >
// // {
// //
// //   typedef Tuple
// //   <
// //     typename StripOptionalElement< Element1_A >::Element,
// //     typename StripOptionalElement< Element2_A >::Element,
// //     typename StripOptionalElement< Element3_A >::Element,
// //     typename StripOptionalElement< Element4_A >::Element
// //   >
// //   Elements;
// //
// //   typedef Tuple
// //   <
// //     typename IsOptional< Element1_A >::Value,
// //     typename IsOptional< Element2_A >::Value,
// //     typename IsOptional< Element3_A >::Value,
// //     typename IsOptional< Element4_A >::Value
// //   >
// //   IsOptional;
// //
// //   Elements elements;
// //
// // };
// //
// // //
// //
// // template< typename Element1_A, typename Element2_A, typename Element3_A, typename Element4_A, typename Element5_A  >
// // struct StripOptionalElements< Element1_A,Element2_A,Element3_A,Element4_A,Element5_A >
// // {
// //
// //   typedef Tuple
// //   <
// //     typename StripOptionalElement< Element1_A >::Element,
// //     typename StripOptionalElement< Element2_A >::Element,
// //     typename StripOptionalElement< Element3_A >::Element,
// //     typename StripOptionalElement< Element4_A >::Element,
// //     typename StripOptionalElement< Element5_A >::Element
// //   >
// //   Elements;
// //
// //   typedef Tuple
// //   <
// //     typename IsOptional< Element1_A >::Value,
// //     typename IsOptional< Element2_A >::Value,
// //     typename IsOptional< Element3_A >::Value,
// //     typename IsOptional< Element4_A >::Value,
// //     typename IsOptional< Element5_A >::Value
// //   >
// //   IsOptional;
// //
// //   Elements elements;
// //
// // };
//
// // template< typename ... Elements_A >
// // struct StripOptionalElements
// // {
// //
// //   typedef Tuple< typename StripOptionalElement< Elements_A ... >::Element> Elements;
// //
// //   Elements elements;
// //
// // };

} // namespace v8 //

} // namespace wTools //

#include "Body.end.cpp"

#endif // _wTools_v8_eOptional_hpp_
