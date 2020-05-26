( function _BufferFromFile_js_()
{ //

let _BufferFromFile = require( __dirname + '/../binding/bufferfromfile.node' );
let Self = BufferFromFile;
function BufferFromFile()
{
  if( this instanceof Self )
  return this;
  let buffer = _BufferFromFile.mmap.apply( this, arguments );
  let result = new Self();
  result._buffer = buffer;
  return result;
}

Self.prototype = Object.create( _BufferFromFile );
Object.assign( Self, _BufferFromFile );

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

let bufferMap =
{
  Int8Array,
  Uint8Array,
  Uint8ClampedArray,
  Int16Array,
  Uint16Array,
  Int32Array,
  Uint32Array,
  Float32Array,
  Float64Array,
}

//

for( let b in bufferMap ) ( function()
{
  let TypedBuffer = bufferMap[ b ];
  Self.prototype[ b ] = function make()
  {
    return new TypedBuffer( this._buffer );
  }
} )();

//

Self.prototype.Buffer = _Buffer;
Self.prototype.ArrayBuffer = _ArrayBuffer;
Self.prototype.NodeBuffer = _NodeBuffer;

module.exports = Self;

} )(); //
