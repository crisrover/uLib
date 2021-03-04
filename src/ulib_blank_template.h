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

#ifndef ulib_blank_template_h
#define ulib_blank_template_h
#include <ulib/src/ulib_common.h>

#ifdef __cplusplus
namespace ulib{
#endif

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
* Function:

            ulib__uint8 Function(params);

* Parameters:
*      Input:

*      Return:
******************************************************************************/
    ulib__uint8 Function(params);
#ifdef __cplusplus
} // extern "C" {
#endif
#ifdef IMPLEMENTATION
ulib__uint8 Function(params){

}
#endif // #ifdef IMPLEMENTATION
#ifdef __cplusplus // namespace ulib{
}
#endif
#endif // #ifndef ulib_blank_template_h
