
if( typeof module !== 'undefined' )
{

  if( typeof _global_ === 'undefined' || !Object.hasOwnProperty.call( _global_, 'wBase' ) )
  {
    let toolsPath = './abase/Layer1.s';
    let toolsExternal = 0;
    try
    {
      toolsPath = require.resolve( toolsPath );
    }
    catch( err )
    {
      toolsExternal = 1;
      require( 'wTools' );
    }
    if( !toolsExternal )
    require( toolsPath );
  }

  module[ 'exports' ] = _global_.wTools;

}
