( function _BufferFromFile_test_ss_( ) {

'use strict';

if( typeof module !== 'undefined' )
{

  var BufferFromFile = require( '../Main.ss' );
  require( 'wTools' );
  var _ = wTools;

  _.include( 'wTesting' );

}

//

var _ = wTools;
var Parent = wTools.Testing;
var sourceFilePath = _.diagnosticLocation().full; // typeof module !== 'undefined' ? __filename : document.scripts[ document.scripts.length-1 ].src;
var testDir = _.pathResolve( __dirname + '../../../tmp.tmp' )
var filePath = _.pathJoin( testDir, 'testFile.txt' );
var testData = '1 - is a random digit set from JS though mapped into memory file with help of BufferFromFile open source package.'

// --
// routines
// --

function buffersFromRaw( test )
{
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

  var buffers = Object.keys( bufferMap );

  for( var i = 0; i < buffers.length; i++)
  {
    var data = testData;
    var type = buffers[ i ];
    var mod = _.strCutOffLeft( type, [ 8, 16, 32, 64 ] )[ 1 ] / 8;

    if( mod > 1 )
    while( data.length % mod !== 0 )
    data = data.slice( 0, -1 );

    _.fileProvider.fileWrite( filePath, data );

    test.description = 'making raw buffer from file';
    var buffer = BufferFromFile( filePath );
    test.shouldBe( _.bufferRawIs( buffer._buffer ) );

    test.description = 'making ' + type + 'from raw';

    var typedBuffer = buffer[ type ]();
    test.shouldBe( _.bufferTypedIs( typedBuffer ) );
    test.identical( typedBuffer.constructor.name, type );
    test.identical( typedBuffer.byteLength, buffer._buffer.byteLength );

    var expected = _.fileProvider.fileRead
    ({
      filePath : filePath,
      encoding : 'arraybuffer'
    });
    expected = new bufferMap[ type ]( expected );
    test.identical( typedBuffer.byteLength, expected.byteLength );
    test.identical( typedBuffer, expected );

    BufferFromFile.unmap( buffer._buffer );
  }
}

// --
// proto
// --

var Self =
{

  name : 'BufferFromFile',
  sourceFilePath : sourceFilePath,
  verbosity : 1,

  tests :
  {
    buffersFromRaw : buffersFromRaw,
  },

}


Self = wTestSuite( Self )
if( typeof module !== 'undefined' && !module.parent )
_.Testing.test( Self.name );

} )( );
