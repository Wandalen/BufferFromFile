#ifndef _wTools_abase_Type_hpp_ //
#define _wTools_abase_Type_hpp_

#include <climits>

namespace wTools { //

typedef char Chr;
typedef unsigned char Wrd1;
typedef signed char Int1;

typedef unsigned short int Wrd2;
typedef signed short int Int2;

typedef uint32_t Wrd4;
typedef int32_t Int4;

typedef unsigned long long int Wrd8;
typedef signed long long int Int8;

typedef unsigned int Wrd4Maybe2;
typedef signed int Int4Maybe2;

typedef unsigned long int Wrd8Maybe4;
typedef signed long int Int8Maybe4;

#if UINT_MAX == 65535
#define Wrd4Maybe2Size 2
#define Int4Maybe2Size 2
#elif UINT_MAX == 4294967295
#define Wrd4Maybe2Size 4
#define Int4Maybe2Size 4
#else
Unexpected value of UINT_MAX!
#endif

#if ULONG_MAX == 4294967295
#define Wrd8Maybe4Size 4
#define Int8Maybe4Size 4
#elif ULONG_MAX == 18446744073709551615UL
#define Wrd8Maybe4Size 8
#define Int8Maybe4Size 8
#else
Unexpected value of ULONG_MAX!
#endif

} // namespace wTools //

#endif // _wTools_abase_Type_hpp_
