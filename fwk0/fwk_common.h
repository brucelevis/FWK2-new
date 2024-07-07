// ----------------------------------------------------------------------------
// if/n/def hell

#define is(macro)                      ifdef_##macro(1,0)
#define ifdef(macro, yes, /*no*/...)   ifdef_##macro(yes, __VA_ARGS__)
#define ifndef(macro, yes, /*no*/...)  ifdef_##macro(__VA_ARGS__, yes)
#define ifdef_1(yes, /*no*/...)        yes
#define ifdef_0(yes, /*no*/...)        __VA_ARGS__

#ifdef _MSC_VER
#define ifdef_GCC                      ifdef_0
#define ifdef_MINGW                    ifdef_0
#define ifdef_TCC                      ifdef_0
#define ifdef_MSC                      ifdef_1
#elif defined __TINYC__
#define ifdef_GCC                      ifdef_0
#define ifdef_MINGW                    ifdef_0
#define ifdef_TCC                      ifdef_1
#define ifdef_MSC                      ifdef_0
#elif defined __MINGW64__ || defined __MINGW32__
#define ifdef_GCC                      ifdef_1
#define ifdef_MINGW                    ifdef_1
#define ifdef_TCC                      ifdef_0
#define ifdef_MSC                      ifdef_0
#else // also __clang__
#define ifdef_GCC                      ifdef_1
#define ifdef_MINGW                    ifdef_0
#define ifdef_TCC                      ifdef_0
#define ifdef_MSC                      ifdef_0
#endif

#ifdef __cplusplus
#define ifdef_CPP                      ifdef_1
#define ifdef_C                        ifdef_0
#else
#define ifdef_C                        ifdef_1
#define ifdef_CPP                      ifdef_0
#endif

#if defined _WIN32
#define ifdef_WIN32                    ifdef_1
#define ifdef_LINUX                    ifdef_0
#define ifdef_OSX                      ifdef_0
#define ifdef_BSD                      ifdef_0
#define ifdef_EMS                      ifdef_0
#elif defined __linux__
#define ifdef_WIN32                    ifdef_0
#define ifdef_LINUX                    ifdef_1
#define ifdef_OSX                      ifdef_0
#define ifdef_BSD                      ifdef_0
#define ifdef_EMS                      ifdef_0
#elif defined __APPLE__
#define ifdef_WIN32                    ifdef_0
#define ifdef_LINUX                    ifdef_0
#define ifdef_OSX                      ifdef_1
#define ifdef_BSD                      ifdef_0
#define ifdef_EMS                      ifdef_0
#elif defined __EMSCRIPTEN__
#define ifdef_WIN32                    ifdef_0
#define ifdef_LINUX                    ifdef_0
#define ifdef_OSX                      ifdef_0
#define ifdef_BSD                      ifdef_0
#define ifdef_EMS                      ifdef_1
#else // __FreeBSD__ || @todo: __ANDROID_API__
#define ifdef_WIN32                    ifdef_0
#define ifdef_LINUX                    ifdef_0
#define ifdef_OSX                      ifdef_0
#define ifdef_BSD                      ifdef_1
#define ifdef_EMS                      ifdef_0
#endif

// rely on NDEBUG as the official/portable way to disable asserts
// we extend NDEBUG=[0,1,2,3] to signal the compiler optimization flags O0,O1,O2,O3
#if   defined NDEBUG && NDEBUG >= 3
#define ifdef_O3                       ifdef_1
#define ifdef_O2                       ifdef_0
#define ifdef_O1                       ifdef_0
#define ifdef_O0                       ifdef_0
#define ifdef_RETAIL                   ifdef_1
#define ifdef_RELEASE                  ifdef_0
#define ifdef_DEBUG                    ifdef_0
#elif defined NDEBUG && NDEBUG >= 2
#define ifdef_O3                       ifdef_0
#define ifdef_O2                       ifdef_1
#define ifdef_O1                       ifdef_0
#define ifdef_O0                       ifdef_0
#define ifdef_RETAIL                   ifdef_0
#define ifdef_RELEASE                  ifdef_1
#define ifdef_DEBUG                    ifdef_0
#elif defined NDEBUG && NDEBUG >= 1
#define ifdef_O3                       ifdef_0
#define ifdef_O2                       ifdef_0
#define ifdef_O1                       ifdef_1
#define ifdef_O0                       ifdef_0
#define ifdef_RETAIL                   ifdef_0
#define ifdef_RELEASE                  ifdef_1
#define ifdef_DEBUG                    ifdef_0
#else
#define ifdef_O3                       ifdef_0
#define ifdef_O2                       ifdef_0
#define ifdef_O1                       ifdef_0
#define ifdef_O0                       ifdef_1
#define ifdef_RETAIL                   ifdef_0
#define ifdef_RELEASE                  ifdef_0
#define ifdef_DEBUG                    ifdef_1
#endif

#if (defined INTPTR_MAX && INTPTR_MAX == INT64_MAX) || defined(_M_X64) || defined(__amd64__) || defined(__x86_64__) || defined(__x86_64) || defined(__ppc64__) || __WORDSIZE == 64
#define ifdef_X64                      ifdef_1
#define ifdef_X32                      ifdef_0
#else
#define ifdef_X64                      ifdef_0
#define ifdef_X32                      ifdef_1
#endif

// ----------------------------------------------------------------------------
// new keywords

#if is(MSC)
#define __thread                   __declspec(thread)
#elif is(TCC) && is(WIN32)
#define __thread                   __declspec(thread) // compiles fine apparently, but does not work
#elif is(TCC)
#define __thread
#endif

// ----------------------------------------------------------------------------
// new macros

#ifndef ASSERT
#define ASSERT(expr, ...)          ifdef(RETAIL, (void)0, do { int fool_msvc[] = {0,}; if(!(expr)) { fool_msvc[0]++;      alert(va("!Expression failed: " #expr " " FILELINE "\n" __VA_ARGS__)), breakpoint(); } } while(0) )
#endif

#ifndef ASSERT_ONCE
#define ASSERT_ONCE(expr, ...)     ifdef(RETAIL, (void)0, do { int fool_msvc[] = {0,}; if(!(expr)) { fool_msvc[0]++; ONCE alert(va("!Expression failed: " #expr " " FILELINE "\n" __VA_ARGS__)), breakpoint(); } } while(0) )
#endif

#ifndef BUILD_VERSION
#define BUILD_VERSION              ""
#endif

#ifndef CAST
#define CAST(type, ...)            ( ifdef(C,(type),type) { __VA_ARGS__ } )
#endif

#ifndef CONCAT
#define CONCAT(a,b)                CONC4T(a,b)
#define CONC4T(a,b)                a##b ///-
#endif

#ifndef COUNTOF
#define COUNTOF(x)                 (int)(sizeof (x) / sizeof 0[x])
#endif

#ifndef EXPAND
#define EXPAND(name, ...)          EXPAND_QUOTE(EXPAND_JOIN(name, EXPAND_COUNT_ARGS(__VA_ARGS__)), (__VA_ARGS__))
#define EXPAND_QUOTE(x, y)         x y ///-
#define EXPAND_JOIN(name, count)   EXPAND_J0IN(name, count) ///-
#define EXPAND_J0IN(name, count)   EXPAND_J01N(name, count) ///-
#define EXPAND_J01N(name, count)   name##count ///-
#define EXPAND_COUNT_ARGS(...)     EXPAND_ARGS((__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)) ///-
#define EXPAND_ARGS(args)          EXPAND_RETURN_COUNT args ///-
#define EXPAND_RETURN_COUNT(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, count, ...) count ///-
#endif

#ifndef FILELINE
#define FILELINE                   __FILE__ ":" STRINGIZE(__LINE__)
#endif

#ifndef MACRO
#define MACRO(name)                CONCAT(name, __LINE__)
#endif

#ifndef ONCE
#define ONCE                       static int MACRO(once) = 0; for(;!MACRO(once);MACRO(once)=1)
#endif

#ifndef STATIC_ASSERT
#define STATIC_ASSERT(EXPR)        typedef struct { unsigned MACRO(static_assert_on_L) : !!(EXPR); } UNIQUE(static_assert_on_L)
#endif

#ifndef STRINGIZE
#define STRINGIZE(x)               STRING1ZE(x)
#define STRING1ZE(x)               #x ///-
#endif

#ifndef UNIQUE
#define UNIQUE(name)               CONCAT(CONCAT(CONCAT(name,CONCAT(_L,__LINE__)),_),__COUNTER__)
#endif

#ifndef VA_COUNT
#define VA_COUNT(...)              (int)(sizeof((int[]){0, ##__VA_ARGS__})/sizeof(int)-1)
#endif

#ifndef VA_FIRST
#define VA_FIRST(...)              VA_F1RST(__VA_ARGS__, throwaway)
#define VA_F1RST(first, ...)       first ///-
#endif

#ifndef VA_OTHER
#define VA_OTHER(...)              VA_0THER(VA_NUM(__VA_ARGS__), __VA_ARGS__)
#define VA_0THER(qty, ...)         VA_0TH3R(qty, __VA_ARGS__) ///-
#define VA_0TH3R(qty, ...)         VA_0TH3R_##qty(__VA_ARGS__) ///-
#define VA_0TH3R_ONE(first)        ///-
#define VA_0TH3R_MORETHAN1(first, ...) , __VA_ARGS__ ///-
#define VA_NUM(...)                VA_SELECT_10TH(__VA_ARGS__, MORETHAN1, MORETHAN1, MORETHAN1, MORETHAN1, MORETHAN1, MORETHAN1, MORETHAN1, MORETHAN1, ONE, throwaway) ///-
#define VA_SELECT_10TH(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, ...) A10 ///-
// VA_COUNT() counts number of va args
// VA_SPLIT() expands to A) 1 item OR B) 1 item + ',' + va_args[1..N]
// VA_FIRST() expands to the first argument
// VA_OTHER() if there's only one argument, expands to nothing.  if there is more
//            than one argument, expands to a comma followed by everything but
//            the first argument. supports up to 9 arguments, but can be expanded.
#endif

#ifndef VA_SPLIT
#define VA_SPLIT(...)              VA_FIRST(__VA_ARGS__) VA_OTHER(__VA_ARGS__)
#endif

// -----------------------------------------------------------------------------
// autorun initializers for C
// - rlyeh, public domain
//
// note: based on code by Joe Lowe (public domain).
// note: XIU for C initializers, XCU for C++ initializers, XTU for C deinitializers

#define AUTORUN AUTORUN_( UNIQUE(fn) )
#ifdef __cplusplus
#define AUTORUN_(fn) \
    static void fn(void); \
    static const int CONCAT(fn,__1) = (fn(), 1); \
    static void fn(void)
#elif defined _MSC_VER && !defined(__clang__) // cl, but not clang-cl
#define AUTORUN_(fn) \
    static void fn(void); \
    static int CONCAT(fn,__1) (){ fn(); return 0; } \
    __pragma(section(".CRT$XIU", long, read)) \
    __declspec(allocate(".CRT$XIU")) \
    static int(* CONCAT(fn,__2) )() = CONCAT(fn,__1); \
    static void fn(void)
#elif defined __TINYC__ // tcc...
#define AUTORUN_(fn) \
    __attribute((constructor)) \
    static void fn(void)
#else // gcc,clang,clang-cl...
#define AUTORUN_(fn) \
    __attribute__((constructor(__COUNTER__+101))) \
    static void fn(void)
#endif

// -----------------------------------------------------------------------------
// visibility
// win32 users would need to -DAPI=EXPORT/IMPORT as needed when building/using FWK as DLL.

#define IMPORT ifdef(WIN32, ifdef(GCC, __attribute__ ((dllimport)), __declspec(dllimport)))
#define EXPORT ifdef(WIN32, ifdef(GCC, __attribute__ ((dllexport)), __declspec(dllexport)))
#define STATIC

#ifndef API
#define API STATIC
#endif

// -----------------------------------------------------------------------------
// pragma libs

#if is(WIN32) && (is(MSC) || is(TCC))
#pragma comment(lib, "advapi32")
#pragma comment(lib, "comdlg32")
#pragma comment(lib, "dbghelp")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "ole32")
#pragma comment(lib, "shell32")
#pragma comment(lib, "user32")
#pragma comment(lib, "winmm")
#pragma comment(lib, "wininet")
#pragma comment(lib, "ws2_32")
#endif

#if is(LINUX) && is(TCC)
#pragma comment(lib, "dl")
#pragma comment(lib, "m")
#pragma comment(lib, "pthread")
#endif

// -----------------------------------------------------------------------------
// tcc fixes

#if is(WIN32) && is(TCC) && is(X64)
#include <tgmath.h>
#define sqrtf sqrt
#define sinf sin
#define asinf asin
#define cosf cos
#define acosf acos
#define tanf tan
#define atan2f atan2
#define powf pow
#define floorf floor
#define logf log
#define ceilf ceil
#define copysignf copysign
#define expf exp
#define fmodf fmod
#define log10f log10
#define hypotf hypot
#undef  bool
typedef char bool; ///-
#endif
