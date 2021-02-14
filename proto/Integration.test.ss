( function _Integration_test_ss_()
{

'use strict';

//

if( typeof module !== 'undefined' )
{
  let _ = require( 'wTools' );
  _.include( 'wTesting' );
}

//

let _ = _globals_.testing.wTools;
let fileProvider = _.fileProvider;
let path = fileProvider.path;

// --
// context
// --

function onSuiteBegin( test )
{
  let context = this;
  context.provider = fileProvider;
  let path = context.provider.path;
  context.suiteTempPath = context.provider.path.tempOpen( path.join( __dirname, '../..'  ), 'integration' );
}

//

function onSuiteEnd( test )
{
  let context = this;
  let path = context.provider.path;
  _.assert( _.strHas( context.suiteTempPath, 'integration' ), context.suiteTempPath );
  path.tempClose( context.suiteTempPath );
}

// --
// test
// --

function production( test )
{
  let context = this;
  let a = test.assetFor( 'production' );
  let runList = [];
  let trigger = _.test.workflowTriggerGet();

  if( trigger === 'pull_request' )
  {
    test.true( true );
    return;
  }

  /* delay to let npm get updated */
  if( trigger === 'publish' )
  a.ready.delay( 60000 );

  console.log( `Event : ${trigger}` );
  console.log( `Env :\n${_.toStr( _.mapBut( process.env, { WTOOLS_BOT_TOKEN : null } ) )}` );

  /* */

  let sampleDir = a.abs( __dirname, '../sample/trivial' );
  let samplePath = a.find
  ({
    filePath : sampleDir,
    filter : { filePath : { 'Sample.(s|js|ss)' : 1 } }
  });

  if( !samplePath.length )
  throw _.err( `Sample with name "Sample.(s|ss|js)" does not exist in directory ${ sampleDir }` );

  /* */

  a.fileProvider.filesReflect({ reflectMap : { [ sampleDir ] : a.abs( 'sample/trivial' ) } });
  let mdlPath = a.abs( __dirname, '../package.json' );
  let mdl = a.fileProvider.fileRead({ filePath : mdlPath, encoding : 'json' });

  let remotePath = null;
  if( _.git.insideRepository( a.abs( __dirname, '..' ) ) )
  remotePath = _.git.remotePathFromLocal( a.abs( __dirname, '..' ) );

  let mdlRepoParsed, remotePathParsed;
  if( remotePath )
  {
    mdlRepoParsed = _.git.path.parse( mdl.repository.url );
    remotePathParsed = _.git.path.parse( remotePath );
  }

  let isFork = mdlRepoParsed.user !== remotePathParsed.user || mdlRepoParsed.repo !== remotePathParsed.repo;

  let version;
  if( isFork )
  version = _.git.path.nativize( remotePath );
  else
  version = _.npm.versionRemoteRetrive( `npm:///${ mdl.name }!alpha` ) === '' ? 'latest' : 'alpha'; /* aaa for Dmytro : ? */
  /*
    Dmytro : it is correct code, the first branch nativizes Git repository path ( forks should use Git path )
    The second branch checks if the npmjs has some defined version of package ( origin should use version on npmjs )

    The routine versionRemoteRetrive returns version of module if it exists, otherwise, the routine returns empty string
    _.npm.versionRemoteRetrive({ remotePath : 'npm:///wTools!alpha' })
    // returns : '0.8.858'
    _.npm.versionRemoteRetrive({ remotePath : 'npm:///wTools!delta' })
    // returns : ''
  */

  if( !version )
  throw _.err( 'Cannot obtain version to install' );

  let structure = { dependencies : { [ mdl.name ] : version } };
  a.fileProvider.fileWrite({ filePath : a.abs( 'package.json' ), data : structure, encoding : 'json' });
  let data = a.fileProvider.fileRead({ filePath : a.abs( 'package.json' ) });
  console.log( data );

  /* */

  a.shell( `npm i --production` )
  .catch( handleDownloadingError )
  .then( ( op ) =>
  {
    test.case = 'install module';
    test.identical( op.exitCode, 0 );
    return null;
  });

  run( 'Sample.s' );
  run( 'Sample.ss' );

  /* */

  return a.ready;

  // /* */
  //
  // function publishIs() /* aaa for Dmytro : lets discuss */ /* Dmytro : the manual checking of triggers is replaced by routine `workflowTriggerGet` */
  // {
  //   if( process.env.GITHUB_WORKFLOW === 'publish' )
  //   return true;
  //
  //   if( process.env.CIRCLECI )
  //   {
  //     let lastCommitLog = a.shell
  //     ({
  //       currentPath : a.abs( __dirname, '..' ),
  //       execPath : 'git log --format=%B -n 1',
  //       sync : 1
  //     });
  //     let commitMsg = lastCommitLog.output;
  //     return _.strBegins( commitMsg, 'version' );
  //   }
  //
  //   return false;
  // }

  /* */

  function run( name )
  {
    let filePath = `sample/trivial/${ name }`;
    if( !a.fileProvider.fileExists( a.abs( filePath ) ) )
    return null;
    runList.push( filePath );
    a.shell( `node ${ filePath }` )
    .then( ( op ) =>
    {
      test.case = `running of sample ${filePath}`;
      test.identical( op.exitCode, 0 );
      test.ge( op.output.length, 3 );
      return null;
    });

  }

  /* */

  function handleDownloadingError( err )
  {
    if( _.strHas( err.message, 'npm ERR! ERROR: Repository not found' ) )
    {
      _.errAttend( err );
      return a.shell( `npm i --production` );
    }
    throw _.err( err );
  }
}

production.timeOut = 300000;

//

function samples( test )
{
  let context = this;
  let ready = _.take( null );

  let sampleDir = path.join( __dirname, '../sample' );

  let appStartNonThrowing = _.process.starter
  ({
    currentPath : sampleDir,
    outputCollecting : 1,
    outputGraying : 1,
    throwingExitCode : 0,
    ready,
    mode : 'fork'
  })

  let found = fileProvider.filesFind
  ({
    filePath : path.join( sampleDir, '**/*.(s|ss)' ),
    withStem : 0,
    withDirs : 0,
    filter :
    {
      maskTransientDirectory : { excludeAny : [ /asset/, /out/ ] }
    },
    mode : 'distinct',
    mandatory : 0,
  });

  /* */

  for( let i = 0 ; i < found.length ; i++ )
  {
    if( _.longHasAny( found[ i ].exts, [ 'browser', 'manual', 'experiment' ] ) )
    continue;

    let startTime;

    ready
    .then( () =>
    {
      test.case = found[ i ].relative;
      startTime = _.time.now();
      return null;
    })

    if( _.longHas( found[ i ].exts, 'throwing' ) )
    {
      appStartNonThrowing({ execPath : found[ i ].relative })
      .then( ( op ) =>
      {
        console.log( _.time.spent( startTime ) );
        test.description = 'nonzero exit code';
        test.notIdentical( op.exitCode, 0 );
        return null;
      })
    }
    else
    {
      appStartNonThrowing({ execPath : found[ i ].relative })
      .then( ( op ) =>
      {
        console.log( _.time.spent( startTime ) );
        test.description = 'good exit code';
        test.identical( op.exitCode, 0 );
        if( op.exitCode )
        return null;
        test.description = 'have no uncaught errors';
        test.identical( _.strCount( op.output, 'ncaught' ), 0 );
        test.identical( _.strCount( op.output, 'uncaught error' ), 0 );
        test.description = 'have some output';
        test.ge( op.output.split( '\n' ).length, 1 );
        test.ge( op.output.length, 3 );
        return null;
      })
    }
  }

  /* */

  return ready;
}

samples.rapidity = -1;

//

function eslint( test )
{
  let context = this;
  let rootPath = path.join( __dirname, '..' );
  let eslint = process.platform === 'win32' ? 'node_modules/eslint/bin/eslint' : 'node_modules/.bin/eslint';
  eslint = path.join( rootPath, eslint );
  let sampleDir = path.join( rootPath, 'sample' );
  let ready = _.take( null );

  if( _.process.insideTestContainer() )
  {
    let validPlatform = process.platform === 'linux';
    let validVersion = process.versions.node.split( '.' )[ 0 ] === '14';
    if( !validPlatform || !validVersion )
    {
      test.true( true );
      return;
    }
  }

  let start = _.process.starter
  ({
    execPath : eslint,
    mode : 'fork',
    currentPath : rootPath,
    args :
    [
      '-c', '.eslintrc.yml',
      '--ext', '.js,.s,.ss',
      '--ignore-pattern', '*.c',
      '--ignore-pattern', '*.ts',
      '--ignore-pattern', '*.html',
      '--ignore-pattern', '*.txt',
      '--ignore-pattern', '*.png',
      '--ignore-pattern', '*.json',
      '--ignore-pattern', '*.yml',
      '--ignore-pattern', '*.yaml',
      '--ignore-pattern', '*.md',
      '--ignore-pattern', '*.xml',
      '--ignore-pattern', '*.css',
      '--ignore-pattern', '_asset',
      '--ignore-pattern', 'out',
      '--ignore-pattern', '*.tgs',
      '--ignore-pattern', '*.bat',
      '--ignore-pattern', '*.sh',
      '--ignore-pattern', '*.jslike',
      '--ignore-pattern', '*.less',
      '--ignore-pattern', '*.hbs',
      '--ignore-pattern', '*.noeslint',
      '--quiet'
    ],
    throwingExitCode : 0,
    outputCollecting : 1,
  })

  /* */

  ready.then( () =>
  {
    test.case = 'eslint proto';
    return start( 'proto/**' );
  })
  .then( ( op ) =>
  {
    test.identical( op.exitCode, 0 ); debugger;
    if( op.output.length < 1000 )
    logger.log( op.output );
    return null;
  })

  /* */

  if( fileProvider.fileExists( sampleDir ) )
  ready.then( () =>
  {
    test.case = 'eslint samples';
    return start( 'sample/**' )
    .then( ( op ) =>
    {
      test.identical( op.exitCode, 0 );
      if( op.output.length < 1000 )
      logger.log( op.output );
      return null;
    })
  })

  /**/

  return ready;
}

eslint.rapidity = -2;

// --
// declare
// --

let Self =
{

  name : 'Integration',
  routineTimeOut : 1500000,
  silencing : 0,

  onSuiteBegin,
  onSuiteEnd,
  context :
  {
    provider : null,
    suiteTempPath : null,
    appJsPath : null
  },

  tests :
  {
    production,
    samples,
    eslint,
  },

}

//

Self = wTestSuite( Self );
if( typeof module !== 'undefined' && !module.parent )
_global_.wTester.test( Self.name );

})();

