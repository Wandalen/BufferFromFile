( function _BufferFromFile_test_ss_( ) {

'use strict';

if( typeof module !== 'undefined' )
{

  var BufferFromFile = require( '../js/Main.ss' );

  require( 'wTools' );

  var _ = _global_.wTools;

  _.include( 'wTesting' );
  _.include( 'wFiles' );

}

//

var _ = wTools;
var testData = '1 - is a random digit set from JS though mapped into memory file with help of BufferFromFile open source package.'
var testDir = _.dirTempMake( _.pathDir( __dirname ) );
var filePath = _.fileProvider.pathNativize( _.pathJoin( testDir, 'testFile.txt' ) );

//

function cleanTestDir()
{
  _.fileProvider.filesDelete( testDir );
}

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
    test.is( _.bufferRawIs( descriptor.ArrayBuffer() ) );

    test.description = 'making ' + type + 'from raw';

    var buffer = descriptor[ type ]();
    test.is( _.bufferTypedIs( buffer ) );
    test.identical( buffer.constructor.name, type );
    test.identical( buffer.byteLength, descriptor.ArrayBuffer().byteLength );

    var expected = _.fileProvider.fileRead
    ({
      filePath : filePath,
      encoding : 'buffer-raw'
    });

    expected = new bufferMap[ type ]( expected );
    test.identical( buffer.byteLength, expected.byteLength );
    test.identical( buffer, expected );

    BufferFromFile.unmap( buffer );
  }

  test.description = 'NodeBuffer';

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile( filePath ).NodeBuffer();
  test.is( _.bufferNodeIs( buffer ) );
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-node' });
  test.identical( buffer, expected )
  BufferFromFile.unmap( buffer );

  test.description = 'ArrayBuffer';

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile( filePath ).ArrayBuffer();
  test.is( _.bufferRawIs( buffer ) );
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-raw' });
  test.identical( buffer.byteLength, expected.byteLength );
  BufferFromFile.unmap( buffer );
}

//

function bufferFromFile( test )
{
  test.description = 'create raw buffer from file';
  _.fileProvider.fileWrite( filePath, testData );

  var descriptor = BufferFromFile( filePath );
  test.is( _.objectIs( descriptor ) );
  test.is( _.bufferRawIs( descriptor.ArrayBuffer() ) );
  var nodeBuffer = descriptor.NodeBuffer();
  test.is( _.bufferNodeIs( nodeBuffer ) );
  test.identical( nodeBuffer.toString(), testData );
  BufferFromFile.unmap( nodeBuffer );

  test.description = 'create raw buffer from file with options';
  _.fileProvider.fileWrite( filePath, testData );

  /* by setting number of bytes to get from beginning of the file */

  var descriptor = BufferFromFile({ filePath : filePath, size : 10 });
  test.is( _.bufferRawIs( descriptor.ArrayBuffer() ) );
  var buffer = descriptor.NodeBuffer();
  test.identical( buffer.byteLength, 10 );
  test.identical( buffer.toString(), testData.slice( 0, 10 ) );
  BufferFromFile.unmap( buffer );

  /* setting size and offset */

  _.fileProvider.fileWrite( filePath, testData );
  var blockSize = _.fileProvider.fileStat( filePath ).blksize || 4096;

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
  test.is( _.bufferRawIs( descriptor.ArrayBuffer() ) );
  test.identical( descriptor.ArrayBuffer().byteLength, 5 );
  test.identical( descriptor.NodeBuffer().toString(), '11111' );
  BufferFromFile.unmap( descriptor.Buffer() );

  //

  var size = 100;
  var data = '';
  for( var i = 0; i < size; i++ )
  {
    data += i;
  }

  _.fileProvider.fileWrite( filePath, data );
  var fileSize = _.fileProvider.fileStat( filePath ).size;
  for( var i = 0; i < fileSize; i+= 10  )
  {
    var offset = _.numberRandomInt( [ 0, i ] );
    var size = _.numberRandomInt( [ 0, fileSize - offset ] );
    test.description = 'create buffer with offset: ' + offset + ' and size: ' + size + ' ,fileSize: ' + fileSize;
    var buffer = BufferFromFile({ filePath : filePath, offset : offset, size : size }).NodeBuffer();
    test.identical( buffer.length, size );
    test.identical( buffer.toString(), data.slice( offset, offset + size ) );
    BufferFromFile.unmap( buffer );
  }

  test.description = 'protection read';

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile({ filePath : filePath, protection : BufferFromFile.Protection.read }).NodeBuffer();
  test.mustNotThrowError( function()
  {
    var got = buffer[ 0 ];
  })
  BufferFromFile.unmap( buffer );

  test.description = 'protection exec';

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile({ filePath : filePath, protection : BufferFromFile.Protection.exec }).NodeBuffer();
  test.mustNotThrowError( function()
  {
    var got = buffer[ 0 ];
  })
  BufferFromFile.unmap( buffer );

  //

  test.description = 'protection readWrite'

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile({ filePath : filePath, protection : BufferFromFile.Protection.readWrite }).NodeBuffer();
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

  test.description = 'protection readWrite'

  _.fileProvider.fileWrite( filePath, testData );
  var prot = BufferFromFile.Protection;
  var buffer = BufferFromFile({ filePath : filePath, protection : prot.read | prot.write  }).NodeBuffer();
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

  // test.description = 'protection none'
  //
  // var buffer = BufferFromFile({ filePath : filePath, protection : BufferFromFile.Protection.none }).NodeBuffer();
  // test.shouldThrowError( function()
  // {
  //   buffer[ 0 ];
  // })
  // test.shouldThrowError( function()
  // {
  //   buffer[ 0 ] = 99;
  // })
  // BufferFromFile.unmap( buffer );

  //

  test.description = 'flags';

  /* flags MAP_PRIVATE */

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile({ filePath : filePath, flag : BufferFromFile.Flag.private }).NodeBuffer();
  buffer[ 0 ] = 55;
  BufferFromFile.flush( buffer );
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-node' });
  test.is( buffer !== expected );
  BufferFromFile.unmap( buffer );

  /* flags MAP_SHARED */

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile({ filePath : filePath, flag : BufferFromFile.Flag.shared }).NodeBuffer();
  buffer[ 0 ] = 55;
  BufferFromFile.flush( buffer );
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-node' });
  test.identical( buffer, expected );
  BufferFromFile.unmap( buffer );

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
      BufferFromFile({ filePath : filePath, offset : -1 });
    })

    _.fileProvider.fileWrite( filePath, testData );
    var size = _.fileProvider.fileStat( filePath ).size;
    test.shouldThrowError( function()
    {
      BufferFromFile({ filePath : filePath, offset : size + 1 });
    })

    test.description = 'incorrect size';
    _.fileProvider.fileWrite( filePath, testData );

    test.shouldThrowError( function()
    {
      BufferFromFile({ filePath : filePath, size : -2 });
    })

    test.description = 'out of file bounds';
    _.fileProvider.fileWrite( filePath, testData );
    test.shouldThrowError( function()
    {
      var size = _.fileProvider.fileStat( filePath ).size;
      BufferFromFile({ filePath : filePath, size : size * 2 });

    })

    test.description = 'out of file bounds, requested size of buffer is bigger than can be returned';
    test.shouldThrowError( function()
    {
      var size = _.fileProvider.fileStat( filePath ).size;
      BufferFromFile({ filePath : filePath, offset : Math.floor( size / 2 ), size : size });
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
    test.is( _.bufferTypedIs( buffer ) );

    /**/

    test.description = 'BufferFromFile.flush: ' + type + 'buffer as single argument';

    buffer[ 0 ] = 48 + Math.round( Math.random()*9 );
    test.mustNotThrowError( function ()
    {
      BufferFromFile.flush( buffer );
    })
    var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-raw' });
    expected = new buffersMap[ type ]( expected );
    test.identical( buffer, expected );

    test.description = 'BufferFromFile.flush: ' + type + 'buffer as option';

    buffer[ 0 ] = 49 + Math.round( Math.random()*9 );
    test.mustNotThrowError( function ()
    {
      BufferFromFile.flush( { buffer : buffer } );
    })
    var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-raw' });
    expected = new buffersMap[ type ]( expected );
    test.identical( buffer, expected );

    BufferFromFile.unmap( buffer );
  }

  //

  test.description = 'BufferFromFile.flush buffer is NodeBuffer';

  /* as argument */

  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile( filePath ).NodeBuffer();
  test.is( _.bufferNodeIs( buffer ) )
  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 59;
    BufferFromFile.flush( buffer );
  });
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-node' });
  test.identical( buffer, expected );

  /* as option */

  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 60;
    BufferFromFile.flush({ buffer : buffer });
  });
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-node' });
  test.identical( buffer, expected );

  BufferFromFile.unmap( buffer );

  //

  test.description = 'BufferFromFile.flush buffer is ArrayBuffer';
  _.fileProvider.fileWrite( filePath, testData );
  var buffer = BufferFromFile( filePath ).ArrayBuffer();
  test.is( _.bufferRawIs( buffer ) );

  /* as argument */

  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 60;
    BufferFromFile.flush( buffer );
  });
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-raw' });
  test.identical( buffer, expected );

  /* as option */

  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 61;
    BufferFromFile.flush({ buffer : buffer });
  });
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-raw' });
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

  //

  test.description = 'random buffer';
  test.shouldThrowError( function ()
  {
    BufferFromFile.flush( new ArrayBuffer( 5 ) );
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

  BufferFromFile.unmap( buffer );

  //

  test.description = 'invalid advise';
  var buffer = BufferFromFile( filePath ).ArrayBuffer();
  var expected = -1;
  BufferFromFile.advise( buffer, -1 );
  var got = BufferFromFile.status( buffer ).advise;
  test.identical( got, expected );
  BufferFromFile.unmap( buffer );

  //

  test.description = 'advise with no arguments';
  test.shouldThrowError( function ()
  {
    BufferFromFile.advise();
  });

  //

  test.description = 'first argument is no a buffer';
  test.shouldThrowError( function ()
  {
    BufferFromFile.advise( 1, 1 );
  })

  //
}

//

function status( test )
{
  _.fileProvider.fileWrite( filePath, testData );
  var stats = _.fileProvider.fileStat( filePath );

  //

  test.description = 'get status';
  var buffer = BufferFromFile( filePath ).ArrayBuffer();
  var status = BufferFromFile.status( buffer );
  test.identical( status.filePath, filePath );
  test.identical( status.offset, BigInt( 0 ) );
  test.identical( status.size, BigInt( stats.size ) );
  test.identical( status.protection, BufferFromFile.Protection.readWrite );
  test.identical( status.flag, BufferFromFile.Flag.shared );
  test.identical( status.advise, BufferFromFile.Advise.normal );
  BufferFromFile.unmap( buffer );

  //

  test.description = 'get status, buffer with options';
  var buffer = BufferFromFile
  ({
    filePath : filePath,
    size : 10,
    offset : 10,
    protection : BufferFromFile.Protection.read,
    flag : BufferFromFile.Flag.private,
    advise : BufferFromFile.Advise.random
  }).ArrayBuffer();

  var status = BufferFromFile.status( buffer );
  test.identical( status.filePath, filePath );
  test.identical( status.offset, BigInt( 10 ) );
  test.identical( status.size, BigInt( 10 ) );
  test.identical( status.protection, BufferFromFile.Protection.read );
  test.identical( status.flag, BufferFromFile.Flag.private );
  test.identical( status.advise, BufferFromFile.Advise.random );
  BufferFromFile.unmap( buffer );

  //

  test.description = 'some random buffer passed';
  var got = BufferFromFile.status( new ArrayBuffer(5) );
  var expected = undefined;
  test.identical( got, expected );

  //

  test.description = 'some random value passed';
  var got = BufferFromFile.status( 2 );
  var expected = undefined;
  test.identical( got, expected );

  //

  test.description = 'status, no args'
  test.shouldThrowError( function ()
  {
    BufferFromFile.status();
  })

  //

  test.description = 'too many args'
  test.shouldThrowError( function ()
  {
    var buffer = BufferFromFile( filePath ).NodeBuffer();
    BufferFromFile.status( buffer, 1, 2 );
  })
}

//

function unmap( test )
{
  test.description = 'after unmap buffer is empty, changes do not affect the file'
  var buffer = BufferFromFile( filePath ).NodeBuffer();
  var expected = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-node' });
  BufferFromFile.unmap( buffer );
  test.identical( buffer.length, 0 );
  buffer[ 0 ] = 101;
  var got = _.fileProvider.fileRead({ filePath : filePath, encoding : 'buffer-node' });
  test.identical( got, expected );

  //

  test.description = 'unmap, no args'
  test.shouldThrowError( function ()
  {
    BufferFromFile.unmap();
  })

  //

  test.description = 'some random buffer passed';
  test.shouldThrowError( function ()
  {
    BufferFromFile.unmap( new ArrayBuffer(5) );
  })

  //

  test.description = 'too many args'
  test.shouldThrowError( function ()
  {
    var buffer = BufferFromFile( filePath ).NodeBuffer();
    BufferFromFile.unmap( buffer, 1, 2 );
  })
}

// --
// proto
// --

var Self =
{

  name : 'BufferFromFile',
  // sourceFilePath : sourceFilePath,
  // verbosity : 5,

  silencing : 1,

  routineTimeOut : 9999999,

  onSuiteEnd : cleanTestDir,

  tests :
  {
    buffersFromRaw : buffersFromRaw,
    bufferFromFile : bufferFromFile,
    flush : flush,
    advise : advise,
    status : status,
    unmap : unmap
  },

}


Self = wTestSuite( Self )
if( typeof module !== 'undefined' && !module.parent )
_.Tester.test( Self.name );

} )( );
