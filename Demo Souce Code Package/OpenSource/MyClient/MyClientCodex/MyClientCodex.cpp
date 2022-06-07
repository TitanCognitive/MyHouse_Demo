/* 
   Copyright (c) 2020 Titan Virtual Corp. All rights reserved.

   http://www.titanvx.com
   Support: support@titanvx.com
   
   The source code and/or documentation ("Licensed Deliverables") are 
   subject to Titan Virtual Corp. intellectual property rights under U.S. and 
   international Copyright laws. 

   The Licensed Deliverables contained herein are PROPRIETARY and
   CONFIDENTIAL to Titan Virtual Corp. and are being provided under the terms and
   conditions of a form of Titan Virtual Corp. software license agreement by and
   between Titan Virtual Corp. and Licensee ("License Agreement") or electronically
   accepted by Licensee. Notwithstanding any terms or conditions to the contrary 
   in the License Agreement, reproduction or disclosure of the Licensed Deliverables 
   to any third party without the express written consent of Titan Virtual Corp. is prohibited.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
   OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE 
   USE OR OTHER DEALINGS IN THE SOFTWARE.   
   
   Any use of the Licensed Deliverables in individual and commercial software must include, 
   in the user documentation and internal comments to the code, the above Disclaimer 
   and U.S. Government End Users Notice.
*/

// Dependencies
#include "MyClient/MyClient.h"

   // Titan dependencies
   #pragma comment(lib, "../../../Titan/Lib/TitanAxiom.lib")

   // Local dependencies
   #if _DEBUG
      // Use debug library
      #pragma comment(lib, "../../_Bin/x64/Debug/MyClientLib.lib")                            
   #else
      // Use release library
      #pragma comment(lib, "../../_Bin/x64/Release/MyClientLib.lib")
   #endif

/*
   Create a new codex axiom using codex name <codex_name> and axiom name <axiom_name> where <axiom_handle> is the agent-supplied handle of the axiom to be created
   Returns: Pointer to the created axiom upon successful operation
            NULL upon failure to create the axiom
*/
AtlasAxiomInterface *CreateAxiom(const TITAN_STRING codex_name, const TITAN_STRING axiom_name, TITAN_HANDLE64 handshake)
{
   // If the codex name is recognized
   if (!strcmp(codex_name, "MyCodex"))
      {
      // If the axiom name is recognized
      if (!strcmp(axiom_name, "MyClient"))
         {
         new MyClient(handshake);
         // Create a new axiom and return its pointer
         return TO_AXIOM(new MyClient(handshake));
         }
      // Return failure if axiom is not found
      return NULL;
      }

   // Return failure if codex is not found
   return NULL;
}

/*
   End of file 
*/
