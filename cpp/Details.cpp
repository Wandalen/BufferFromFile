
// #include <nan.h>
#include <errno.h>
#include <string>
#include <future>
#include "IncludeNode.hpp"

#ifdef _WIN32
  #include <windows.h>
  #include "WinMmap.hpp"
#else
  #include <unistd.h>
  #include <sys/mman.h>
#endif

#include <iostream>

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
  return sysinfo.dwAllocationGranularity;
#else
  return sysconf( _SC_PAGESIZE );
#endif

}

//

wTypedBuffer<> fileMap( off_t offset, size_t size, uv_os_fd_t fd, int protection, int flag )
{
  wTypedBuffer<> result;

  size_t pageSize = pageSizeGet();
  size_t _size = 0;
  off_t _offset = offset;
  off_t offsetDiff = 0;

  if( ( size_t ) offset < pageSize )
  {
    _offset = 0;
    offsetDiff = offset;
  }

  if( ( size_t )offset > pageSize )
  if( offset % pageSize != 0 )
  {
    _offset = ( offset / pageSize ) * pageSize;
    offsetDiff = offset - _offset;
  }

  _size = offsetDiff + size;

  // std::cout << "offsetDiff: " << offsetDiff
  //  << "size: " << _size
  //  << "offset: " << _offset
  //  << std::endl;

  void* r = mmap( NULL, _size, protection, flag, fd, _offset );

  if (r != MAP_FAILED)
  {
  	if ( offsetDiff > 0 )
  	r = ( char* )r + offsetDiff;
  	result.use( ( char* )r, size );
  }

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
  munmap( memory.buffer.data(), memory.buffer.length() );

 if( memory.file.result >= 0 )
 {
    #ifdef _WIN32
    CloseHandle( memory.file.data );
    #endif
    uv_fs_req_cleanup( &memory.file );
 }

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

  #ifdef _WIN32

  DWORD dwDesiredAccess = 0;

  if ( protection & PROT_READ )
  dwDesiredAccess |= GENERIC_READ;

  if (protection & PROT_WRITE)
  {
	dwDesiredAccess |= GENERIC_READ;
	dwDesiredAccess |= GENERIC_WRITE;
  }

  if (protection & PROT_EXEC)
  {
	 dwDesiredAccess |= GENERIC_READ;
	 dwDesiredAccess |= GENERIC_EXECUTE;
  }

  HANDLE fh = CreateFileA( path.c_str(), dwDesiredAccess, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
  if( fh == INVALID_HANDLE_VALUE  )
  {
    result->result = -1;
  }
  else
  {
    uv_fs_stat( uv_default_loop(), result, path.c_str(), NULL);
    result->data = fh;
    result->result = 1;
  }

  #else

  if( protection & PROT_WRITE )
  protection = O_RDWR;
  else
  protection = O_RDONLY;

  int fd = uv_fs_open(uv_default_loop(), result, path.c_str(), protection, 0, NULL);

  assert_M(fd == result->result);

  uv_fs_fstat(uv_default_loop(), result, fd, NULL);

  // cout << "fd : " << fd << endl;
  // cout << "result->result : " << result->result << endl;

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  result->result = fd;

  // cout << "fd : " << fd << endl;
  // cout << "result->result : " << result->result << endl;

  assert_M(fd == result->result);

  #endif

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

inline
Memory*
memoryOf( Local< Object > src, LocalString name )
{
  // return (Memory*)NULL;

  auto srcBuf = src->Get( name );

  if( srcBuf->IsTypedArray() )
  srcBuf = Local< TypedArray >::Cast( srcBuf )->Buffer();

  Local< ArrayBuffer > buffer;
  if( !toNative( srcBuf,buffer ) )
  return (Memory*)NULL;

  if( buffer->InternalFieldCount() < 1 )
  return (Memory*)NULL;

  Memory& memory = *(Memory*)buffer->GetAlignedPointerFromInternalField( 0 );

  src->Set( name, srcBuf );

  // cout << "memoryOf : " << &memory << endl;

  return &memory;
}

//

#include "Body.end.cpp"
