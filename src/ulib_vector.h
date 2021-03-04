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

/***********************************************************************************
*  Implementation of a LIFO linear vector
*  Pushing the data results in a copy in the vector buffer
*  Popping the data gets you a copy of the data, and moves the last index down
*  Iteration over the whole vector can be done like this:
*   while (UlibVectorPop(output) == ULIB_SUCCESS)
* NOTES:
*   1. The vector uses buffers of fixed size specified: BufferSize * ElementSize
*       INIT_ULIB_VECTOR(vec, BufferSize, ElementSize)
*   2. If the buffer fills, a new one is allocated of the same size.
*   3. The size of this buffer should be determined by user.
*   4. Pop function automatically frees the memory used by the buffer
*   5. In case of an error, the error code is stored in ulibError global variable
*   UlibGetLastErrorText(char* str) can be used to get the error text description
***********************************************************************************/
#ifndef _ulib_vector_h_
#define _ulib_vector_h_

#include "ulib_common.h"

#ifdef __cplusplus
namespace ulib{
#endif

#ifdef __cplusplus
extern "C"{
#endif
    typedef struct buffer_ {
        ulib__uint8*       data;
        struct buffer_*    previousBuffer;
        ulib__SizeType     lastIndex;
    }buffer;

    typedef struct ulib_vector_ {
        ulib__SizeType   bufferSize;     // Buffer size
        ulib__SizeType   elemSize;       // Element size
        buffer*          workBuffer;     // The current working buffer
        ulib__SizeType   lastIndexSize;  // Last index size in bytes
        ulib__uint32     ulibVectorAllocations;
        ulib__uint32     ulibVectorFree;
    }ulib_vector;

    buffer* Allocate(IN ulib_vector*);

#define INIT_ULIB_VECTOR(vec, BufferSize, ElementSize)\
    vec.bufferSize = BufferSize;\
    vec.elemSize = ElementSize;\
    vec.ulibVectorAllocations = 0u;\
    vec.ulibVectorFree = 0u;\
    vec.workBuffer = Allocate(&vec);\
    vec.lastIndexSize = sizeof(vec.workBuffer->lastIndex);


/* Public functions */
/******************************************************************************
* Function:
*           ulib__bool UlibVectorPush(IN ulib_vector* v,
*                                     IN const void* data,
*                                     IN const ulib__SizeType length);
* Parameters:
*      Input:  ulib_vector* v
*              const void* data
*              const ulib__SizeType length
*      Return: ULIB_SUCCESS if successful
*              ULIB_ERROR if an error occurred
******************************************************************************/
    ulib__bool UlibVectorPush(IN ulib_vector* v,
                              IN const void* data,
                              IN const ulib__SizeType length);

/******************************************************************************
* Function:
*           ulib__bool UlibVectorPop(IN ulib_vector* v, OUT void* output);
* Parameters:
*      Output:  void* output
*      Return: ULIB_SUCCESS if successful
*              ULIB_ERROR if no more strings are stored in the vector
******************************************************************************/
    ulib__bool UlibVectorPop(IN ulib_vector* v, OUT void* output);

/******************************************************************************
* Function:
*          void UlibVectorFree(IN ulib_vector* v);
* Frees all memory used by the vector, used in case not all the vector was
* traversed using UlibVectorPop()
* Parameters:
*      Input:  ulib_vector* v
*      Return: none
******************************************************************************/
    void UlibVectorFree(IN ulib_vector* v);
#ifdef __cplusplus
}
#endif

/******************************************************************************
* Internal functions
******************************************************************************/
// Statistics - total number of allocations
#ifdef IMPLEMENTATION
buffer* Allocate(ulib_vector* v){
    buffer* buff = (buffer*)malloc(sizeof(buffer));
    if (buff){
        buff->data = (ulib__uint8*)malloc(v->elemSize * v->bufferSize);
        if (buff->data){
            buff->previousBuffer = ULIB_NULL;
            buff->lastIndex = 0;
#ifdef ULIB_VECTOR_DEBUG
            ++(v->ulibVectorAllocations);
#endif
            return (buff);
        }
        ULIB_FREE(buff->data);
        ulibError = ULIB_MALLOC_ERROR;
        return (ULIB_NULL);
    }
    ULIB_FREE(buff);
    ulibError = ULIB_MALLOC_ERROR;
    return (ULIB_NULL);
}

void Free(buffer** buf, ulib__uint32* count){
    ULIB_FREE((*buf)->data);
    ULIB_FREE(*buf);
#ifdef ULIB_VECTOR_DEBUG
    ++(*count);
#else
    ULIB_UNUSED(count);
#endif
}


void UlibVectorFree(ulib_vector* v){
    if (v){
        while (UlibVectorPop(v, ULIB_NULL));
    }
}

ulib__bool UlibVectorPushElement(ulib_vector* v,
                                 const void* data) {
    return UlibVectorPush(v, data, v->elemSize);
}

ulib__bool UlibVectorPush(ulib_vector* v,
                          const void* data,
                          const ulib__SizeType length){
    if (v){
        if (v->workBuffer){
            ulib__SizeType len = v->elemSize * length;
            ulib__SizeType startOffset = v->workBuffer->lastIndex;
            // Data and index is bigger than the chunk allocated of BUFFER_SIZE
            if (len + v->lastIndexSize > v->bufferSize){
                ulibError = ULIB_VECTOR_BUFFER_TOO_SMALL;
                return (ULIB_ERROR);
            }
            else if (startOffset + len + v->lastIndexSize > v->bufferSize){// Does not fit in current buffer, a new one is needed
                buffer* newMem = Allocate(v);
                if (newMem == ULIB_NULL){
                    return (ULIB_ERROR);
                }
                newMem->previousBuffer = v->workBuffer;
                v->workBuffer = newMem;
                startOffset = 0;
            } // else if (startOffset + len + v->lastIndexSize > BUFFER_SIZE)
            if (data){
                memcpy(&v->workBuffer->data[v->workBuffer->lastIndex], data, len);
                v->workBuffer->lastIndex += len;
                memcpy(&v->workBuffer->data[v->workBuffer->lastIndex],
                       &startOffset,
                       v->lastIndexSize);
                v->workBuffer->lastIndex += v->lastIndexSize;
                return (ULIB_SUCCESS);
            }
        }// if (v->workBuffer)
        ulibError = ULIB_VECTOR_NOT_INIT;
    }// if (v)
    return (ULIB_ERROR);
}


ulib__bool UlibVectorPop(ulib_vector* v, void* output){
    if (v){// Data valid in current work buffer
        ulib__SizeType start_offset;
        if (!v->workBuffer){
            ulibError = ULIB_INVALID_VECTOR;
            return (ULIB_ERROR);
        }
        // Check if we still have data in work buffer
        if (v->workBuffer->lastIndex == 0 && v->workBuffer->previousBuffer != ULIB_NULL){
            buffer* current = v->workBuffer;
            v->workBuffer = v->workBuffer->previousBuffer;
            Free(&current, &v->ulibVectorFree);
        }
        if (v->workBuffer->lastIndex == 0){
            // Got to the beginning of the list
            Free(&v->workBuffer, &v->ulibVectorFree);
            return (ULIB_ERROR);
        }
        // Get the index in the buffer
        memcpy(&start_offset,
               &v->workBuffer->data[v->workBuffer->lastIndex - v->lastIndexSize],
               v->lastIndexSize);
        // Get the string
        if (output){
            memcpy(output,
                   &v->workBuffer->data[start_offset],
                   v->workBuffer->lastIndex - v->lastIndexSize - start_offset);
        }
        v->workBuffer->lastIndex = start_offset;
        return (ULIB_SUCCESS);
    } // if (v)
    return (ULIB_ERROR);
}

#endif // IMPLEMENTATION
#ifdef __cplusplus
} // namespace ulib
#endif
#endif // _ulib_vector_h_
