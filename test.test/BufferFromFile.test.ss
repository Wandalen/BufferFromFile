( function _BufferFromFile_test_ss_( ) {

'use strict';

if( typeof module !== 'undefined' )
{

  var BufferFromFile = require( '../js/Main.ss' );

  require( 'wTools' );

  var _ = _global_.wTools;

  _.include( 'wTesting' );
  _.include( 'wFiles' );
  _.include( 'wAppBasic' );

}

//

var _ = wTools;

//

function onSuiteBegin()
{ 
  let context = this;
  
  context.suiteTempPath = _.path.pathDirTempOpen( _.path.join( __dirname, '..'  ), 'BufferFromFile' );
  context.assetsOriginalSuitePath = _.path.join( __dirname, '_asset' );
  context.filePath = _.fileProvider.path.nativize( _.path.join( context.suiteTempPath, 'testFile.txt' ) );
  context.testData = '1 - is a random digit set from JS though mapped into memory file with help of BufferFromFile open source package.'
  context.bufferFromFilePath = _.path.nativize( _.path.join( _.path.normalize( __dirname ), '../js/Main.ss' ) );
  context.toolsPath = _.path.nativize( _.path.join( _.path.normalize( __dirname ), '../proto/dwtools/Tools.s' ) );
  _.fileProvider.fieldPush( 'UsingBigIntForStat', 0 );
}

function onSuiteEnd()
{ 
  let context = this;
  _.assert( _.strHas( context.suiteTempPath, 'BufferFromFile' ), context.suiteTempPath );
  _.fileProvider.path.pathDirTempClose( context.suiteTempPath );
  _.fileProvider.fieldPop( 'UsingBigIntForStat', 0 );
}

function assetFor( test, asset )
{
  let self = this;
  let a = test.assetFor( asset );

  a.reflect = function reflect()
  {

    let reflected = a.fileProvider.filesReflect({ reflectMap : { [ a.originalAssetPath ] : a.routinePath }, onUp : onUp });

    reflected.forEach( ( r ) =>
    { 
      if( r.dst.ext !== 'js' && r.dst.ext !== 's' )
      return;
      var read = a.fileProvider.fileRead( r.dst.absolute );
      read = _.strReplace( read, `'wTools'`, `'${_.strEscape( self.toolsPath )}'` );
      read = _.strReplace( read, `'bufferFromFile'`, `'${_.strEscape( self.bufferFromFilePath )}'` );
      a.fileProvider.fileWrite( r.dst.absolute, read );
    });

  }

  return a;

  function onUp( r )
  {
    if( !_.strHas( r.dst.relative, '.atest.' ) )
    return;
    let relative = _.strReplace( r.dst.relative, '.atest.', '.test.' );
    r.dst.relative = relative;
    _.assert( _.strHas( r.dst.absolute, '.test.' ) );
  }

}

// --
// routines
// --

function buffersFromRaw( test )
{ 
  let context = this;
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
    var data = context.testData;
    var type = buffers[ i ];
    var mod = _.strIsolateLeftOrNone( type, [ '8', '16', '32', '64' ] )[ 1 ] / 8;

    if( mod > 1 )
    while( data.length % mod !== 0 )
    data = data.slice( 0, -1 );

    _.fileProvider.fileWrite( context.filePath, data );

    test.description = 'making raw buffer from file';
    var descriptor = BufferFromFile( context.filePath );
    test.is( _.bufferRawIs( descriptor.ArrayBuffer() ) );

    test.description = 'making ' + type + 'from raw';

    var buffer = descriptor[ type ]();
    test.is( _.bufferTypedIs( buffer ) );
    test.identical( buffer.constructor.name, type );
    test.identical( buffer.byteLength, descriptor.ArrayBuffer().byteLength );
    
    var expected = _.fileProvider.fileRead
    ({
      filePath : context.filePath,
      encoding : 'buffer.raw'
    });

    expected = new bufferMap[ type ]( expected );
    test.identical( buffer.byteLength, expected.byteLength );
    test.identical( buffer, expected );

    BufferFromFile.unmap( buffer );
  }

  test.description = 'NodeBuffer';

  _.fileProvider.fileWrite( context.filePath, context.testData );
  var buffer = BufferFromFile( context.filePath ).NodeBuffer();
  test.is( _.bufferNodeIs( buffer ) );
  var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.node' });
  test.identical( buffer, expected )
  BufferFromFile.unmap( buffer );

  test.description = 'ArrayBuffer';

  _.fileProvider.fileWrite( context.filePath, context.testData );
  var buffer = BufferFromFile( context.filePath ).ArrayBuffer();
  test.is( _.bufferRawIs( buffer ) );
  var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.raw' });
  test.identical( buffer.byteLength, expected.byteLength );
  BufferFromFile.unmap( buffer );
}

//

function bufferFromFile( test )
{ 
  let context = this;
  
  test.description = 'create raw buffer from file';
  _.fileProvider.fileWrite( context.filePath, context.testData );

  var descriptor = BufferFromFile( context.filePath );
  test.is( _.objectIs( descriptor ) );
  test.is( _.bufferRawIs( descriptor.ArrayBuffer() ) );
  var nodeBuffer = descriptor.NodeBuffer();
  test.is( _.bufferNodeIs( nodeBuffer ) );
  test.identical( nodeBuffer.toString(), context.testData );
  BufferFromFile.unmap( nodeBuffer );

  test.description = 'create raw buffer from file with options';
  _.fileProvider.fileWrite( context.filePath, context.testData );

  /* by setting number of bytes to get from beginning of the file */

  var descriptor = BufferFromFile({ filePath : context.filePath, size : 10 });
  test.is( _.bufferRawIs( descriptor.ArrayBuffer() ) );
  var buffer = descriptor.NodeBuffer();
  test.identical( buffer.byteLength, 10 );
  test.identical( buffer.toString(), context.testData.slice( 0, 10 ) );
  BufferFromFile.unmap( buffer );

  /* setting size and offset */

  _.fileProvider.fileWrite( context.filePath, context.testData );
  var blockSize = _.fileProvider.statRead( context.filePath ).blksize || 4096;

  var size = blockSize * 2;
  var data = '';
  for( var i = 0; i < size; i++ )
  {
    if( i < size / 2 )
    data += '0'
    else
    data += '1';
  }

  _.fileProvider.fileWrite( context.filePath, data );
  var descriptor = BufferFromFile({ filePath : context.filePath, size : 5, offset : blockSize });
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

  _.fileProvider.fileWrite( context.filePath, data );
  var fileSize = _.fileProvider.statRead( context.filePath ).size;
  for( var i = 0; i < fileSize; i+= 10  )
  {
    var offset = _.intRandom( [ 0, i ] );
    var size = _.intRandom( [ 0, fileSize - offset ] );
    test.description = 'create buffer with offset: ' + offset + ' and size: ' + size + ' ,fileSize: ' + fileSize;
    var buffer = BufferFromFile({ filePath : context.filePath, offset : offset, size : size }).NodeBuffer();
    test.identical( buffer.length, size );
    test.identical( buffer.toString(), data.slice( offset, offset + size ) );
    BufferFromFile.unmap( buffer );
  }

  test.description = 'protection read';

  _.fileProvider.fileWrite( context.filePath, context.testData );
  var buffer = BufferFromFile({ filePath : context.filePath, protection : BufferFromFile.Protection.read }).NodeBuffer();
  test.mustNotThrowError( function()
  {
    var got = buffer[ 0 ];
  })
  BufferFromFile.unmap( buffer );

  test.description = 'protection exec';

  _.fileProvider.fileWrite( context.filePath, context.testData );
  var buffer = BufferFromFile({ filePath : context.filePath, protection : BufferFromFile.Protection.exec }).NodeBuffer();
  test.mustNotThrowError( function()
  {
    var got = buffer[ 0 ];
  })
  BufferFromFile.unmap( buffer );

  //

  test.description = 'protection readWrite'

  _.fileProvider.fileWrite( context.filePath, context.testData );
  var buffer = BufferFromFile({ filePath : context.filePath, protection : BufferFromFile.Protection.readWrite }).NodeBuffer();
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

  _.fileProvider.fileWrite( context.filePath, context.testData );
  var prot = BufferFromFile.Protection;
  var buffer = BufferFromFile({ filePath : context.filePath, protection : prot.read | prot.write  }).NodeBuffer();
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
  // var buffer = BufferFromFile({ filePath : context.filePath, protection : BufferFromFile.Protection.none }).NodeBuffer();
  // test.shouldThrowErrorSync( function()
  // {
  //   buffer[ 0 ];
  // })
  // test.shouldThrowErrorSync( function()
  // {
  //   buffer[ 0 ] = 99;
  // })
  // BufferFromFile.unmap( buffer );

  //

  test.description = 'flags';

  /* flags MAP_PRIVATE */

  _.fileProvider.fileWrite( context.filePath, context.testData );
  var buffer = BufferFromFile({ filePath : context.filePath, flag : BufferFromFile.Flag.private }).NodeBuffer();
  buffer[ 0 ] = 55;
  BufferFromFile.flush( buffer );
  var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.node' });
  test.is( buffer !== expected );
  BufferFromFile.unmap( buffer );

  /* flags MAP_SHARED */

  _.fileProvider.fileWrite( context.filePath, context.testData );
  var buffer = BufferFromFile({ filePath : context.filePath, flag : BufferFromFile.Flag.shared }).NodeBuffer();
  buffer[ 0 ] = 55;
  BufferFromFile.flush( buffer );
  var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.node' });
  test.identical( buffer, expected );
  BufferFromFile.unmap( buffer );

  /**/

  if( Config.debug )
  {
    test.description = 'no arguments provided';
    test.shouldThrowErrorSync( function()
    {
      BufferFromFile();
    })

    test.description = 'incorrect argument type';
    test.shouldThrowErrorSync( function()
    {
      BufferFromFile( 1 );
    })

    test.description = 'try to read not existing file';
    _.fileProvider.fileDelete( context.filePath );
    test.shouldThrowErrorSync( function()
    {
      BufferFromFile( filePath );
    })

    test.description = 'incorrect offset';
    _.fileProvider.fileWrite( context.filePath, context.testData );

    test.shouldThrowErrorSync( function()
    {
      BufferFromFile({ filePath : context.filePath, offset : -1 });
    })

    _.fileProvider.fileWrite( context.filePath, context.testData );
    var size = _.fileProvider.statRead( context.filePath ).size;
    test.shouldThrowErrorSync( function()
    {
      BufferFromFile({ filePath : context.filePath, offset : size + 1 });
    })

    test.description = 'incorrect size';
    _.fileProvider.fileWrite( context.filePath, context.testData );

    test.shouldThrowErrorSync( function()
    {
      BufferFromFile({ filePath : context.filePath, size : -2 });
    })

    test.description = 'out of file bounds';
    _.fileProvider.fileWrite( context.filePath, context.testData );
    test.shouldThrowErrorSync( function()
    {
      var size = _.fileProvider.statRead( context.filePath ).size;
      BufferFromFile({ filePath : context.filePath, size : size * 2 });

    })

    test.description = 'out of file bounds, requested size of buffer is bigger than can be returned';
    test.shouldThrowErrorSync( function()
    {
      var size = _.fileProvider.statRead( context.filePath ).size;
      BufferFromFile({ filePath : context.filePath, offset : Math.floor( size / 2 ), size : size });
    })
  }
}

//

function flush( test )
{ 
  let context = this;
  
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
    var data = context.testData;
    var type = buffers[ i ];
    var mod = _.strIsolateLeftOrNone( type, [ '8', '16', '32', '64' ] )[ 1 ] / 8;

    if( mod > 1 )
    while( data.length % mod !== 0 )
    data = data.slice( 0, -1 );

    _.fileProvider.fileWrite( context.filePath, data );
    var buffer = BufferFromFile( context.filePath )[ type ]();
    test.is( _.bufferTypedIs( buffer ) );

    /**/

    test.description = 'BufferFromFile.flush: ' + type + 'buffer as single argument';

    buffer[ 0 ] = 48 + Math.round( Math.random()*9 );
    test.mustNotThrowError( function ()
    {
      BufferFromFile.flush( buffer );
    })
    var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.raw' });
    expected = new buffersMap[ type ]( expected );
    test.identical( buffer, expected );

    test.description = 'BufferFromFile.flush: ' + type + 'buffer as option';

    buffer[ 0 ] = 49 + Math.round( Math.random()*9 );
    test.mustNotThrowError( function ()
    {
      BufferFromFile.flush( { buffer : buffer } );
    })
    var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.raw' });
    expected = new buffersMap[ type ]( expected );
    test.identical( buffer, expected );

    BufferFromFile.unmap( buffer );
  }

  //

  test.description = 'BufferFromFile.flush buffer is NodeBuffer';

  /* as argument */

  _.fileProvider.fileWrite( context.filePath, context.testData );
  var buffer = BufferFromFile( context.filePath ).NodeBuffer();
  test.is( _.bufferNodeIs( buffer ) )
  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 59;
    BufferFromFile.flush( buffer );
  });
  var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.node' });
  test.identical( buffer, expected );

  /* as option */

  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 60;
    BufferFromFile.flush({ buffer : buffer });
  });
  var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.node' });
  test.identical( buffer, expected );

  BufferFromFile.unmap( buffer );

  //

  test.description = 'BufferFromFile.flush buffer is ArrayBuffer';
  _.fileProvider.fileWrite( context.filePath, context.testData );
  var buffer = BufferFromFile( context.filePath ).ArrayBuffer();
  test.is( _.bufferRawIs( buffer ) );

  /* as argument */

  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 60;
    BufferFromFile.flush( buffer );
  });
  var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.raw' });
  test.identical( buffer, expected );

  /* as option */

  test.mustNotThrowError( function ()
  {
    buffer[ 0 ] = 61;
    BufferFromFile.flush({ buffer : buffer });
  });
  var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.raw' });
  test.identical( buffer, expected );

  BufferFromFile.unmap( buffer );

  //

  test.description = 'buffer is mandatory argument';
  test.shouldThrowErrorSync( function ()
  {
    BufferFromFile.flush();
  });

  //

  test.description = 'buffer is mandatory option';
  test.shouldThrowErrorSync( function ()
  {
    BufferFromFile.flush({ sync : BufferFromFile.Sync.sync });
  });

  //

  test.description = 'random buffer';
  test.shouldThrowErrorSync( function ()
  {
    BufferFromFile.flush( new ArrayBuffer( 5 ) );
  });
}

//

function advise( test )
{ 
  let context = this;
  
  var advises = _.mapOwnKeys( BufferFromFile.Advise );
  _.fileProvider.fileWrite( context.filePath, context.testData );

  var buffer = BufferFromFile( context.filePath ).ArrayBuffer();
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
  var buffer = BufferFromFile( context.filePath ).ArrayBuffer();
  var expected = -1;
  BufferFromFile.advise( buffer, -1 );
  var got = BufferFromFile.status( buffer ).advise;
  test.identical( got, expected );
  BufferFromFile.unmap( buffer );

  //

  test.description = 'advise with no arguments';
  test.shouldThrowErrorSync( function ()
  {
    BufferFromFile.advise();
  });

  //

  test.description = 'first argument is no a buffer';
  test.shouldThrowErrorSync( function ()
  {
    BufferFromFile.advise( 1, 1 );
  })

  //
}

//

function status( test )
{ 
  let context = this;
  
  _.fileProvider.fileWrite( context.filePath, context.testData );
  var stats = _.fileProvider.statRead( context.filePath );

  //

  test.description = 'get status';
  var buffer = BufferFromFile( context.filePath ).ArrayBuffer();
  var status = BufferFromFile.status( buffer );
  test.identical( status.filePath, context.filePath );
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
    filePath : context.filePath,
    size : 10,
    offset : 10,
    protection : BufferFromFile.Protection.read,
    flag : BufferFromFile.Flag.private,
    advise : BufferFromFile.Advise.random
  }).ArrayBuffer();

  var status = BufferFromFile.status( buffer );
  test.identical( status.filePath, context.filePath );
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
  test.shouldThrowErrorSync( function ()
  {
    BufferFromFile.status();
  })

  //

  test.description = 'too many args'
  var buffer = BufferFromFile( context.filePath ).NodeBuffer();
  test.shouldThrowErrorSync( function ()
  {
    BufferFromFile.status( buffer, 1, 2 );
  })
  BufferFromFile.unmap( buffer );
}

//

function unmap( test )
{ 
  let context = this;
  
  _.fileProvider.fileWrite( context.filePath, context.testData );
  
  test.description = 'after unmap buffer is empty, changes do not affect the file'
  var buffer = BufferFromFile( context.filePath ).NodeBuffer();
  var expected = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.node' });
  BufferFromFile.unmap( buffer );
  test.identical( buffer.length, 0 );
  buffer[ 0 ] = 101;
  var got = _.fileProvider.fileRead({ filePath : context.filePath, encoding : 'buffer.node' });
  test.identical( got, expected );

  //

  test.description = 'unmap, no args'
  test.shouldThrowErrorSync( function ()
  {
    BufferFromFile.unmap();
  })

  //

  test.description = 'some random buffer passed';
  test.shouldThrowErrorSync( function ()
  {
    BufferFromFile.unmap( new ArrayBuffer(5) );
  })

  //
  
  test.description = 'too many args'
  var buffer = BufferFromFile( context.filePath ).NodeBuffer();
  test.shouldThrowErrorSync( function ()
  {
    BufferFromFile.unmap( buffer, 1, 2 );
  })
  BufferFromFile.unmap( buffer );
}

//

function readOnlyBuffer( test )
{
  let context = this;
  let a = test.assetFor( false );
  
  let _BufferFromFilePath_ = a.path.nativize( require.resolve( '../js/Main.ss' ) );
  
  let program1Path = a.program({ routine : program1, locals : { _BufferFromFilePath_, _FilePath_ : context.filePath } });
  let program2Path = a.program({ routine : program2, locals : { _BufferFromFilePath_, _FilePath_ : context.filePath } });
  
  _.fileProvider.fileWrite( context.filePath, context.testData );
  
  /* */
  
  a.appStartNonThrowing({ execPath : program1Path })
  .then( ( op ) =>
  {
    test.identical( op.exitCode, 0 );
    test.identical( op.exitSignal, null );
    test.is( _.strHas( op.output, '49' ) );
    return null;
  });
  
  /* */
  
  a.appStartNonThrowing({ execPath : program2Path })
  .then( ( op ) =>
  {
    test.notIdentical( op.exitCode, 0 );
    if( process.platform === 'win32' )
    test.identical( op.exitSignal, null );
    else if( process.platform === 'linux' )
    test.identical( op.exitSignal, 'SIGSEGV' );
    else if( process.platform === 'darwin' )
    test.identical( op.exitSignal, 'SIGBUS' );
    
    test.is( !_.strHas( op.output, 'Buffer changed' ) );
    return null;
  });

  
  /* */
  
  return a.ready;
  
  function program1()
  {
    let BufferFromFile = require( _BufferFromFilePath_ )
    var buffer = BufferFromFile({ filePath : _FilePath_, protection : BufferFromFile.Protection.read }).Uint8Array();
    console.log( buffer[ 0 ].toString() )
    BufferFromFile.unmap( buffer );
  }
  
  function program2()
  {
    let BufferFromFile = require( _BufferFromFilePath_ )
    var buffer = BufferFromFile({ filePath : _FilePath_, protection : BufferFromFile.Protection.read }).Uint8Array();
    buffer[ 0 ] = 1;
    console.log( 'Buffer changed' )
    BufferFromFile.unmap( buffer );
  }
}

readOnlyBuffer.timeOut = 30000;

//

function ipc( test )
{
  let context = this;
  
  let a = context.assetFor( test, 'ipc' );
  
  a.reflect();
  
  _.fileProvider.fileWrite( _.path.join( a.routinePath, 'File.txt' ), 'abc' );
  
  var buffer = BufferFromFile( _.path.nativize( _.path.join( a.routinePath, 'File.txt' ) ) ).NodeBuffer();
  buffer.fill( 'a' );
  BufferFromFile.flush( buffer );
  
  let childProgramPath = _.path.join( a.routinePath, 'Child.js' );
  
  var o = 
  { 
    execPath : 'node ' + childProgramPath,
    currentPath : context.suiteTempPath,
    ipc : 1,
    mode : 'spawn'
  }

  /* */

  let ready = _.process.start( o );
  let childBuffer;
  let finalBuffer;
  
  o.process.on( 'message', ( m ) => 
  { 
    if( m.ready === 1 )
    {
      buffer.fill( 'a' );
      BufferFromFile.flush( buffer );
      o.process.send( 'ready' );
    }
    else if( m.childBuffer )
    {
      childBuffer = m.childBuffer;
    }
    else if( m.ready === 2 )
    {
      finalBuffer = buffer.toString();
    }
  })
  
  ready.finally( ( err, op ) =>
  { 
    BufferFromFile.unmap( buffer );
    
    if( err )
    throw err;
    
    test.identical( op.exitCode, 0 );
    test.identical( childBuffer, 'aaa' )
    test.identical( finalBuffer, 'bbb' )
    return null;
  })

  /* */

  return ready;
}

ipc.description = 
`
  Main process sends data to child via buffer, child reads buffer and sends receveived data back via ipc.
  Child process sends data to parent via buffer, parent reads buffer and checks both results.
`

//

function experiment( test )
{  
  let context = this;
  
  var file = _.path.join( testDir, test.name, 'fileA' );
  _.fileProvider.fileWrite( context.filePath, context.testData );
 
  var buffer = BufferFromFile( filePath ).ArrayBuffer();
  BufferFromFile.unmap( buffer );
  test.identical( 1,1 );

  return _.timeOut( 1000, () =>
  { 
    var buffer = BufferFromFile( filePath ).ArrayBuffer();
    BufferFromFile.unmap( buffer );
    test.identical( 1,1 );
  })

}

experiment.experimental = 1;

// --
// proto
// --

var Proto =
{

  name : 'BufferFromFile',

  silencing : 1,

  onSuiteBegin,
  onSuiteEnd,
  
  context : 
  {
    suiteTempPath : null,
    assetsOriginalSuitePath : null,
    appJsPath : null,
    filePath : null,
    testData : null,
    bufferFromFilePath : null,
    toolsPath : null,
    assetFor
  },

  tests :
  {
    buffersFromRaw,
    bufferFromFile,
    flush,
    advise,
    status,
    unmap,
    
    readOnlyBuffer,
    
    ipc,

    experiment
  },

}


var Self = new wTestSuite( Proto )
if( typeof module !== 'undefined' && !module.parent )
wTester.test( Self.name );

} )( );
