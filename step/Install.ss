#!/usr/bin/env node
( function _Install_() {
  
  'use strict';
  
  let ChildProcess = require( 'child_process' );
  let path = require( 'path' );
  
  let o = 
  { 
    stdio: 'inherit', 
    cwd : path.join( __dirname, '..' ),
  }
  let npm = process.platform === 'win32' ? 'npm.cmd' : 'npm';
  
  install();
  
  /* */

  function install()
  {
    let pnd = ChildProcess.spawn( npm, [ 'run', 'node-pre-gyp-install' ], o );
    
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
    let pnd = ChildProcess.spawn( npm, [ 'run', 'node-pre-gyp-build' ], o )
    
    pnd.on( 'exit', function( exitCode ) 
    {
      if( exitCode === 0 )
      return;
      console.error( 'Build failed' );
      return process.exit( exitCode );
    });
  }
})();
