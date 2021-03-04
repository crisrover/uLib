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

#ifndef _ulib_win_listdir_h_
#define _ulib_win_listdir_h_

#include "ulib_common.h"
//#define ULIB_VECTOR_DEBUG
#include "ulib_vector.h"
#ifdef __cplusplus
namespace ulib {
#endif

#define INIT_LISTDIRDATA(listDirData)\
    listDirData.processFile = ULIB_NULL;\
    listDirData.processDirectory = ULIB_NULL;\
    listDirData.totalFiles = 0;\
    listDirData.totalDirs = 0;\
    listDirData.dir = ULIB_NULL;\
    listDirData.recurse = ULIB_FALSE;\
    listDirData.shouldExit = ULIB_NULL;


#define ULIB_MAX_WINDOWS_PATH 32768u * sizeof(_TCHAR)
//
// The callback for dirs and files.
// fullPath is the file name with full path
// fileName is only the file name
//
typedef void (*ProcessFileName)(_TCHAR* fullPath, _TCHAR* fileName);

typedef struct ListDirData_
{
OUT   ulib__uint64            totalFiles;       /* Total number of files */
OUT   ulib__uint64            totalDirs;        /* Total number of dirs */
IN    ProcessFileName         processFile;      /* File callback */
IN    ProcessFileName         processDirectory; /* Directory callback */
IN    _TCHAR*                 dir;              /* Start dir */
IN    volatile ulib__bool*    shouldExit;       /* This is a volatile byte set by CTRL-C handler */
IN    ulib__bool              recurse;          /* Scan folders recursively */
}ListDirData;

/* Public functions */
/******************************************************************************
* Function: ulib__bool ListDir(ListDirData* dir)
* Parameters:
*      Input:  ListDirData* dir
*              The structure can contain two function pointers to be called for
*              files and directories, and the start directory
*      Return: ULIB_SUCCESS if successful
*              ULIB_FILE_NOT_FOUND in case of an error - the start directory
*              is not found
* NOTE: In case of an error, UlibGetSystemLastErrorString() can be used to get the
* error formatted as string, or uint32_t dw = GetLastError() can be used to get
* the error code.
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
#pragma pack(1)
    typedef struct _item
    {
        HANDLE handle;
        _TCHAR parentPth[ULIB_MAX_WINDOWS_PATH];
    }item;
#pragma pack ()

ulib__uint8 ListDir(ListDirData* dir);
#ifdef __cplusplus
}
#endif

/* ========================================================================= */
#ifdef IMPLEMENTATION
static item            it;
static WIN32_FIND_DATA file;
static ulib__SizeType  dirLength = 0;
static ulib_vector     vector;
static _TCHAR          searchPth[ULIB_MAX_WINDOWS_PATH];
ulib__uint8 ListDir(ListDirData* listDirData){
    it.handle = ULIB_NULL;
    memset(it.parentPth, 0, ULIB_MAX_WINDOWS_PATH);
    memset(searchPth, 0, ULIB_MAX_WINDOWS_PATH);
    INIT_ULIB_VECTOR(vector, ULIB_MAX_WINDOWS_PATH << 8u, sizeof(_TCHAR));
    dirLength = _tcslen(listDirData->dir);
    _tcscpy(it.parentPth, listDirData->dir);
    if ((it.parentPth[dirLength - 1] == _T("\\")[0]) ||
        (it.parentPth[dirLength - 1] == _T("/")[0])){
        it.parentPth[dirLength - 1] = _T('\0');
    }
    _tcscpy(searchPth, it.parentPth);
    _tcscat(searchPth, TEXT("\\*"));
    it.handle = FindFirstFile(searchPth, &file);
    if (it.handle == INVALID_HANDLE_VALUE){
        if (vector.workBuffer){
            UlibVectorFree(&vector);
        }
        return (ULIB_FILE_NOT_FOUND);
    }
    for (;;){
        for (;;){
            if (listDirData->shouldExit && (*(listDirData->shouldExit))){
                break;
            }
            if (it.handle  == INVALID_HANDLE_VALUE){
                break;
            }
            if (_tcscmp(file.cFileName, TEXT(".")) != 0 &&
                                      _tcscmp(file.cFileName, TEXT("..")) != 0){
                if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){// dir
                    ++listDirData->totalDirs;
                    _tcscpy(searchPth, it.parentPth);
                    _tcscat(searchPth, TEXT("\\"));
                    _tcscat(searchPth, file.cFileName);
                    if (listDirData->processDirectory != ULIB_NULL){
                        listDirData->processDirectory(searchPth, file.cFileName);
                    }
                    if (listDirData->recurse == ULIB_TRUE){
                        if (UlibVectorPush(&vector, &it, sizeof(HANDLE) +
                            _tcslen(it.parentPth)) != ULIB_SUCCESS){
                            return (ULIB_ERROR);
                        }
                        _tcscpy(it.parentPth, searchPth);
                        _tcscat(searchPth, TEXT("\\*"));// prepare for next run
                        it.handle = FindFirstFile(searchPth, &file);
                    }
                    else if ((FindNextFile(it.handle, &file) == 0))
                            break;
                }
                else{ // if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    ++listDirData->totalFiles;
                    if (listDirData->processFile != ULIB_NULL){
                        _tcscpy(searchPth, it.parentPth);
                        _tcscat(searchPth, TEXT("\\"));
                        _tcscat(searchPth, file.cFileName);
                        listDirData->processFile(searchPth, file.cFileName);
                    }
                    if ((FindNextFile(it.handle, &file) == 0))
                            break;
                }
            }// if (_tcscmp(file.cFileName, TEXT(".")) != 0 &&
            else                // _tcscmp(file.cFileName, TEXT("..")) != 0)
                if ((FindNextFile(it.handle, &file) == 0))
                    break;
        }// for (;;) - inner for
    retry:
        memset(it.parentPth, 0, ULIB_MAX_WINDOWS_PATH);
        if(UlibVectorPop(&vector, &it) == ULIB_SUCCESS){
            if (FindNextFile(it.handle, &file) == 0)
                goto retry;
        }
        else
            break;
    }//for(;;) - main loop
    if (vector.workBuffer){
        UlibVectorFree(&vector);
    }
    if (it.handle){
        FindClose(it.handle);
    }
#ifdef ULIB_VECTOR_DEBUG
    _tprintf(_T("\nAllocations: %d\n"), vector.ulibVectorAllocations);
    _tprintf(_T("Free:        %d\n"), vector.ulibVectorFree);
    _tprintf(_T("Total mem:   %zd\n"), vector.ulibVectorAllocations *
                                      vector.bufferSize);
#endif
    return (ULIB_SUCCESS);
}
#endif
#ifdef __cplusplus
} /* namespace ulib{ */
#endif
#endif // #ifndef _ulib_win_listdir_h_
