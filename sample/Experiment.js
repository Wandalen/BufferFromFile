
var BufferFromFile = require( '..' );
var File = require( 'fs' );
var _ = require( 'wTools' );

console.log( 'BufferFromFile' );
console.log( BufferFromFile );

/* */

var filePath = __dirname + '/TestFile.txt';
var buffer = BufferFromFile( filePath ).Uint8Array();

// var buffer = BufferFromFile.uint8Array
// ({
//   filePath : filePath,
//   size : size,
// });

buffer[ 0 ] = 48 + Math.round( Math.random()*9 );

BufferFromFile.advise( buffer,BufferFromFile.Advise.sequential );
BufferFromFile.unmap( buffer );

// return;

console.log( 'buffer.length :',buffer.length );
console.log( 'buffer.toString :',buffer.toString() );
console.log( 'bufferToStr :',_.bufferToStr( buffer ) );

// var status = BufferFromFile.status( new ArrayBuffer() );
var status = BufferFromFile.status( buffer );
console.log( 'status' );
console.log( status );

// BufferFromFile.flush( buffer.buffer );

setTimeout( function(){},1000 )
