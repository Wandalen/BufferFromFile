// Sample make read only buffer from a file

var BufferFromFile = require( '..' );

// mmap file

var filePath = __dirname + '/TestFile.txt';
var buffer = BufferFromFile({ filePath : filePath, protection : BufferFromFile.Protection.read }).Uint8Array();

// attempt to write data would give error

/* buffer[ 0 ] = 48 + Math.round( Math.random()*9 ); */

// read data and status

console.log( 'buffer.toString :',buffer.toString() );
console.log( 'status',BufferFromFile.status( buffer ) );

// unmap file

BufferFromFile.unmap( buffer );
