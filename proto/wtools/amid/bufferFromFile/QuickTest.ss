( function _QuickTest_()
{
  let BufferFromFile = require( './Main.ss' );
  let buffer = BufferFromFile( __filename ).Uint8Array();
  BufferFromFile.unmap( buffer );
})();
