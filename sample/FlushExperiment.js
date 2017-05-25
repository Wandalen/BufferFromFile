// Sample flush written data to hard drive

try
{
  var BufferFromFile = require( 'bufferfromfile' );
}
catch( err )
{
  var BufferFromFile = require( '..' );
}

// mmap file

var filePath = __dirname + '/TestFile.txt';
var buffer = BufferFromFile( filePath );
var arrayBuffer = buffer.Uint8Array();

arrayBuffer[ 0 ] = 48 + Math.round( Math.random()*9 );

BufferFromFile.flush( arrayBuffer );

console.log( "OK" );

arrayBuffer[ 0 ] = 48 + Math.round( Math.random()*9 );

BufferFromFile.flush({ buffer : arrayBuffer } );

console.log( "OK" );
