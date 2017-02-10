#ifndef _wTools_v8_fFieldsOf_hpp_ //
#define _wTools_v8_fFieldsOf_hpp_

#include "./fToV8.hpp"

namespace wTools { //

namespace v8 { //

template< typename TypeTarget_A >
class _FieldsOf
{

public : // type

  typedef _FieldsOf< TypeTarget_A > Class;

  static const PropertyAttribute Regular = PropertyAttribute::None;

public : // routines

  inline _FieldsOf( Local< TypeTarget_A > target ){ this->target = target; };

  template< typename KeyType_A, typename ValType_A >
  Class&
  set( const KeyType_A key , ValType_A val )
  {
    this->target->DefineOwnProperty( this->context, vstr( key ), toV8( val ), this->access ).FromMaybe( false );
    return (*this);
  }

  template< typename KeyType_A, typename ValType_A >
  Class&
  set( const KeyType_A* key , ValType_A val )
  {
    this->target->DefineOwnProperty( this->context, vstr( key ), toV8( val ), this->access ).FromMaybe( false );
    return (*this);
  }

public : // var

  Local< TypeTarget_A > target;
  PropertyAttribute access = static_cast< PropertyAttribute >( ReadOnly | DontDelete );
  Local< Context > context = Isolate::GetCurrent()->GetCurrentContext();

};

//

template< typename TypeTarget_A >
_FieldsOf< TypeTarget_A >
FieldsOf( Local< TypeTarget_A > target )
{
  return _FieldsOf< TypeTarget_A >( target );
}

} // namespace v8 //

} // namespace wTools //

#endif // _wTools_v8_fFieldsOf_hpp_
