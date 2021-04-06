var _ = require('wTools')
_.include( 'wLooker' );

let src = 
[
  { name : 'node.exe', pid : 1, ppid : 0 },
  { name : 'node.exe', pid : 2, ppid : 1 },
  { name : 'node.exe', pid : 3, ppid : 1 },
]

src[ Symbol.iterator ] = _iterate;

function _iterate()
{
  debugger
  let iterator = Object.create( null );
  iterator.next = next;
  iterator.index = 0;
  iterator.instance = this;
  return iterator;

  function next()
  {
    debugger

    let result = Object.create( null );
    result.done = this.index === this.instance.length;
    if( result.done )
    return result;
    result.value = this.instance[ this.index ];
    this.index += 1;
    return result;
  }

}

_.look({ src, onUp : handleUp1 });

function handleUp1( e, k, it )
{
  debugger
}