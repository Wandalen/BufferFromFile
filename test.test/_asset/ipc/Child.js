var _ = require( 'wTools' );
_.include( 'wPathBasic' )
_.include( 'wConsequence' )

var BufferFromFile = require( 'bufferFromFile' );
var buffer = BufferFromFile( _.path.nativize( _.path.join( __dirname, 'File.txt' ) ) ).NodeBuffer();

process.send( { ready : 1 } );

process.on( 'message', () => 
{  
  process.send( { childBuffer : buffer.toString() } )
  buffer.fill( 'b' );
  BufferFromFile.flush( buffer );
  process.send( { ready : 2 } )
  BufferFromFile.unmap( buffer );
  process.exit( 0 )
})


_.time.out( 10000, () => 
{ 
  BufferFromFile.unmap( buffer );
  process.exit( -1 )
})
