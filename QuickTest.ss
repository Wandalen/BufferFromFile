( function _QuickTest_() {

  let BufferFromFile = require( './js/Main.ss' );
  var buffer = BufferFromFile( __filename ).Uint8Array();
  BufferFromFile.unmap( buffer )
})();
