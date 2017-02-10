#ifndef _WinMmap_hpp_
#define _WinMmap_hpp_

#include <io.h>
#include <errno.h>
#include <windows.h>
#include <sys/types.h>

//

#define PROT_NONE         0x00
#define PROT_READ         0x01
#define PROT_WRITE        0x02
#define PROT_EXEC         0x04

#define MAP_FILE          0x00
#define MAP_SHARED        0x01
#define MAP_PRIVATE       0x02
#define MAP_TYPE          0x0F
#define MAP_ANONYMOUS     0x20
#define MAP_FAILED        ( ( void* ) -1 )

#define MS_ASYNC          0x01
#define MS_SYNC           0x02
#define MS_INVALIDATE     0x04

#define MADV_NORMAL       0x00
#define MADV_RANDOM       0x01
#define MADV_SEQUENTIAL   0x02
#define MADV_WILLNEED     0x03
#define MADV_DONTNEED     0x04

//

inline void* mmap( void* addr, size_t length, int protection, int flags, int fd, off_t offset )
{

  /* anonymous mmap */

  if( fd < 0 )
  {
    if( !( flags & MAP_ANONYMOUS ) || offset )
    return MAP_FAILED;
  }
  else if( flags & MAP_ANONYMOUS )
  {
    return MAP_FAILED;
  }

  /* protection */

  DWORD wProtection;

  if( protection & ~( PROT_READ | PROT_WRITE | PROT_EXEC ) )
  return MAP_FAILED;

  if( protection & PROT_WRITE )
  {
    if( protection & PROT_EXEC )
    wProtection = PAGE_EXECUTE_READWRITE;
    else
    wProtection = PAGE_READWRITE;
  }
  else if( protection & PROT_EXEC )
  {
    if( protection & PROT_READ )
    wProtection = PAGE_EXECUTE_READ;
    else
    wProtection = PAGE_EXECUTE;
  }
  else
  {
    wProtection = PAGE_READONLY;
  }

  /* microsoft mess */

  off_t end = length + offset;
  const DWORD dwEndLow = ( sizeof( off_t ) > sizeof( DWORD ) ) ? DWORD( end & 0xFFFFFFFFL ) : DWORD( end );
  const DWORD dwEndHigh = ( sizeof( off_t ) > sizeof( DWORD ) ) ? DWORD( end & 0xFFFFFFFFL ) : DWORD( 0 );
  const DWORD dwOffsetLow = ( sizeof( off_t ) > sizeof( DWORD ) ) ? DWORD( offset & 0xFFFFFFFFL ) : DWORD( offset );
  const DWORD dwOffsetHigh = ( sizeof( off_t ) > sizeof( DWORD ) ) ? DWORD( offset & 0xFFFFFFFFL ) : DWORD( 0 );

  HANDLE h = ( fd < 0 ) ? HANDLE( _get_osfhandle( fd ) ) : INVALID_HANDLE_VALUE;

  HANDLE fm = CreateFileMapping( h, NULL, wProtection, dwEndHigh, dwEndLow, NULL );
  if( fm == NULL )
  return MAP_FAILED;

  /* desired access */

  DWORD dwDesiredAccess;
  if( protection & PROT_WRITE )
  dwDesiredAccess = FILE_MAP_WRITE;
  else
  dwDesiredAccess = FILE_MAP_READ;
  if( protection & PROT_EXEC )
  dwDesiredAccess |= FILE_MAP_EXECUTE;
  if( flags & MAP_PRIVATE )
  dwDesiredAccess |= FILE_MAP_COPY;

  /* map */

  void* map = MapViewOfFile( fm, dwDesiredAccess, dwOffsetHigh, dwOffsetLow, length );

  /* */

  CloseHandle( fm );

  return ( map != nullptr ) ? map : MAP_FAILED;
}

//

inline int munmap( void* addr, size_t length )
{
  if( UnmapViewOfFile( addr ) )
  return 0;

  errno = GetLastError(  );
  return -1;
}

//

inline int msync(  void* addr, size_t length, int flags )
{
  if( FlushViewOfFile( addr, length ) )
  return 0;

  errno = GetLastError(  );
  return -1;
}

//

inline int madvise( void* addr, size_t length, int advice )
{
  return 0;
}

#endif // _WinMmap_hpp_
