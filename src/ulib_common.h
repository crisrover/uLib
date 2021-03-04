/*

Copyright (c) 2018-2021, Croitor Cristian

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

For licensing, please check the LICENSE file included with the source code.
*/

/*
 Common include files and defines
 Common defines
*/
#ifndef ulib_common_h
#define ulib_common_h

#ifdef _MSC_VER
#pragma warning(push, 0)
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h> // /Wall warnings
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#pragma warning(pop)
#pragma warning( disable:  4505) // Disable Function unused
#pragma warning( disable:  4514) // Disable unref'd inline function has been removed
#pragma warning( disable:  5045) // Disable Spectre mitigation warning
#ifdef NDEBUG
#pragma warning( disable:  4710) // Disable function not inlined
#pragma warning( disable:  4711) // Disable selected for automatic inline expansion
#endif
#else
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#endif
#ifdef __cplusplus
namespace ulib{
#endif

 typedef char                    ulib__char;
 typedef char unsigned           ulib__uint8;
 typedef char signed             ulib__int8;
 typedef short unsigned          ulib__uint16;
 typedef short signed            ulib__int16;
 typedef int unsigned            ulib__uint32;
 typedef int signed              ulib__int32;
 typedef int                     ulib__bool;
#if defined(_MSC_VER)
 typedef __int64                 ulib__int64;
 typedef __int64 unsigned        ulib__uint64;
#elif defined(__GNUC__)
 typedef long long unsigned      ulib__uint64;
 typedef long long               ulib__int64;
#else
#error This compiler is not supported.
#endif
 typedef float                   ulib__float;
 typedef double                  ulib__double;
 typedef size_t                  ulib__SizeType;
 typedef ptrdiff_t               ulib__OffsetType;
 // Produce compiler error if size is wrong
 typedef unsigned char validate_uint8[sizeof(ulib__uint8) == 1 ? 1 : -1];
 typedef unsigned char validate_uint16[sizeof(ulib__uint16) == 2 ? 1 : -1];
 typedef unsigned char validate_uint32[sizeof(ulib__uint32) == 4 ? 1 : -1];
 typedef unsigned char validate_uint64[sizeof(ulib__uint64) == 8 ? 1 : -1];

 typedef struct timerStruct_ {
     LARGE_INTEGER ulibStartTimer;
     LARGE_INTEGER ulibFrequency;
     LARGE_INTEGER ulibStopTimer;
 }timer_struct;

 #ifdef _MSC_VER
#define ULIB_INLINE __forceinline
#else
#define ULIB_INLINE inline
#endif

#ifdef _MSC_VER
#define ULIB_WIN_EOL "\r\n"
#define _TULIB_WIN_EOL _T("\r\n")
#else
#define ULIB_LIN_EOL "\n"
#define _TULIB_LIN_EOL _T("\n")
#endif

#ifdef _MSC_VER
#define ULIB_EOL ULIB_WIN_EOL
#define _TULIB_EOL _TULIB_WIN_EOL
#else
#define ULIB_EOL ULIB_LIN_EOL
#define _TULIB_EOL _TULIB_LIN_EOL
#endif

#define ULIB_TRUE       1u
#define ULIB_FALSE      0u

#define IN
#define OUT
#define INOUT
#define ULIB_UNUSED(p) (void) p

#ifdef __cplusplus
#define ULIB_NULL     0
#define ULIB_EXTERN   extern "C"
#else
#define ULIB_NULL    ((void*)(0))
#define ULIB_EXTERN   extern
#endif

#define ULIB_FREE(p) free(p);p=ULIB_NULL
#define ULIB_ASSERT(cond) if(!cond)((*(ulib__int32*)(ULIB_NULL)) = ULIB_NULL)

#define ULIB_START_TIMER 0
#define ULIB_STOP_TIMER 1u

#define ULIB_KILOBYTE 1024u
#define ULIB_MEGABYTE ULIB_KILOBYTE * ULIB_KILOBYTE

 /* LOG utils */
#define LOG(...) \
fprintf(stdout, __VA_ARGS__);\
fprintf(stdout, ULIB_EOL)

#define LOG_WARNING(...) \
fputts("  Warning: ", stderr);\
fprintf(stderr, __VA_ARGS__);\
fprintf(stderr, ULIB_EOL)

#define LOG_ERROR(...) \
fputts("  Error: ", stderr);\
fprintf(stderr, __VA_ARGS__);\
fprintf(stderr, ULIB_EOL)

#define LOG_FATAL(...) \
fputs("  Fatal error: ", stderr);\
fprintf(stderr, __VA_ARGS__);\
fprintf(stderr, ULIB_EOL);\
exit(EXIT_FAILURE)

#define _TLOG(...) \
_ftprintf(stdout, __VA_ARGS__);\
_ftprintf(stdout, _TULIB_EOL)

#define _TLOG_WARNING(...) \
_fputts(_T("  Warning: "), stderr);\
_ftprintf(stderr, __VA_ARGS__);\
_ftprintf(stderr, _TULIB_EOL)

#define _TLOG_ERROR(...) \
_fputts(_T("  Error: "), stderr);\
_ftprintf(stderr, __VA_ARGS__);\
_ftprintf(stderr, _TULIB_EOL)

#define _TLOG_FATAL(...) \
_fputts(_T("  Fatal error: "), stderr);\
_ftprintf(stderr, __VA_ARGS__);\
_ftprintf(stderr, _TULIB_EOL);\
exit(EXIT_FAILURE)

#ifdef __cplusplus
 extern "C" {
#endif
 /**********************************************************************************
 * Description
 * uliberror will contain the error encountered somewhere in ulib
 * ulibErrors[uliberror] will yield the description of the error
 * static ulib__uint8 GetLastErrorText(OUT char* str);
 **********************************************************************************/
#define ULIB_FAIL                           -1
#define ULIB_SUCCESS                        0u   // Success - returned by default by all ulib functions
#define ULIB_ERROR                          1u   // General error, for more detail check uliberror variable
#define ULIB_NO_SUCCESS                     1u   // General fail - returned by default by all ulib functions
#define ULIB_MALLOC_ERROR                   2u   // Malloc error - malloc returned NULL
#define ULIB_VECTOR_NOT_INIT                3u   // Ulib vector is not initialized
#define ULIB_VECTOR_BUFFER_TOO_SMALL        4u   // Ulib vector buffer is too small
#define ULIB_INVALID_VECTOR                 5u   // Ulib vector is invalid
#define ULIB_FILE_NOT_FOUND                 6u   // Ulib file not found

#define MAX_ERROR_STRING_LEN 256U * sizeof(TCHAR) // Use this when creating a TCHAR* for GetLastErrorText()

ULIB_EXTERN ulib__uint8 ulibError; // Global variable holding the ulib error

 ulib__uint8 UlibGetLastErrorText(OUT _TCHAR* str);
 #ifdef __cplusplus
} /* extern "C" {*/
#endif
#ifdef IMPLEMENTATION
 ulib__uint8 ulibError = ULIB_SUCCESS;

 static const _TCHAR* ulibErrors[] = {_T("Ulib error"),
                                      _T("Ulib success"),
                                      _T("malloc error"),
                                      _T("Ulib vector not initialized"),
                                      _T("Ulib vector buffer too small"),
                                      _T("Ulib invalid vector"),
                                      _T("Ulib file not found")  };
/**********************************************************************************
* Function:
*
* static ulib__uint8 GetLastErrorText(OUT _TCHAR* str);
*
* Parameters:
*      Input:
*      Output:  char* output
*      Return:  ULIB_SUCCESS if successful
*               ULIB_ERROR if no str was NULL
* Remarks:
If str cannot contain MAX_ERROR_STRING_LEN chars, the result is undefined behavior
**********************************************************************************/
 ulib__uint8 UlibGetLastErrorText(_TCHAR* str){
     if (str){
         if (_stprintf(str, _T("Error: %d - %s"), ulibError, ulibErrors[ulibError])){
             return (ULIB_SUCCESS);
         }
     }
     return (ULIB_ERROR);
 }
#endif /* #ifdef IMPLEMENTATION */
/******************************************************************************
*                              TIMING UTILS                                   *
/******************************************************************************

/******************************************************************************
*  Basic timer
*  Example usage:

   double elapsed;
   BEGIN_TIMED_BLOCK(test);
   FunctionToBeTimed(void);
   END_TIMED_BLOCK(test, elapsed);
   printf("Timed: %.6f s\n", elapsed);

******************************************************************************/
#ifdef _MSC_VER
#define BEGIN_TIMED_BLOCK(name) \
{LARGE_INTEGER ulibStartTimer##name;\
LARGE_INTEGER ulibFrequency##name;\
QueryPerformanceCounter(&ulibStartTimer##name);\
QueryPerformanceFrequency(&ulibFrequency##name);

#define END_TIMED_BLOCK(name, res) \
LARGE_INTEGER ulibStopTimer##name;\
QueryPerformanceCounter(&ulibStopTimer##name);\
res = (double)(ulibStopTimer##name.QuadPart - ulibStartTimer##name.QuadPart) /\
      (double)ulibFrequency##name.QuadPart;}

double static Timer(timer_struct* t, ulib__uint8 action)
{
    if (action == ULIB_START_TIMER)
    {
        QueryPerformanceCounter(&t->ulibStartTimer);
        QueryPerformanceFrequency(&t->ulibFrequency);
        return ULIB_SUCCESS;
    }
    else if (action == ULIB_STOP_TIMER)
    {
        QueryPerformanceCounter(&t->ulibStopTimer);
        return((double)(t->ulibStopTimer.QuadPart - t->ulibStartTimer.QuadPart) /
              (double)t->ulibFrequency.QuadPart);
    }
    return ULIB_ERROR;
}
/* Linux specific */
#else
#include <sys/time.h>
#include <cstdio> //Check this!

#define BEGIN_TIMED_BLOCK(name) \
timeval ulibStartTimer##name;\
gettimeofday(&ulibStartTimer##name,ULIB_NULL);

#define END_TIMED_BLOCK(name, res) \
timeval ulibStopTimer##name;\
timeval result;\
gettimeofday(&ulibStopTimer##name,ULIB_NULL);\
timersub(&ulibStopTimer##name,&ulibStartTimer##name,&result);\
res = result.tv_sec + result.tv_usec/1000000.0;

#endif // #ifdef _MSC_VER

#ifdef __cplusplus
} // namespace ulib{
#endif // #ifdef __cplusplus
#endif // #ifndef ulib_common_h
