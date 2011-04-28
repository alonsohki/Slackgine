#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(__arm__)
typedef bool b8;
typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;
typedef u64 uint64_t;
#elif defined(__x86_64)
typedef bool b8;
typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long i64;
typedef unsigned long u64;
#elif defined(__i586__) || defined(__i686__)
typedef bool b8;
typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;
#endif

static inline bool detectBigEndian ()
{
    static union { const u16 w; const u8 v[2]; } const u = { 1 };
    return ( u.v[0] == 0 );
}

#endif
