#ifndef _wTools_v8_fArgumentsOf_hpp_ //
#define _wTools_v8_fArgumentsOf_hpp_

#include "./fToNative.hpp"
#include "./cOptional.hpp"

#include "Body.begin.cpp"

namespace wTools { //

namespace v8 { //

template< typename ... Args_A >
struct ArgumentsOf
{

public : // type

  typedef ArgumentsOf< Args_A ... > Class;
  typedef OptionalTuple< Args_A ... > Elements;

  static const size_t length = Elements::length;

public : // method

  inline ArgumentsOf()
  {
  }

  inline ArgumentsOf( const std::string& name )
  {
    self.name = name;
  }

  inline ArgumentsOf( const FunctionCallbackInfo< Value >& info )
  {
    self.from( info );
  }

  inline Class& operator=( const FunctionCallbackInfo< Value >& info )
  {
    self.from( info );
    return self;
  }

//

  template< size_t index_A >
  typename Elements::template Element< index_A >&
  element()
  {
    return self.elements.template elementGet< typename Elements:: template Element< index_A > >( index_A );
  }

  //

  template< size_t index_A, typename ... Arguments_A >
  struct ElementDefault;

  template< size_t index_A, typename Element_A >
  struct ElementDefault< index_A,Element_A >
  {
    static inline bool set( Class& instance )
    {
      cout << "default not set" << endl;
      return false;
    }
  };

  template< size_t index_A, typename Element_A, const Element_A default_A >
  struct ElementDefault< index_A,OptionalWithDefault< Element_A,default_A > >
  {
    static inline bool set( Class& instance )
    {
      cout << "default set " << default_A << endl;
      instance.element< index_A >() = default_A;
      return true;
    }
  };

//

  inline bool from( const FunctionCallbackInfo< Value >& info )
  {
    self.good = true;

    if( self.length < (size_t)info.Length() )
    {
      self.good = false;
      errThrow( "Routine ",self.name," expects ",self.length," arguments, but got ",info.Length()," arguments" );
      return self.good;
    }

    size_t got = this->_from< 0,Args_A ... >( info,0 );

    if( self.good )
    if( self.length != got )
    {
      self.good = false;
      errThrow( "Routine ",self.name," expects ",self.length," arguments, but understood ",got," arguments" );
      return self.good;
    }

    cout << "from done " << self.good << endl;

    return self.good;
  }

  //

  template< size_t index_A = 0, typename First_A, typename ... Other_A  >
  inline size_t _from( const FunctionCallbackInfo< Value >& info, size_t indexFrom )
  {
    size_t result = indexFrom;

    cout << "_from " << indexFrom << endl;

    // constexpr bool hasDefault = HasDefault< First_A >::Value;
    constexpr bool isOptional = IsOptional< First_A >::Value;

    bool good = toNative( info[ indexFrom ], this->element< index_A >() );

    if( good )
    result = this->_from< index_A+1, Other_A... >( info,indexFrom+1 );
    else if( isOptional )
    {
      ElementDefault< index_A,First_A >::set( self );
      result = this->_from< index_A+1, Other_A... >( info,indexFrom+1 );
    }
    else
    {
      self.good = false;
      errThrow( "Routine ",self.name," expects mandatory argument #",index_A," of specific type but cant convert to that type." );
    }

    return result;
  }

//

  template< size_t index_A  >
  inline size_t _from( const FunctionCallbackInfo< Value >& info,size_t indexFrom )
  {
    return indexFrom;
    // cout << "_from done " << indexFrom << endl;
  }

public : // var

  bool good = false;
  std::string name = "Anonymous";
  Elements elements;

};

} // namespace v8 //

} // namespace wTools //

#include "Body.end.cpp"

#endif // _wTools_v8_fArgumentsOf_hpp_
