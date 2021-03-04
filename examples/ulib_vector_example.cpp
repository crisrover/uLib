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
#include "ulib_vector.h"

struct Data
{
    int firstEntry;
    float secondEntry;
    short thirdEntry;
};

int main(int, char**)
{
    Data data = { 0 };
    ulib::ulib_vector vec;
    ulib::ulib__SizeType bufferSize = ULIB_KILOBYTE; // should be enough
    ulib::ulib__SizeType elementSize = sizeof(Data);
    INIT_ULIB_VECTOR(vec, bufferSize, elementSize);

    float f = 15.0f;
    short s = 10;
    for (int i = 0; i < 10; ++i, --s)
    {
        f += 0.10f;
        data.firstEntry = i;
        data.secondEntry = f;
        data.thirdEntry = s;
        ulib::UlibVectorPush(&vec, &data, elementSize);
    }
    data.firstEntry = 124;
    data.secondEntry = 17.34f;
    data.thirdEntry = 234;
    ulib::UlibVectorPushElement(&vec, &data);

    while (ulib::UlibVectorPop(&vec, &data) == ULIB_SUCCESS)
    {
        printf("data.firstEntry: %d\n", data.firstEntry);
        printf("data.secondEntry: %f\n", data.secondEntry);
        printf("data.thirdEntry: %d\n", data.thirdEntry);
    }

    return (ULIB_SUCCESS);
}
