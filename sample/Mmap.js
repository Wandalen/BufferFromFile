// Sample simply mmap a file

try
{
  var BufferFromFile = require( 'bufferfromfile' );
}
catch( err )
{
  var BufferFromFile = require( '..' );
}

// mmap file to buffer

var filePath = __dirname + '/TestFile.txt';
var buffer = BufferFromFile( filePath ).Uint8Array();

// edit single byte

buffer[ 0 ] = 48 + Math.round( Math.random()*9 );

// print the buffer

console.log( 'buffer.length :',buffer.length );
console.log( 'buffer.toString :',buffer.toString() );

// unmap file

BufferFromFile.unmap( buffer );

console.log( "\nTry : cat sample/TestFile.txt" );
