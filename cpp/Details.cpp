
// #include <nan.h>
#include <errno.h>
#include <string>
#include <future>

#ifdef _WIN32
  #include <windows.h>
  #include "WinMmap.hpp"
#else
  #include <unistd.h>
  #include <sys/mman.h>
#endif

#include <iostream>

#include "IncludeNode.hpp"
#include "Tools.hpp"
#include "wTools/OptionalTuple.hpp"
#include "wTools/OptionalTuple.test.hpp"

#include "Body.begin.cpp"

using namespace ::std;
using namespace ::wTools;
using namespace ::wTools::v8;

//

struct Memory
{

  Memory(){};
  Memory( char* data, size_t size )
  {
    buffer.use( data,size );
  }

  uv_fs_t file;
  wTypedBuffer<> buffer;
  Persistent< ArrayBuffer > arrayBuffer;

  std::string filePath;
  Int8 offset;
  Int8 size;
  Int4 protection;
  Int4 flag;
  Int4 advise;

};

//

size_t pageSizeGet()
{

#ifdef _WIN32
  SYSTEM_INFO sysinfo;
  GetSystemInfo( &sysinfo );
  return sysinfo.dwPageSize;
#else
  return sysconf( _SC_PAGESIZE );
#endif

}

//

wTypedBuffer<> fileMap( off_t offset, size_t size, int fd, int protection, int flag )
{
  wTypedBuffer<> result;

  void* r = mmap( NULL, size, protection, flag, fd, offset );

  if( r != MAP_FAILED )
  result.use( r,size );

  // char* data = static_cast< char* >( mmap( NULL, size, protection, flag, memory.file.result, offset ) );

  return result;
}

//

void fileUnmap( Memory& memory )
{
  // cout << "fileUnmap" << endl;

  if( !memory.arrayBuffer.IsEmpty() )
  {
    Isolate* isolate = Isolate::GetCurrent();
    Local< ArrayBuffer > arrayBuffer = memory.arrayBuffer.Get( isolate );
    arrayBuffer->SetAlignedPointerInInternalField( 0,NULL );
    arrayBuffer->Neuter();
    memory.arrayBuffer.Reset();
  }

  if( memory.buffer.data() != NULL )
  munmap( memory.buffer.data(), memory.buffer.size() );
  uv_fs_req_cleanup( &memory.file );
  delete &memory;
}

//

void fileUnmap( char* data, void* _memory )
{
  auto& memory = *static_cast< Memory* >( _memory );
  assert_M( memory.buffer.data() == data );
  fileUnmap( memory );
}

//

// template< typename T >
// void fileUnmap( const WeakCallbackInfo<T>& data )
// {
//   cout << "fileUnmap" << endl;
// }

//

uv_fs_t& _fileOpen( uv_fs_t& req, const string& path, int protection = O_RDWR )
{
  uv_fs_t* result;

  if( &req == NULL )
  result = new uv_fs_t;
  else
  result = &req;

  int fd = uv_fs_open( uv_default_loop(), result, path.c_str(), protection, 0, NULL );

  // cout << "fd : " << fd << endl;
  // cout << "result->result : " << result->result << endl;

  assert_M( fd == result->result );

  uv_fs_fstat( uv_default_loop(), result, fd, NULL );

  uv_run( uv_default_loop(),UV_RUN_DEFAULT );

  result->result = fd;

  // cout << "fd : " << fd << endl;
  // cout << "result->result : " << result->result << endl;

  assert_M( fd == result->result );

  return ( *result );
}

//

inline
Memory*
memoryOf( Local< Value > src )
{
  // return (Memory*)NULL;

  if( src->IsTypedArray() )
  src = Local< TypedArray >::Cast( src )->Buffer();

  Local< ArrayBuffer > buffer;
  if( !toNative( src,buffer ) )
  return (Memory*)NULL;

  if( buffer->InternalFieldCount() < 1 )
  return (Memory*)NULL;

  Memory& memory = *(Memory*)buffer->GetAlignedPointerFromInternalField( 0 );

  // cout << "memoryOf : " << &memory << endl;

  return &memory;
}

//

#include "Body.end.cpp"
