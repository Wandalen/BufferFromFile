( function _GraphBranch_s_( ) {

'use strict';

var _ = _global_.wTools;
var _ObjectHasOwnProperty = Object.hasOwnProperty;

// if( typeof module !== 'undefined' )
// {
//
//   require( '../UseBase.s' );
//
// }

//

function onMixinApply( mixinDescriptor, dstClass )
{

  var dstPrototype = dstClass.prototype;

  _.assert( _.mixinHas( dstPrototype,_.Copyable ) && _.mixinHas( dstPrototype,_.graph.GraphNode ),'wGraphBranch : wCopyable and wGraphNode should be mixed in first' );
  _.assert( arguments.length === 2, 'Expects exactly two arguments' );

  _.mixinApply( this, dstPrototype );
  // _.mixinApply
  // ({
  //   dstPrototype : dstPrototype,
  //   descriptor : Self,
  // });

  _.assert( Object.hasOwnProperty.call( dstPrototype,'cloneEmpty' ) );

  _.accessor.declare( dstPrototype,
  {
    elements : 'elements',
  });

}

//

function clone()
{
  var self = this;

  _.assert( arguments.length === 0, 'Expects no arguments' );

  var elements = _.methodsCall( self.elements,'clone' );
  var result = self.cloneExtending({ elements : elements });

  return result;
}

//

function cloneEmpty()
{
  var self = this;

  _.assert( arguments.length === 0, 'Expects no arguments' );

  var result = self.cloneExtending({ elements : [] });

  return result;
}

//

function _equalAre( original )
{

  if( !original )
  original = wCopyable.prototype._equalAre_functor();

  _.assert( !!original );

  return function _equalAre( src1,src2,o )
  {

    var result = original.apply( this,arguments );

    if( !result )
    return result;

    if( src1.elements.length !== src2.elements.length )
    return false;

    for( var e = 0 ; e < src2.elements.length ; e++ )
    {
      debugger; xxx
      if( !src1.elements[ e ]._equalAre( src1.elements[ e ],src2.elements[ e ],o ) )
      return false;
    }

    return true;
  }

}

//

function _elementsSet( src )
{
  var self = this;

  _.assert( _.longIs( src ) || src === null );

  self.elementsDetach();

  /* */

  if( src !== null )
  src = _.longSlice( src );

  // var validator =
  // {
  //   set : function set( obj, k, e )
  //   {
  //     debugger;
  //     if( _.objectIs( obj[ k ] ) )
  //     obj[ k ].downDetachBefore();
  //
  //     if( e.down !== self )
  //     if( _.objectIs( e ) )
  //     e.downAttachAfter( self );
  //
  //     debugger;
  //     return true;
  //   },
  //   deleteProperty : function deleteProperty( obj, k )
  //   {
  //     debugger;
  //     obj[ k ].downDetachBefore();
  //     delete obj[ k ];
  //     return true;
  //   },
  // }
  //
  // var proxy = Proxy.revocable( src, validator );
  // src = proxy.proxy;
  //
  // Object.defineProperty( src, 'revoke',
  // {
  //   value : _.routineJoin( proxy,proxy.revoke ),
  //   writable : true,
  //   enumerable : false,
  //   configurable : true,
  // });

  // _.assert( !self[ elementsSymbol ] || self[ elementsSymbol ].revoke );
  // if( self[ elementsSymbol ] && self[ elementsSymbol ].revoke )
  // {
  //   debugger;
  //   self[ elementsSymbol ].revoke();
  //   delete self[ elementsSymbol ].revoke;
  // }

  self[ elementsSymbol ] = src;

  Object.freeze( src );

  if( src )
  for( var s = 0 ; s < src.length ; s++ )
  {
    src[ s ].downAttachAfter( self );
  }

}

//

function elementsAppend( element )
{
  var self = this;
  var system = self.system;
  var elements = self.elements.slice();

  _.assert( arguments.length === 1, 'Expects single argument' );
  _.assert( element instanceof self.Node || _.arrayIs( element ) );

  _.arrayAppendArraysOnce( elements,[ element ] );
  self.elements = elements;

}

//

// function elementsDetach_static( src )
// {
//   var self = this;
//
//   _.assert( !self.instanceIs() );
//   _.assert( _.longIs( src ) );
//   _.assert( arguments.length === 1, 'Expects single argument' );
//
//   for( var s = 0 ; s < src.length ; s++ )
//   {
//     src[ s ].detach();
//   }
//
//   self[ elementsSymbol ] = src;
//
// }

//

function elementsDetach( elements )
{
  var self = this;

  // if( !self.instanceIs() )
  // return self.Self.elementsDetach( elements );

  var selfElements = self.elements ? self.elements.slice() : [];
  self[ elementsSymbol ] = selfElements;

  _.assert( self.instanceIs() );
  _.assert( arguments.length === 0 || arguments.length === 1 );

  if( elements === undefined )
  elements = self.elements ? self.elements.slice() : [];

  for( var s = 0 ; s < elements.length ; s++ )
  {
    _.assert( selfElements.indexOf( elements[ s ] ) !== -1 );
    elements[ s ].downDetachBefore();
    _.arrayRemoveOnceStrictly( selfElements,elements[ s ] );
  }

}

//

// function elementsFinit_static( src )
// {
//   var self = this;
//
//   _.assert( !self.instanceIs() );
//   _.assert( _.longIs( src ) );
//   _.assert( arguments.length === 1, 'Expects single argument' );
//
//   for( var s = 0 ; s < src.length ; s++ )
//   {
//     src[ s ].finit();
//     _.assert( src[ s ].down === null );
//   }
//
// }

//

function elementsFinit()
{
  var self = this;

  // if( !self.instanceIs() )
  // return self.Self.elementsDetach( elements );

  _.assert( self.instanceIs() );
  _.assert( arguments.length === 0, 'Expects no arguments' );

  var elements = self.elements.slice();

  for( var e = 0 ; e < elements.length ; e++ )
  {
    elements[ e ].finit();
    _.assert( elements[ e ].down === null );
  }

  _.assert( self.elements.length === 0 );

}

//

function nodeEach( o )
{
  var self = this;

  _.assert( o === undefined || _.mapIs( o ) || _.routineIs( o ) );

  if( _.routineIs( arguments[ 0 ] ) )
  o = { onUp : arguments[ 1 ] };

  o.node = self;

  o.elementsGet = function( node ){ return node.elements || []; };
  o.nameGet = function( node ){ return node.qualifiedName; };

  return _.graph.eachNode( o );
}

//

function nucleusOfTypeGet( o )
{
  var result = [];
  var o = o || Object.create( null );

  if( !o.type )
  o.type = this.Self;

  if( !o.elements )
  o.elements = this.elements;

  _.assert( _.arrayIs( o.elements ) );
  _.assert( arguments.length <= 1 );
  _.routineOptions( nucleusOfTypeGet,o );

  function addElements( es )
  {

    for( var e = 0 ; e < es.length ; e++ )
    {
      if( es[ e ] instanceof o.type )
      {
        addElements( es[ e ].elements );
        if( o.finiting )
        es[ e ].elementsDetach();
        _.assert( es[ e ].elements.length === 0 );
        _.assert( es[ e ].down === null );
        if( o.finiting )
        es[ e ].finit();
      }
      else
      result.push( es[ e ] );
    }

  }

  addElements( o.elements );

  return result;
}

nucleusOfTypeGet.defaults =
{
  type : null,
  elements : null,
  finiting : 1,
}

// --
// relations
// --

var elementsSymbol = Symbol.for( 'elements' );

var Composes =
{
}

var Aggregates =
{
}

var Associates =
{
  elements : _.define.own([]),
}

var Restricts =
{
}

var Statics =
{
  nucleusOfTypeGet : nucleusOfTypeGet,
}

// --
// declare
// --

var Functors =
{

  _equalAre : _equalAre,

}

//

var ExtendDstNotOwn =
{

  clone : clone,
  cloneEmpty : cloneEmpty,

}

//

var Supplement =
{

  _elementsSet : _elementsSet,

  elementAppend : elementsAppend,
  elementsAppend : elementsAppend,

  elementsDetach : elementsDetach,
  elementsFinit : elementsFinit,

  nodeEach : nodeEach,
  nucleusOfTypeGet : nucleusOfTypeGet,


  /* */

  Composes : Composes,
  Aggregates : Aggregates,
  Associates : Associates,
  Restricts : Restricts,
  Statics : Statics,

}

//

var Self =
{

  onMixinApply : onMixinApply,

  functors : Functors,
  supplementOwn : ExtendDstNotOwn,
  supplement : Supplement,

  name : 'wGraphBranch',
  shortName : 'GraphBranch',

}

//

if( typeof module !== 'undefined' )
module[ 'exports' ] = Self;
_.graph[ Self.shortName ] = _.mixinDelcare( Self );

})();
