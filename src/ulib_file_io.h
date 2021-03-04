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

#ifndef _ulib_file_io_h_
#define _ulib_file_io_h_

#include "ulib_common.h"

/******************************************************************************
* Public functions
*
* ulib__uint8* _tReadEntireFile(IN  const _TCHAR* fileName,
*                               OUT ulib__SizeType* fileSize)
* ulib__uint8 _tWriteEntireFile(IN const _TCHAR* fileName,
*                               IN const ulib__uint8* buffer,
*                               IN const ulib__SizeType count);
******************************************************************************/

#ifdef __cplusplus
namespace ulib{
#endif
#ifdef __cplusplus
extern "C"{
#endif
/******************************************************************************
* Function:
*           ulib__uint8* _tReadEntireFile(IN  const _TCHAR* fileName,
*                                         OUT ulib__SizeType* fileSize)
* Function that reads all content of a file in binary mode
* Allocates the space needed via malloc - user should call ULIB_FREE when done
* Appends the NULL terminator to the read buffer
* Parameters:
*       Input:  const _TCHAR* FileName
*       Return: ulib__uint8* buffer allocated with malloc of size of file
                length + 1, including NULL terminator
*               NULL if something went wrong IE:
*                    - didn't read all the file contents,
*                    - error in malloc
*                    - file not found, etc
******************************************************************************/
    ulib__uint8* _tReadEntireFile(IN  const _TCHAR* fileName,
                                  OUT ulib__SizeType* fileSize);
/*****************************************************************************/

/******************************************************************************
* Function:
*           ulib__uint8 _tWriteEntireFile(IN const _TCHAR* fileName,
*                                         IN const ulib__uint8* buffer,
*                                         IN const ulib__SizeType count);
* Function that writes a uint8_t* to a file, binary mode
* Overwrites if file exists
* Parameters:
*       Input:  const char* FileName
*               const ulib__uint8* buffer
*               const ulib__SizeType count
*       Return: NULL if something went wrong
******************************************************************************/
    ulib__uint8 _tWriteEntireFile(IN const _TCHAR* fileName,
                                  IN const ulib__uint8* buffer,
                                  IN const ulib__SizeType count);
/*****************************************************************************/
#ifdef __cplusplus
} /* extern "C" {*/
#endif


#ifdef IMPLEMENTATION
ulib__uint8* _tReadEntireFile(IN const _TCHAR* fileName,
                              OUT ulib__SizeType* fileSize){
    FILE* file = ULIB_NULL;
    ulib__uint8* contents = ULIB_NULL;
    ulib__int32 localSize = 0;
    ulib__SizeType readCount = 0;
    _tfopen_s(&file, fileName, _TEXT("rb"));
    if (file == ULIB_NULL){
        fileSize = ULIB_NULL;
        return (ULIB_NULL);
    }
    fseek(file, 0, SEEK_END);
    localSize = ftell(file);
    if (localSize == -1L){
        return (ULIB_NULL);
    }
    fseek(file, 0, SEEK_SET);
    *fileSize = (ulib__SizeType)localSize;
    contents = (ulib__uint8*)malloc((*fileSize) + 1u);
    if (contents == ULIB_NULL){
        return (ULIB_NULL);
    }
    readCount = fread(contents, 1, *fileSize, file);
    if (readCount != *fileSize){
        fileSize = ULIB_NULL;
        ULIB_FREE(contents);
        return (ULIB_NULL);
    }
    contents[*fileSize] = 0L;
    fclose(file);
    return(contents);
}

ulib__uint8 _tWriteEntireFile(IN const _TCHAR* fileName,
                              IN const ulib__uint8* buffer,
                              IN const ulib__SizeType count){
    FILE* file = ULIB_NULL;
    ulib__SizeType writeCount = 0;
    _tfopen_s(&file, fileName, _TEXT("wb"));
    if (file == ULIB_NULL){
        return (ULIB_ERROR);
    }
    writeCount = fwrite(buffer, 1u, count, file);
    if (writeCount != count){
        fclose(file);
        return (ULIB_ERROR);
    }
    fclose(file);
    return (ULIB_SUCCESS);
}

#endif // #ifdef IMPLEMENTATION
#ifdef __cplusplus // namespace ulib{
}
#endif
#endif // #ifndef ulib_file_io_h
