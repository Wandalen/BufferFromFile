#ifndef _Main_cpp_ //
#define _Main_cpp_

#include "Details.cpp"

#include "../.module/NjsCppBasic/cpp/Body.begin.cpp"

//

void mmap_js( const FunctionCallbackInfo< Value >& info )
{
  const char* routineName = "mmap";

  Isolate* isolate = info.GetIsolate();
  // Local< Context > context = isolate->GetCurrentContext();
  Local< Object > o;

  argsExpects_M( 1,1 );

  if( !info[ 0 ]->IsString() && !info[ 0 ]->IsObject() )
  return ::wTools::v8::errThrow( "Expects String ( filePath ) or Object ( options )" );

  if( info[ 0 ]->IsString() )
  {
    o = Object::New( isolate );
    ( void )o->Set( ::wTools::v8::Isolate::GetCurrent()->GetCurrentContext(), vstr( "filePath" ),info[ 0 ] );
  }
  else
  {
    o = info[ 0 ]->ToObject( ::wTools::v8::Isolate::GetCurrent()->GetCurrentContext() ).ToLocalChecked();
  }

  // ArgumentsOf
  // <
  //   std::string, /* filePath */
  //   size_t,
  //   int,
  //   int,
  //   OptionalWithDefault< off_t,0 >,
  //   OptionalWithDefault< int,0 >
  // >
  // args( routineName );
  // args.from( info );
  //
  // if( !args.good )
  // return;

  // const auto& filePath = args.element< 0 >();
  // const auto& size = args.element< 1 >();
  // const auto& protection = args.element< 2 >();
  // const auto& flag  = args.element< 3 >();
  // const auto& offset = args.element< 4 >();
  // const auto& advise = args.element< 5 >();

  vOption_M( std::string, filePath );

  vOptionOptional_M( Int8, offset, 0 );
  vOptionOptional_M( Int8, size, -1 );

  vOptionOptional_M( int, protection, PROT_READ | PROT_WRITE );
  vOptionOptional_M( int, flag, ( protection & PROT_WRITE ) ? MAP_SHARED : MAP_PRIVATE );
  vOptionOptional_M( int, advise, MADV_NORMAL );

  // vOptionOptional2_M( IsString, xxx, "xxx" );
  // std::cout << xxx->IsString();

  /* */

  Memory& memory = *new Memory();

  _fileOpen( memory.file, filePath, protection );

  if (memory.file.result <= 0)
  {
	  fileUnmap( memory );
	  return ::wTools::v8::errThrow( "Failed open file, ", filePath );
  }

  uint64_t fileSize = memory.file.statbuf.st_size;

  if (size == -1)
  {
	 size = fileSize - (uint64_t)offset;
	 if (size < 0 )
	 {
    fileUnmap( memory );
		return ::wTools::v8::errThrow( "Incorrect offset value." );
	 }
  }

  if( size < 0 || offset < 0 )
  {
    fileUnmap( memory );
    return ::wTools::v8::errThrow( "Routine expects positive value of size/offset property." );
  }


  if( fileSize - offset < (uint64_t) size || ( uint64_t ) offset > fileSize )
  {
    fileUnmap( memory );
    return ::wTools::v8::errThrow( "Requested bytes range goes beyond the end of a file, please provide lower size/offset values." );
  }

  // if( ( uint64_t ) offset % pageSizeGet() != 0 )
  // {
  //   ::wTools::v8::errThrow( "Offset: ", offset, " must be a multiple of the page size: ", pageSizeGet() );
  //   return;
  // }

  #ifdef _WIN32
  uv_os_fd_t fd = memory.file.data;
  #else
  uv_os_fd_t fd = memory.file.result;
  #endif

  memory.buffer = fileMap( offset, size, fd, protection, flag );

  if( memory.buffer.length() != ( long )size )
  {
    ::wTools::v8::errThrow
    (
      "BufferFromFile : mmap failed\n",
      "offset : ",offset,"\n",
      "size : ",size,"\n",
      "fileHandle : ",memory.file.result,"\n",
      "protection : ",protection,"\n",
      "flag : ",flag,"\n"
    );
    fileUnmap( memory );
    return;
  }

  #if NODE_VERSION_AT_LEAST( 14, 0, 0 )
  std::unique_ptr< BackingStore > backing;
  backing = ArrayBuffer::NewBackingStore( memory.buffer.data(), memory.buffer.length(), [](void*, size_t, void*){}, nullptr );
  auto arrayBuffer = ArrayBuffer::New( isolate, std::move( backing ) );
  #else
  auto arrayBuffer = ArrayBuffer::New( isolate,memory.buffer.data(),memory.buffer.length() );
  #endif
  
  // Persistent< ArrayBuffer > buffer( isolate,_buffer );
  // buffer.SetWeak( &memory,fileUnmap,::v8::WeakCallbackType::kParameter );

  if( arrayBuffer.IsEmpty() )
  {
    fileUnmap( memory );
    return ::wTools::v8::errThrow( "Cant allocate buffer for mapped file" );
  }

  arrayBuffer->SetAlignedPointerInInternalField( 0,&memory );

  memory.arrayBuffer.Reset( isolate,arrayBuffer );

  memory.filePath = filePath;
  memory.offset = offset;
  memory.size = size;
  memory.protection = protection;
  memory.flag = flag;
  memory.advise = advise;

  info.GetReturnValue().Set( arrayBuffer );

  /* */

  if( advise != MADV_NORMAL )
  madvise( memory.buffer.data(), size, advise );

  // cout << "filePath : " << memory.filePath << endl;

}

//

void unmap_js( const FunctionCallbackInfo< Value >& info )
{
  const char* routineName = "unmap";

  //Isolate* isolate = info.GetIsolate();
  // Local< Context > context = isolate->GetCurrentContext();

  argsExpects_M( 1,1 );

  /* */

  Memory& memory = *memoryOf( info[ 0 ] );
  if( &memory == NULL )
  return ::wTools::v8::errThrow( "Routine ",routineName," expects mandatory argument ( buffer ) made by ArrayFromFile" );

  /* */

  fileUnmap( memory );

}

//

void status_js( const FunctionCallbackInfo< Value >& info )
{
  const char* routineName = "status";

  Isolate* isolate = info.GetIsolate();
  // Local< Context > context = isolate->GetCurrentContext();

  argsExpects_M( 1,1 );

  /* */

  Memory& memory = *memoryOf( info[ 0 ] );

  if( &memory == NULL )
  return;

  /* */

  Local< Object > result = Object::New( isolate );
  auto fieldsOfResult = FieldsOf( result );
  fieldsOfResult.access = fieldsOfResult.Regular;

  fieldsOfResult
  .set( "filePath",memory.filePath )
  .set( "fileHandle",memory.file.result )
  .set( "offset",memory.offset )
  .set( "size",memory.size )
  .set( "protection",memory.protection )
  .set( "flag",memory.flag )
  .set( "advise",memory.advise )
  ;

  /* */

  info.GetReturnValue().Set( result );
}

//

void advise_js( const FunctionCallbackInfo< Value >& info )
{
  const char* routineName = "advise";

  // Isolate* isolate = info.GetIsolate();
  // Local< Context > context = isolate->GetCurrentContext();

  argsExpects_M( 2,2 );

  /* */

  Memory& memory = *memoryOf( info[ 0 ] );
  if( &memory == NULL )
  return ::wTools::v8::errThrow( "Routine ",routineName," expects mandatory argument ( buffer ) made by ArrayFromFile" );

  Int4 advise;
  if( !toNative( info[ 1 ],advise ) )
  return ::wTools::v8::errThrow( "Routine ",routineName," expects second mandatory argument ( advise ) of type int" );

  /* */

  int result = madvise( memory.buffer.data(), memory.buffer.length(), advise );

  if( result < 0 )
  memory.advise = -1;
  else
  memory.advise = advise;

}

//

void flush_js( const FunctionCallbackInfo< Value >& info )
{
  const char* routineName = "flush";

  Isolate* isolate = info.GetIsolate();
  // Local< Context > context = isolate->GetCurrentContext();
  Local< Object > o;

  argsExpects_M( 1,1 );

  if( !info[ 0 ]->IsArrayBuffer() && !info[ 0 ]->IsObject() )
  return ::wTools::v8::errThrow( "Expects ArrayBuffer ( buffer ) or Object ( options )" );

  if( info[ 0 ]->IsArrayBuffer() )
  {
    o = Object::New( isolate );
    ( void )o->Set( ::wTools::v8::Isolate::GetCurrent()->GetCurrentContext(), vstr( "buffer" ),info[ 0 ] );
  }
  else
  {
    o = info[ 0 ]->ToObject( ::wTools::v8::Isolate::GetCurrent()->GetCurrentContext() ).ToLocalChecked();
  }

  /* arguments */

  // ArgumentsOf
  // <
  //   wTypedBuffer< char* >,
  //   int,
  //   size_t,
  //   bool,
  //   bool
  // >
  // args( info );
  //
  // auto buffer = args.element< 0 >();
  // auto offset = args.element< 1 >();
  // auto size = args.element< 2 >();
  // auto sync = args.element< 3 >();
  // auto invalidate = args.element< 4 >();

  // auto buffer = args.element< 0 >();
  // auto offset = args.element< 1 >();
  // auto size = args.element< 2 >();
  // auto sync = args.element< 3 >();
  // auto invalidate = args.element< 4 >();

  // return;

  Memory& memory = *memoryOf( o, vstr( "buffer" ) );

  if( &memory == NULL )
  return ::wTools::v8::errThrow( "Routine ",routineName," expects mandatory argument ( buffer ) made by ArrayFromFile" );

  vOption_M( Local< ArrayBuffer >, buffer );

  vOptionOptional_M( Int8, offset, 0 );
  vOptionOptional_M( Int8, size, -1 );

  if( size == -1 )
  size = memory.buffer.length();

  vOptionOptional_M( bool, sync, true );
  vOptionOptional_M( bool, invalidate, false );

  /* */

  int flag = ( ( sync ? MS_SYNC : MS_ASYNC ) | ( invalidate ? MS_INVALIDATE : 0 ) );
  #if NODE_VERSION_AT_LEAST( 14, 0, 0 )
  int ret = msync( ( (char*)buffer->GetBackingStore()->Data() ) + offset, size, flag );
  #else
  int ret = msync( ( (char*)buffer->GetContents().Data() ) + offset, size, flag );
  #endif

  if( ret )
  return ::wTools::v8::errThrow
  (
    "BufferFromFile : flush failed", "\n",
    "errno : ", errno, "\n",
    "offset : ", offset, "\n",
    "size : ", size, "\n",
    "sync : ", sync, "\n",
    "invalidate : ", invalidate, "\n"
  );

}

//

void Init( Local< Object > exports, Local< Object > module )
{
  Isolate* isolate = Isolate::GetCurrent();
  // Local< Context > context = isolate->GetCurrentContext();

  auto fieldsOfExport = FieldsOf( exports );

  fieldsOfExport.set( "sizeOfPage", ( uint32_t ) pageSizeGet() );

  fieldsOfExport
  .set( "mmap", mmap_js )
  .set( "buffer", mmap_js )
  .set( "flush", flush_js )
  .set( "status", status_js )
  .set( "advise", advise_js )
  .set( "unmap",unmap_js )
  ;

  /* protection */

  Local< Object > Protection = Object::New( isolate );
  auto fieldsOfProtection = FieldsOf( Protection );

  fieldsOfProtection
  .set( "read",PROT_READ )
  .set( "write",PROT_WRITE )
  .set( "readWrite",PROT_WRITE | PROT_READ )
  .set( "exec",PROT_EXEC )
  .set( "none",PROT_NONE )
  ;

  fieldsOfExport.set( fromScope_M( Protection ) );

  /* flag */

  Local< Object > Flag = Object::New( isolate );
  auto fieldsOfFlag = FieldsOf( Flag );

  fieldsOfFlag
  .set( "shared",MAP_SHARED )
  .set( "private",MAP_PRIVATE )
  ;

  fieldsOfExport.set( fromScope_M( Flag ) );

  /* sync */

  Local< Object > Sync = Object::New( isolate );
  auto fieldsOfSync = FieldsOf( Sync );

  fieldsOfSync
  .set( "async",MS_ASYNC )
  .set( "sync",MS_SYNC )
  .set( "invalidate",MS_INVALIDATE )
  ;

  fieldsOfExport.set( fromScope_M( Sync ) );

  /* advise */

  Local< Object > Advise = Object::New( isolate );
  auto fieldsOfAdvise = FieldsOf( Advise );

  fieldsOfAdvise
  .set( "normal",MADV_NORMAL )
  .set( "random",MADV_RANDOM )
  .set( "sequential",MADV_SEQUENTIAL )
  .set( "willneed",MADV_WILLNEED )
  .set( "dontneed",MADV_DONTNEED )
  ;

  fieldsOfExport.set( fromScope_M( Advise ) );

}

//

NODE_MODULE( ArrayBufferFile, Init )

//

#include "../.module/NjsCppBasic/cpp/Body.end.cpp"

#endif // _Main_cpp_ //
