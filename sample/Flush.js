// Sample flush written data to hard drive

var BufferFromFile = require( '..' );

// mmap file

var filePath = __dirname + '/TestFile.txt';
var buffer = BufferFromFile( filePath ).Uint8Array();

// write data

buffer[ 0 ] = 48 + Math.round( Math.random()*9 );

// flush written data to make sure data on will get on hard drive shortly

BufferFromFile.flush( buffer );

// rewrite data

buffer[ 0 ] = 48 + Math.round( Math.random()*9 );

// unmap file

BufferFromFile.unmap( buffer );

console.log( "\nTry : cat sample/TestFile.txt" );
