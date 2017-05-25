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
    var descriptor = BufferFromFile( filePath );
    test.shouldBe( _.bufferRawIs( descriptor.ArrayBuffer() ) );

    test.description = 'making ' + type + 'from raw';

    var buffer = descriptor[ type ]();
    test.shouldBe( _.bufferTypedIs( buffer ) );
    test.identical( buffer.constructor.name, type );
    test.identical( buffer.byteLength, descriptor.ArrayBuffer().byteLength );

    var expected = _.fileProvider.fileRead
    ({
      filePath : filePath,
      encoding : 'arraybuffer'
    });

    expected = new bufferMap[ type ]( expected );
    test.identical( buffer.byteLength, expected.byteLength );
    test.identical( buffer, expected );

    BufferFromFile.unmap( buffer );
  }

  test.description = 'NodeBuffer';

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile( filePath ).NodeBuffer();
  test.shouldBe( _.bufferNodeIs( buffer ) );
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer' });
  test.identical( buffer, expected )
  BufferFromFile.unmap( buffer );

  test.description = 'ArrayBuffer';

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile( filePath ).ArrayBuffer();
  test.shouldBe( _.bufferRawIs( buffer ) );
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'arraybuffer' });
  test.identical( buffer.byteLength, expected.byteLength );
  BufferFromFile.unmap( buffer );
}

//

function bufferFromFile( test )
{
  test.description = 'create raw buffer from file';
  _.fileProvider.fileWrite( filePath, testData );

  var descriptor = BufferFromFile( filePath );
  test.shouldBe( _.objectIs( descriptor ) );
  test.shouldBe( _.bufferRawIs( descriptor.ArrayBuffer() ) );
  var nodeBuffer = descriptor.NodeBuffer();
  test.shouldBe( _.bufferNodeIs( nodeBuffer ) );
  test.identical( nodeBuffer.toString(), testData );

  test.description = 'create raw buffer from file with options';
  _.fileProvider.fileWrite( filePath, testData );

  /* by setting number of bytes to get from beginning of the file */

  var descriptor = BufferFromFile({ filePath : filePath, size : 10 });
  test.shouldBe( _.bufferRawIs( descriptor.ArrayBuffer() ) );
  test.identical( descriptor.ArrayBuffer().byteLength, 10 );
  test.identical( descriptor.NodeBuffer().toString(), testData.slice( 0, 10 ) );

  /* setting size and offset */

  _.fileProvider.fileWrite( filePath, testData );
  var blockSize = _.fileProvider.fileStat( filePath ).blksize;

  var size = blockSize * 2;
  var data = '';
  for( var i = 0; i < size; i++ )
  {
    if( i < size / 2 )
    data += '0'
    else
    data += '1';
  }

  _.fileProvider.fileWrite( filePath, data );
  var descriptor = BufferFromFile({ filePath : filePath, size : 5, offset : blockSize });
  test.shouldBe( _.bufferRawIs( descriptor.ArrayBuffer() ) );
  test.identical( descriptor.ArrayBuffer().byteLength, 5 );
  test.identical( descriptor.NodeBuffer().toString(), '11111' );

  //

  test.description = 'protection read';

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile({ filePath : filePath, protection : BufferFromFile.Protection.read }).ArrayBuffer();
  test.mustNotThrowError( function()
  {
    buffer[ 0 ];
  })
  test.shouldThrowError( function()
  {
    buffer[ 0 ] = 99;
  })
  BufferFromFile.unmap( buffer );

  //

  test.description = 'protection readWrite'

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile({ filePath : filePath, protection : BufferFromFile.Protection.readWrite }).ArrayBuffer();
  test.mustNotThrowError( function()
  {
    buffer[ 0 ];
  })
  test.mustNotThrowError( function()
  {
    buffer[ 0 ] = 99;
  })
  BufferFromFile.unmap( buffer );

  //

  test.description = 'protection none'

  var buffer = BufferFromFile({ filePath : filePath, protection : BufferFromFile.Protection.none }).ArrayBuffer();
  test.shouldThrowError( function()
  {
    buffer[ 0 ];
  })
  test.shouldThrowError( function()
  {
    buffer[ 0 ] = 99;
  })
  BufferFromFile.unmap( buffer );

  //

  test.description = 'flags';

  /* flags MAP_PRIVATE */

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile({ filePath : filePath, flag : BufferFromFile.Flag.private }).ArrayBuffer();
  buffer[ 0 ] = 55;
  BufferFromFile.flush( buffer );
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer' });
  test.shouldBe( buffer !== expected );
  BufferFromFile.unmap( buffer );

  /* flags MAP_SHARED */

  _.fileProvider.fileWrite( filePath, testData );
  var descriptor = BufferFromFile({ filePath : filePath, flag : BufferFromFile.Flag.shared });
  var nodeBuffer = descriptor.NodeBuffer();
  nodeBuffer[ 0 ] = 55;
  BufferFromFile.flush( nodeBuffer );
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer' });
  test.identical( nodeBuffer, expected );
  BufferFromFile.unmap( nodeBuffer );

  /**/

  if( Config.debug )
  {
    test.description = 'no arguments provided';
    test.shouldThrowError( function()
    {
      BufferFromFile();
    })

    test.description = 'incorrect argument type';
    test.shouldThrowError( function()
    {
      BufferFromFile( 1 );
    })

    test.description = 'try to read not existing file';
    _.fileProvider.fileDelete( filePath );
    test.shouldThrowError( function()
    {
      BufferFromFile( filePath );
    })

    test.description = 'incorrect offset';
    _.fileProvider.fileWrite( filePath, testData );
    test.shouldThrowError( function()
    {
      BufferFromFile({ filePath : filePath, offset : 100 });
    })

    test.description = 'out of file bounds';
    _.fileProvider.fileWrite( filePath, testData );
    test.shouldThrowError( function()
    {
      var size = _.fileProvider.fileStat( filePath ).size;
      BufferFromFile({ filePath : filePath, size : size * 2 });
    })
  }
}

//

function flush( test )
{
  var buffersMap =
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

  var buffers = Object.keys( buffersMap );

  for( var i = 0; i < buffers.length; i++ )
  {
    var data = testData;
    var type = buffers[ i ];
    var mod = _.strCutOffLeft( type, [ 8, 16, 32, 64 ] )[ 1 ] / 8;

    if( mod > 1 )
    while( data.length % mod !== 0 )
    data = data.slice( 0, -1 );

    _.fileProvider.fileWrite( filePath, data );
    var buffer = BufferFromFile( filePath )[ type ]();
    test.shouldBe( _.bufferTypedIs( buffer ) );

    /**/

    test.description = 'BufferFromFile.flush: ' + type + 'buffer as single argument';

    buffer[ 0 ] = 48 + Math.round( Math.random()*9 );
    test.mustNotThrowError( function ()
    {
      BufferFromFile.flush( buffer );
    })
    var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'arraybuffer' });
    expected = new buffersMap[ type ]( expected );
    test.identical( buffer, expected );

    test.description = 'BufferFromFile.flush: ' + type + 'buffer as option';

    buffer[ 0 ] = 49 + Math.round( Math.random()*9 );
    test.mustNotThrowError( function ()
    {
      BufferFromFile.flush( { buffer : buffer } );
    })
    var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'arraybuffer' });
    expected = new buffersMap[ type ]( expected );
    test.identical( buffer, expected );

    BufferFromFile.unmap( buffer );
  }

  //

  test.description = 'BufferFromFile.flush buffer is NodeBuffer';

  /* as argument */

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile( filePath ).NodeBuffer();
  test.shouldBe( _.bufferNodeIs( buffer ) )
  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 59;
    BufferFromFile.flush( buffer );
  });
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer' });
  test.identical( buffer, expected );

  /* as option */

  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 60;
    BufferFromFile.flush({ buffer : buffer });
  });
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer' });
  test.identical( buffer, expected );

  BufferFromFile.unmap( buffer );

  //

  test.description = 'BufferFromFile.flush buffer is ArrayBuffer';
  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile( filePath ).ArrayBuffer();
  test.shouldBe( _.bufferRawIs( buffer ) );

  /* as argument */

  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 60;
    BufferFromFile.flush( buffer );
  });
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'arraybuffer' });
  test.identical( buffer, expected );

  /* as option */

  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 61;
    BufferFromFile.flush({ buffer : buffer });
  });
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'arraybuffer' });
  test.identical( buffer, expected );

  BufferFromFile.unmap( buffer );

  //

  test.description = 'buffer is mandatory argument';
  test.shouldThrowError( function ()
  {
    BufferFromFile.flush();
  });

  //

  test.description = 'buffer is mandatory option';
  test.shouldThrowError( function ()
  {
    BufferFromFile.flush({ sync : BufferFromFile.Sync.sync });
  });
}

//

function advise( test )
{
  var advises = _.mapOwnKeys( BufferFromFile.Advise );
  _.fileProvider.fileWrite( filePath, testData );

  var buffer = BufferFromFile( filePath ).ArrayBuffer();
  for( var i = 0; i < advises.length; i++ )
  {
    var advise = BufferFromFile.Advise[ advises[ i ] ];
    test.description = 'Advise ' + advises[ i ];

    BufferFromFile.advise( buffer, advise );
    var status = BufferFromFile.status( buffer );
    test.identical( status.advise, advise );
  }

  //

  test.description = 'advise with no arguments';
  test.shouldThrowError( function ()
  {
    BufferFromFile.advise();
  });

  test.description = 'first argument is no a buffer';
  test.shouldThrowError( function ()
  {
    BufferFromFile.advise( 1, 1 );
  })

  test.description = 'second argument is no a integer';
  test.shouldThrowError( function ()
  {
    BufferFromFile.advise( buffer, '1' );
  })

  test.description = 'invalid advise';
  test.shouldThrowError( function ()
  {
    BufferFromFile.advise( buffer, 99 );
  })

  BufferFromFile.unmap( buffer );
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
    bufferFromFile : bufferFromFile,
    flush : flush,
    advise : advise,
  },

}


Self = wTestSuite( Self )
if( typeof module !== 'undefined' && !module.parent )
_.Testing.test( Self.name );

} )( );
