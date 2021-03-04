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

#ifndef ulib_string_utils_h
#define ulib_string_utils_h
#include "ulib_common.h"

/******************************************************************************
* Public API functions
*
* _TCHAR           ToLower(const IN _TCHAR chr)
* void             ToLowerString(INOUT _TCHAR* string)
* ulib__bool       ORCompareMultipleStrings(const IN ulib__uint16 numberOfStrings, ...)
* ulib__bool       WildcardMatch(const IN _TCHAR* pattern, const IN _TCHAR* str);
* ulib__OffsetType Find(const char* text, const char* pattern)
******************************************************************************/

#ifdef __cplusplus
namespace ulib{
#endif
#ifdef __cplusplus
extern "C"{
#endif
/******************************************************************************
* Function: ULIB_INLINE TCHAR ToLower(const IN __TCHAR chr)
* Parameters:
*       Input : str - _TCHAR to be converted to lowercase
*       Return: lowercase _TCHAR
******************************************************************************/
    ULIB_INLINE _TCHAR ToLower(const IN _TCHAR chr);

/******************************************************************************
* Function: ULIB_INLINE void  ToLowerString(INOUT __TCHAR* string)
* Parameters:
*       Input/Output : string - _TCHAR* string to be converted to lowercase
*       Return: Nothing
******************************************************************************/
    ULIB_INLINE void  ToLowerString(INOUT _TCHAR* string);

/******************************************************************************
* Function:
*          static ulib__bool ORCompareMultipleStrings(int numberOfStrings, ...)
* Compares fist va_arg with the next one(s)
* Parameters:
*       Input:   numberOfStrings - how many strings are in total
*               _TCHAR* string to be compared
*       Return: ULIB_TRUE if any string matches with the first one
******************************************************************************/
    ulib__bool ORCompareMultipleStrings(const IN ulib__uint16 numberOfStrings, ...);

/******************************************************************************
* Function:
*          ulib__bool WildcardMatch(IN const __TCHAR* pattern, IN const __TCHAR* str)
* Matches the pattern string that can contain wildcard(s) (*) against the str
* Parameters:
*       Input:  const _TCHAR* pattern
*               const _TCHAR* str
*       Return: ULIB_TRUE  if it is a match
*               ULIB_FALSE if not
******************************************************************************/
    ulib__bool WildcardMatch(const IN _TCHAR* pattern, const IN _TCHAR* str);

/******************************************************************************
* Function:
*          ulib__OffsetType Find(const _TCHAR *text, const _TCHAR *pattern)
* Perform a plain search in text for pattern
* Parameters:
*      Input:  const _TCHAR* text
               const _TCHAR* pattern
*      Return: index in text if successful
*              ULIB_FAIL (-1) if not found
******************************************************************************/
    ulib__OffsetType Find(const IN _TCHAR* text, const IN _TCHAR* pattern);
/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif


#ifdef IMPLEMENTATION
#ifdef __cplusplus
extern "C"{
#endif
_TCHAR ToLower(const _TCHAR chr){
    if (chr >= _T('A') && chr <= _T('Z')){
        return (_TCHAR)(_totlower(chr));
    }
    return((_TCHAR)chr);
}// _TCHAR ToLower(const _TCHAR chr)

void ToLowerString(_TCHAR* str){
    if (str){
        while (*str){
            *str = ToLower(*str);
            ++str;
        }
    }
}// void ToLowerString(_TCHAR* str)

ulib__bool ORCompareMultipleStrings(const ulib__uint16 numberOfStrings, ...){
    va_list list;
    _TCHAR* firstString;
    _TCHAR* secondString;
    ulib__uint8 i = 1u;
    va_start(list, numberOfStrings);
    firstString = va_arg(list, _TCHAR*);
    for (; i < numberOfStrings; ++i){
        secondString = va_arg(list, _TCHAR*);
        if (_tcscmp(firstString, secondString) == 0){
            va_end(list);
            return (ULIB_TRUE);
        }
    }
    va_end(list);
    return (ULIB_FALSE);
}// ulib__bool ORCompareMultipleStrings(const ulib__uint16 numberOfStrings, ...)

ulib__bool WildcardMatch(IN const _TCHAR* pattern, IN const _TCHAR* str){
    const _TCHAR* backup = ULIB_NULL;
    if (!pattern || !str){
        return (ULIB_FALSE);
    }
    while (*pattern){
        if (*pattern == _T('*')){
            backup = pattern;
            while (*pattern && *pattern == _T('*')) ++pattern;
            if (!*pattern){ // last is * so everything matches!
                return (ULIB_TRUE);
            }
            while (*str){
                if (*str != *pattern){
                    ++str;
                }
                else{
                    break;
                }
            }
            if (!*str){ // reached the end of str without a match
                return (ULIB_FALSE);
            }
            while (*str && *pattern && (*str == *pattern)) ++str, ++pattern;

            if (*pattern && *pattern != _T('*')){
                // Haven't found all the pattern, so restore it
                pattern = backup;
            }
            if (!*pattern && !*str){
                return(ULIB_TRUE);
            }
        }
        else{ // Plain search
            if (*str != *pattern && *pattern != _T('*')) return (ULIB_FALSE);
            while (*str && *pattern && *str == *pattern && *pattern != '*')
                ++str, ++pattern;
        }
        if (!*pattern && *str && backup){ // in case of *.txt in adfas.txt.txt
            pattern = backup;
        }
    }
    if (!*str && !*pattern){
        return (ULIB_TRUE);
    }
    return (ULIB_FALSE);
}// ulib__bool WildcardMatch(IN const _TCHAR* pattern, IN const _TCHAR* str)

ulib__OffsetType Find(const _TCHAR* text, const _TCHAR* pattern){
    ulib__SizeType patLen = _tcslen(pattern);
    ulib__SizeType strLen = _tcslen(text);
    ulib__SizeType i = 0;
    ulib__SizeType f = 0;

    if (patLen > strLen){
        return (ULIB_FAIL);
    }

    strLen -= (patLen - 1u); // No need to search in the last chars
    for (; i < strLen; ++i){
        if (text[i] == pattern[0]){
            for (; f < patLen; ++f){
                if (text[i + f] != pattern[f]){
                    break;
                }
            }
            if (f == patLen){
                return (ulib__OffsetType)(i);
            }
        }
    }
    return (ULIB_FAIL);
}// ulib__OffsetType Find(const char* text, const char* pattern)
#ifdef __cplusplus
}// extern "C"
#endif
#endif // #ifdef IMPLEMENTATION
#ifdef __cplusplus
} // namespace ulib
#endif
#endif // #ifndef ulib_string_utils.h
