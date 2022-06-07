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
#include "Common/Titan.h"

// Titan framework dependencies
#define TITAN_SYSTEM_MEMORY
#define TITAN_SYSTEM_TOOLS
#include TITAN_DEPENDENCIES

// Dependencies
#include "MyScene/MyScene.h"

/*
   Attach system to the context
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/ 
titan_result_t MyScene::AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept_handle, const TITAN_STRING attach_params)
{  
   titan_result_t                                     result;

   // Get system dependencies   
   // If the memory interface is not initialized; request a new interace
   if (Memory == NULL) return TITAN_RESULT_NOT_INITIALIZED;
   // If the tools interface is not initialized; request a new interace
   if (Tools == NULL) return TITAN_RESULT_NOT_INITIALIZED;

   // Remember concept
   m_concept = concept_handle;

   // Set simulation methods for the lamp
   // BE block setup   

   // Be:
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Active", 0, TO_HANDLE64(this), &BE_Active)) != TITAN_RESULT_OK) return result;
   // DO:
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Remember", 0, TO_HANDLE64(this), &DO_Remember)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Recall", 0, TO_HANDLE64(this), &DO_Recall)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Forget", 0, TO_HANDLE64(this), &DO_Forget)) != TITAN_RESULT_OK) return result;

   // Set instantiator; return failure if unsuccessful
   if ((result = Cognitive->SetInstantiator(TO_INSTANTIATOR(this))) != TITAN_RESULT_OK) return result;
   
   // Create or retrieve scene interface label; return failure if unsuccessful
   if ((m_scene_interface_label = Label->GetHandle("Scene Interface")) == 0) return TITAN_RESULT_INSERTION_FAIL;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Attach axiom to simulation
*/
titan_result_t MyScene::AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept_handle)
{
   // Create a scene; return failure if unsuccessful
   printf("Creating a scene instance named 'Kitchen'.\n");

   // Create a main reference; return failure if unsuccessful
   if ((m_reference = Reference->Create(caller_id, "Kitchen", concept_handle)) == 0) return TITAN_RESULT_INIT_FAIL;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Link axiom
*/
titan_result_t MyScene::AtlasSim_Link(TITAN_HANDLE64 concept_handle)
{
   atlas_value_t                                      value;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     label_handle;

   // Grab the construct interface
   if ((label_handle = Label->GetFirst(m_reference, "ApplicationWindow", reference_info, ATLAS_GET_REFERENCE)) != NULL)
      {
      // If the reference is valid, get the video interface from reference; otherwise, return a failure
      if (reference_info.reference == 0 || (Label->GetValue(reference_info.reference, "Video Interface", value) != TITAN_RESULT_OK)) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      // Get GL interface; return failure if unsuccessful
      if (value.amount.format_handle == TITAN_UINT_ERROR || (m_GL = _SP(MyGLAPI, value.amount.raw_value)) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      }

   // Return success
   return TITAN_RESULT_OK;
}

/*
   End of file
*/
