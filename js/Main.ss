( function _BufferFromFile_js_( ) { //

var _BufferFromFile = require( __dirname + '/../build/Release/bufferfromfile.node' );
var Self = function BufferFromFile()
{
  if( this instanceof Self )
  return;
  var buffer = _BufferFromFile.mmap.apply( this,arguments );
  var result = new Self();
  result._buffer = buffer;
  return result;
}




Self.prototype = Object.create( _BufferFromFile );
Object.assign( Self,_BufferFromFile );

//

function _Buffer()
{
  return this._buffer;
}

//

function _ArrayBuffer()
{
  return this._buffer;
}

//

function _NodeBuffer()
{
  return Buffer.from( this._buffer );
}

//

var bufferMap =
{
  'Int8Array' : Int8Array,
  'Uint8Array' : Uint8Array,
  'Uint8ClampedArray' : Uint8ClampedArray,
  'Int16Array' : Int16Array,
  'Uint16Array' : Uint16Array,
  'Int32Array' : Int32Array,
  'Uint32Array' : Uint32Array,
  'Float32Array' : Float32Array,
  'Float64Array' : Float64Array,
}

//

for( var b in bufferMap ) (function()
{
  var TypedBuffer = bufferMap[ b ];
  Self.prototype[ b ] = function make()
  {
    return new TypedBuffer( this._buffer );
  }
})();

//

Self.prototype.Buffer = _Buffer;
Self.prototype.ArrayBuffer = _ArrayBuffer;
Self.prototype.NodeBuffer = _NodeBuffer;

module.exports = Self;

})(); //
