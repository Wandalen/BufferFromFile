#ifndef _wTools_ManualTyple_hpp_ //
#define _wTools_ManualTyple_hpp_

#include "Body.begin.cpp"

namespace wTools { //

// template< typename ... Elements_A >
// struct Optional;
//

template< typename Element_A >
struct Optional
{
  typedef Element_A Element;
  Element element;
  inline Optional( Element e ) : element( e ) {}
};

// template< typename Element1_A,typename Element2_A >
// struct Optional< Element1_A,Element2_A >
// {
//   typedef Element_A Element;
//   Element element;
//   inline Optional( Element e ) : element( e ) {}
// };

template< typename Element_A, const Element_A default_A >
struct OptionalWithDefault
{
  typedef Element_A Element;
  Element element;

  static const Element_A def = default_A;
  inline OptionalWithDefault( Element e ) : element( e ) {}
};

//

template< typename Element_A > struct StripOptionalElement { typedef Element_A Element; };
template< typename Element_A > struct StripOptionalElement< Optional< Element_A > > { typedef Element_A Element; };
template< typename Element_A, const Element_A default_A > struct StripOptionalElement< OptionalWithDefault< Element_A,default_A > > { typedef Element_A Element; };

template< typename Element_A >
inline
Element_A stripOptionalElement( Element_A element )
{
  return element;
}

template< typename Element_A >
inline
Element_A stripOptionalElement( Optional< Element_A > element )
{
  return element.element;
}

template< typename Element_A, const Element_A default_A >
inline
Element_A stripOptionalElement( OptionalWithDefault< Element_A,default_A > element )
{
  return element.element;
}

//

template< typename Element_A > struct IsOptional { static const bool Value = false; };
template< typename Element_A > struct IsOptional< Optional< Element_A > > { static const bool Value = true; };
template< typename Element_A, const Element_A default_A > struct IsOptional< OptionalWithDefault< Element_A,default_A > > { static const bool Value = true; };

template< typename Element_A >
inline
bool isOptional( Element_A element )
{
  return false;
}

template< typename Element_A >
inline
bool isOptional( Optional< Element_A > element )
{
  return true;
}

template< typename Element_A, const Element_A default_A >
inline
bool isOptional( OptionalWithDefault< Element_A,default_A > element )
{
  return true;
}

//

template< typename Element_A > struct HasDefault { static const bool Value = false; };
template< typename Element_A > struct HasDefault< Optional< Element_A > > { static const bool Value = false; };
template< typename Element_A, const Element_A default_A > struct HasDefault< OptionalWithDefault< Element_A,default_A > > { static const bool Value = true; };

template< typename Element_A >
inline
bool hasDefault( Element_A element )
{
  return false;
}

template< typename Element_A >
inline
bool hasDefault( Optional< Element_A > element )
{
  return false;
}

template< typename Element_A, const Element_A default_A >
inline
bool hasDefault( OptionalWithDefault< Element_A,default_A > element )
{
  return true;
}

//

template< typename Element_A > struct DefaultOfOptional { static const int Value = 0; };
template< typename Element_A > struct DefaultOfOptional< Optional< Element_A > > { static const int Value = 0; };
template< typename Element_A, const Element_A default_A > struct DefaultOfOptional< OptionalWithDefault< Element_A,default_A > > { static const Element_A Value = default_A; };

//


template< size_t index_A, typename ... Elements_A >
struct __OptionalTuple
{

  template< typename Element_A >
  inline
  Element_A&
  elementGet( size_t index )
  {
    return *(Element_A*)NULL;
  }

  inline
  int&
  elementGet()
  {
    return *(int*)NULL;
  }

  inline
  bool
  isOptional( size_t index )
  {
    return false;
  }

  static const bool _IsOptional = false;

};

//

template< size_t index_A, typename Element1_A, typename ... Elements_A >
struct __OptionalTuple< index_A, Element1_A, Elements_A ... > : __OptionalTuple< index_A+1,Elements_A ... >
{

public : // type

  typedef __OptionalTuple< index_A,Element1_A,Elements_A ... > Class;
  typedef __OptionalTuple< index_A+1,Elements_A ... > Parent;

  typedef typename StripOptionalElement< Element1_A >::Element Element;
  static const bool _IsOptional = IsOptional< Element1_A >::Value;

  static const size_t index = index_A;
  static const size_t indexReverse = sizeof...( Elements_A )-1;

public : // routine

  __OptionalTuple()
  {
  }

  __OptionalTuple( Element1_A element1, Elements_A ... elements ) : Parent( elements ... )
  {
    this->element = stripOptionalElement( element1 );
  }

  inline
  Parent&
  parent()
  {
    return (*this);
  }

  inline
  bool isOptional( size_t index )
  {
    if( index != 0 )
    return this->parent().isOptional( index-1 );
    return this->_IsOptional;
  }

  template< typename Element_A >
  inline
  Element_A&
  elementGet( size_t index )
  {
    if( index != 0 )
    return this->parent().template elementGet< Element_A >( index-1 );
    return *(Element_A*)&this->elementGet();
  }

  inline
  Element&
  elementGet()
  {
    return this->element;
  }

  inline
  void
  eachElementUniform( void (*onElement)( Element& e, size_t k ) )
  {
    onElement( this->elementGet(),this->index );
  }

public : // var

  Element element;

};

//

template< size_t index_A, typename Element1_A, typename ... Elements_A >
struct _OptionalTupleParent;

template< size_t index2_A, typename Element1_A, typename ... Elements_A >
struct _OptionalTupleParent< 0,__OptionalTuple< index2_A, Element1_A, Elements_A... > >
{
  typedef __OptionalTuple< index2_A, Element1_A, Elements_A... > Class;
};

template< size_t index_A, size_t index2_A, typename Element1_A, typename ... Elements_A >
struct _OptionalTupleParent< index_A,__OptionalTuple< index2_A, Element1_A, Elements_A ... > >
{
  typedef typename _OptionalTupleParent< index_A - 1,__OptionalTuple< index2_A + 1, Elements_A ... > >::Class Class;
};

//

template< size_t index_A, typename Element1_A, typename ... Elements_A >
struct _OptionalTupleElementType;

template< size_t index2_A, typename Element1_A, typename ... Elements_A >
struct _OptionalTupleElementType< 0,__OptionalTuple< index2_A, Element1_A, Elements_A... > >
{
  typedef Element1_A Element;
};

template< size_t index_A, size_t index2_A, typename Element1_A, typename ... Elements_A >
struct _OptionalTupleElementType< index_A,__OptionalTuple< index2_A, Element1_A, Elements_A ... > >
{
  typedef typename _OptionalTupleElementType< index_A - 1,__OptionalTuple< index2_A + 1, Elements_A ... > >::Element Element;
};

//

template< typename ... Elements_A >
struct OptionalTuple : __OptionalTuple< 0,Elements_A ... >
{

public : // type

  typedef OptionalTuple< Elements_A ... > Class;
  typedef __OptionalTuple< 0,Elements_A ... > Parent;

  template< size_t index_A >
  using ParentGet = typename _OptionalTupleParent< index_A,Parent >::Class;

  template< size_t index_A >
  // static const bool IsOptional = _OptionalTupleParent< index_A,Parent >::Class::_IsOptional;
  using IsOptional = typename _OptionalTupleParent< index_A,Parent >::Class::_IsOptional;

  template< size_t index_A >
  using Element = typename _OptionalTupleParent< index_A,Parent >::Class::Element;
  // using Element = typename _OptionalTupleElementType< index_A,Parent >::Element;

  static const size_t length = sizeof...( Elements_A );

public : // routine

  OptionalTuple()
  {
  }

  OptionalTuple( Elements_A ... elements ) : Parent( elements ... )
  {
  }

  Parent& parent()
  {
    return (*this);
  }

  template< size_t index_A >
  ParentGet< index_A >&
  parentGet()
  {
    return (*this);
  }

};

//

template< typename Tuple_A,size_t index_A >
struct OptionalTupleElementType
{
  typedef typename _OptionalTupleElementType< index_A,typename Tuple_A::Parent >::Element Element;
};

} // namespace wTools //

#include "Body.end.cpp"

#endif // _wTools_ManualTyple_hpp_
