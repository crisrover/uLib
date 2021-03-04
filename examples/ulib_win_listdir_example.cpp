/*

Copyright (c) 2021, Croitor Cristian

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

#define IMPLEMENTATION
#include "ulib_win_listdir.h"

static void FileCallBack(TCHAR* pth, TCHAR* filename)
{
    //_tprintf(_T("%s\r\n"), filename);
}

int main(int, char**)
{
    static ulib::ListDirData listDirData;
    INIT_LISTDIRDATA(listDirData);
    listDirData.processFile = FileCallBack;
    listDirData.processDirectory = FileCallBack;
    listDirData.recurse = ULIB_TRUE;
    listDirData.dir = (_TCHAR*)_T("c:\\");
    ListDir(&listDirData);
    _tprintf(_T("Total files on disk: %llu \r\n"), listDirData.totalFiles);
    _tprintf(_T("Total folders on disk: %llu \r\n"), listDirData.totalDirs);
    return (ULIB_SUCCESS);
}
