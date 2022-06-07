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
#include "../MyScene/MyScene.h"
#include "../MyCamera/MyCamera.h"
#include "../MyConsole/MyConsole.h"
#include "../MyObject/MyObject.h"
#include "../MyLight/MyLight.h"
#include "../MyDoor/MyDoor.h"

   // Titan dependencies
   #pragma comment(lib, "../../../Titan/Lib/TitanAxiom.lib")

   // Local dependencies
   #if _DEBUG
      // Use debug library
      #pragma comment(lib, "../../_Bin/x64/Debug/MyKitchenLib.lib")                            
   #else
      // Use release library
      #pragma comment(lib, "../../_Bin/x64/Release/MyKitchenLib.lib")
   #endif

// Set up GL libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment( lib, "glu32.lib" )


/*
   Create a new codex axiom using codex name <codex_name> and axiom name <axiom_name> where <axiom_handle> is the agent-supplied handle of the axiom to be created
   Returns: Pointer to the created axiom upon successful operation
            NULL upon failure to create the axiom
*/
AtlasAxiomInterface *CreateAxiom(const TITAN_STRING codex_name, const TITAN_STRING axiom_name, TITAN_HANDLE64 handshake)
{
   // Create a new codex and return its pointer
   if (!strcmp(codex_name, "Titan"))
      {
      // Get object axiom
      if (!strcmp(axiom_name, "MyObject"))
         {
         // Create an object axiom; return failure if unsuccessful
         return TO_AXIOM(new MyObject(handshake));
         }
      // Get light axiom
      if (!strcmp(axiom_name, "MyLight"))
         {
         // Create a light axiom; return failure if unsuccessful
         return TO_AXIOM(new MyLight(handshake));
         }
      // Get door axiom
      if (!strcmp(axiom_name, "MyDoor"))
         {
         // Create a door axiom; return failure if unsuccessful
         return TO_AXIOM(new MyDoor(handshake));
         }      
      // Get camera axiom
      if (!strcmp(axiom_name, "MyCamera"))
         {
         // Create a camera axiom; return failure if unsuccessful
         return TO_AXIOM(new MyCamera(handshake));
         }
      // Get scene axiom
      if (!strcmp(axiom_name, "MyScene"))
         {
         // Create a scene axiom; return failure if unsuccessful
         return TO_AXIOM(new MyScene(handshake));
         }
      // Get console axiom
      if (!strcmp(axiom_name, "MyConsole"))
         {
         // Create a console axiom; return failure if unsuccessful
         return TO_AXIOM(new MyConsole(handshake));
         }
      // Return failure
      return NULL;
      }

   // Return failure otherwise
   return NULL;
}

/*
   End of file 
*/
