#!/usr/bin/env node
( function _Install_() {
  
  'use strict';
  
  let ChildProcess = require( 'child_process' );
  let path = require( 'path' );
  
  let o = 
  { 
    stdio: 'inherit',
    cwd : path.join( __dirname, '..' ),
    shell : true
  }

  let isWin32 = process.platform === 'win32';
  let nodePreGyp = path.resolve( __dirname, '../node_modules/@mapbox/node-pre-gyp/bin/node-pre-gyp' );
  if( isWin32 )
  nodePreGyp = `${nodePreGyp}.cmd`;

  nodePreGyp = strQuote( nodePreGyp );
  
  install();
  
  /* */

  function install()
  {
    let pnd = ChildProcess.spawn( nodePreGyp, [ 'install', '--update-binary', '--fallback-to-build' ], o );
    
    pnd.on( 'exit', ( exitCode ) =>
    {
      if( exitCode !== 0 )
      return process.exit( exitCode );
      test();
    });
  }
  
  /* */
  
  function test()
  {
    let pnd = ChildProcess.fork( path.join( __dirname, '../QuickTest.ss' ), [], o );
    
    pnd.on( 'exit', ( exitCode ) =>
    {
      if( exitCode !== 0 )
      {
        console.error( 'Problem with the binary; manual build incoming' );
        build();
      }
      else
      {
        console.log( 'Binary is fine; exiting' );
      }
    });
  }
  
  /* */
  
  function build() 
  {
    let pnd = ChildProcess.spawn( `${nodePreGyp} configure && ${nodePreGyp} rebuild`, [], o )
    
    pnd.on( 'exit', function( exitCode ) 
    {
      if( exitCode === 0 )
      return;
      console.error( 'Build failed' );
      return process.exit( exitCode );
    });
  }

  function strQuote( src )
  {
    return `"${src}"`;
  }

})();
