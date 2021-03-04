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

#ifndef _ulib_win_api_h_
#define _ulib_win_api_h_
#include "ulib_common.h"
#ifdef _MSC_VER
#pragma warning(push, 0)
#include <tlhelp32.h> // Wall warnings
#pragma warning(pop)
#else
#include <tlhelp32.h> // This is only for compiling with gcc under Windows
#endif

/******************************************************************************
* Public functions
*
* DWORD GetPid(IN const _TCHAR* processName)
* BOOL KillProcess(IN const DWORD pid)
* _TCHAR* UlibGetSystemLastErrorString(void)
* static void SetControlCHandler(void)
******************************************************************************/

#ifdef __cplusplus
namespace ulib{
#endif
#ifdef __cplusplus
extern "C"{
#endif

#ifdef ULIB_SET_CONTROL_C_HANDLER
    ULIB_EXTERN volatile ulib__bool ulib_CTRL_C_Handler_shouldExit;
    ULIB_EXTERN volatile ulib__bool ulib_CTRL_C_Handler_okToExit;
#endif
/******************************************************************************
* Function:
*           DWORD GetPid(IN const _TCHAR* processName)
* Retrieves the Pid for a known process name.
* Parameters:
*       Input:  processName - The process name as _TCHAR*
* Return value:
*       Pid or ULIB_NO_SUCCESS
******************************************************************************/
    DWORD GetPid(IN const _TCHAR* processName);

/******************************************************************************
* Function:
*           BOOL KillProcess(const DWORD pid)
* Kills the process with the given pid
* Parameters:
*       Input:  pid - The process pid
* Return: On success a non-zero value is returned - from MSDN! as.. BOOL!
*         ULIB_FALSE is returned if the function fails
* _UlibGetSystemLastErrorString can be used in the case of function fail
* to get the last system error as string.
******************************************************************************/
    BOOL KillProcess(IN const DWORD pid);

/******************************************************************************
* Function:
*           _TCHAR* UlibGetSystemLastErrorString(void)
* Returns a _TCHAR* containing the string representation of last Windows error
* The _TCHAR* ***MUST*** be freed after using it with LocalFree()
* Parameters:
* Return: _TCHAR*
******************************************************************************/
    _TCHAR* UlibGetSystemLastErrorString(void);

#ifdef __cplusplus
} // extern "C" {
#endif

#ifdef IMPLEMENTATION
_TCHAR* _UlibGetSystemLastErrorString(void){
    ulib__uint32 dw = GetLastError();
    _TCHAR* messageBuffer;
    if (!FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                        FORMAT_MESSAGE_FROM_SYSTEM |
                        FORMAT_MESSAGE_IGNORE_INSERTS |
                        FORMAT_MESSAGE_MAX_WIDTH_MASK,
                        ULIB_NULL,
                        dw,
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                        (LPTSTR)&messageBuffer,
                        0, ULIB_NULL)){
        if (messageBuffer){
            LocalFree(messageBuffer);
            return((_TCHAR*)(ULIB_NO_SUCCESS));
        }
    } // if (!FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
    return(messageBuffer);
}

DWORD GetPid(const _TCHAR* processName){
     PROCESSENTRY32 pe32;
     HANDLE processSnapshot;
     if (processName == ULIB_NULL){
         return((DWORD)ULIB_NO_SUCCESS);
     }
     // Take a snapshot of all processes in the system.
     processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
     if (processSnapshot == INVALID_HANDLE_VALUE){
         return((DWORD)ULIB_NO_SUCCESS);
     }
     pe32.dwSize = sizeof(PROCESSENTRY32);
     //Retrieve information about the first process, and exit if unsuccessful
     if (!Process32First(processSnapshot, &pe32)){
         CloseHandle(processSnapshot);
         return((DWORD)ULIB_NO_SUCCESS);
     }
     do{
         if (_tcsstr(pe32.szExeFile, processName) != ULIB_NULL){
             CloseHandle(processSnapshot);
             return(pe32.th32ProcessID);
         }
     } while (Process32Next(processSnapshot, &pe32));
     return((DWORD)ULIB_NO_SUCCESS);
}

BOOL KillProcess(const DWORD pid){
    BOOL res = ULIB_FALSE;
    HANDLE process = OpenProcess(
                PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                0,
                pid);
    if (process != ULIB_NULL){
        res = TerminateProcess(process, 0);
        CloseHandle(process);
    }
    return(res);
}
#endif // #ifdef IMPLEMENTATION

#ifdef ULIB_SET_CONTROL_C_HANDLER
volatile ulib__bool ulib_CTRL_C_Handler_shouldExit = ULIB_FALSE;
volatile ulib__bool ulib_CTRL_C_Handler_okToExit = ULIB_FALSE;

/******************************************************************************
* Function (internal):
*          static BOOL __stdcall CtrlHandler(DWORD controlType)
* This is the callback for a HandlerRoutine for Windows
* Subtlety:
* DO NOT USE MUTEX IN SIGNAL HANDLING FUNCTIONS!
* When a CTRL + C interrupt occurs, Win32 operating systems generates a new
* thread to specifically handle that interrupt.
* Because signal - handler routines are usually called asynchronously when an
* interrupt occurs, your signal - handler function may get control when a
* run - time operation is incomplete and in an unknown state.
* After CtrlHandler exit, Windows will unconditionally terminate your
* process without giving your code any chance for cleanup.
* So, here two volatile variables are used, one to indicate to the rest of
* the program that it should exit, due to CTRL-C, and the other one to wait
* for the program to cleanup.
*
* Parameters:
    controlType - this function monitors CTRL_C_EVENT || CTRL_BREAK_EVENT
* Return value: On success, ULIB_TRUE is returned
* While ulib_CTRL_C_Handler_okToExit is false the function never returns
******************************************************************************/
static BOOL __stdcall CtrlHandler(DWORD controlType){
    // Handle the CTRL-C signal.
    if (controlType == CTRL_C_EVENT || controlType == CTRL_BREAK_EVENT){
        _TLOG(_T("\r\nCtrl-C detected"));
        ulib_CTRL_C_Handler_shouldExit = ULIB_TRUE;
    }
    while (ulib_CTRL_C_Handler_okToExit == ULIB_FALSE){
        Sleep(10u);
    }
    return (ULIB_TRUE);
}

/******************************************************************************
* Function:
*           static void SetControlCHandler(void)
* Sets the CtrlHandler routine
* Parameters:
* Return value: Nothing
******************************************************************************/
static void SetControlCHandler(void){
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)){
        _TLOG_WARNING(_T("could not set CTRL-C signal handler"));
    }
}
#endif // #ifdef ULIB_SET_CONTROL_C_HANDLER
#ifdef __cplusplus // namespace ulib{
}
#endif
#endif // #ifndef _ulib_win_api_h_
