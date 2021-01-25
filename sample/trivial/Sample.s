var BufferFromFile = require( 'bufferfromfile' );

// mmap file

var filePath = __dirname + '/../TestFile.txt';
var buffer = BufferFromFile( filePath ).NodeBuffer();

console.log( 'buffer.length :', buffer.length );
console.log( 'buffer.toString :', buffer.toString() );

// update buffer data

buffer[ 0 ] = 48;

// flush written data to make sure data on will get on hard drive shortly

BufferFromFile.flush( buffer );

// unmap file

BufferFromFile.unmap( buffer );

console.log( '\nTry : cat sample/TestFile.txt' );
