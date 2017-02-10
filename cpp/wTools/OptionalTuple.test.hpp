#ifndef _wTools_ManualTyple_hpp_ //
#define _wTools_ManualTyple_hpp_

#include "./OptionalTuple.hpp"

#include "Body.begin.cpp"

namespace wTools { //

typedef OptionalTuple< int,std::string,bool > SomeTuple;
const OptionalTupleElementType< SomeTuple,1 >::Element x = "13";
const SomeTuple::Element< 1 > y = "13";

void testTuple()
{

  OptionalTuple< int,Optional< string >,bool > optionalTuple( 1,string( "33" ),true );

  cout << "IsOptional< 0 > : " << optionalTuple.isOptional( 0 ) << endl;
  cout << "IsOptional< 1 > : " << optionalTuple.isOptional( 1 ) << endl;
  // cout << "IsOptional< 2 > : " << decltype( optionalTuple )::IsOptional< 2 > << endl;

  // cout << "_IsOptional :" << decltype( optionalTuple )::_IsOptional << endl;

  OptionalTuple< int,std::string,bool > someTuple( 1,"2",false );

  size_t i = 0;
  cout << "elementGet< int >( 0 ) : " << someTuple.elementGet< int >( i ) << " " << endl;

  // cout << "y" << y( 13,10 ) << endl;

  // SomeTuple::ParentGet< 1 > someTuple2( "2",true );

  // std::cout << "element" << i << ": " << _manualTupleElementGet( someTuple.parent(),i ) << " " << std::endl;

  std::cout << "parentGet< 2 >().element : " << someTuple.parentGet< 2 >().element << " " << std::endl;

  // auto onElement = []( auto& e, size_t k )->void
  // {
  //   cout << "e :" << e << " k :" << k << endl;
  // };
  //
  // int context = 100;

  // EachElement< int, onElement ) >::eachElement( someTuple );

  // someTuple.eachElementUniform( onElement );

  //OptionalTupleElementType< SomeTuple,1 >::Element x = "13";

}

} // namespace wTools //

#include "Body.end.cpp"

#endif // _wTools_ManualTyple_hpp_
