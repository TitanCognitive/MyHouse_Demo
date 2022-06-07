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
#define TITAN_SYSTEM_FILE
#include TITAN_DEPENDENCIES

// Local dependencies
#include "MyCamera/MyCamera.h"

/*
   Attach camera axiom to the knowledge base
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/ 
titan_result_t MyCamera::AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept_handle, const TITAN_STRING attach_params)
{  
   titan_result_t                                     result;

   // Get system dependencies   
   // If the memory interface is not initialized; request a new interace
   if (Memory == NULL) return TITAN_RESULT_NOT_INITIALIZED;
   // If the tools interface is not initialized; request a new interace
   if (Tools == NULL) return TITAN_RESULT_NOT_INITIALIZED;

   // Remember concept
   m_concept = concept_handle;

   // BE: Camera states
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Active", 0, TO_HANDLE64(this), &BE_Active)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Fast", 0, TO_HANDLE64(this), &BE_Fast)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Slow", 0, TO_HANDLE64(this), &BE_Slow)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Current", 0, TO_HANDLE64(this), &BE_Current)) != TITAN_RESULT_OK) return result;
   // DO: Camera actions
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Select", 0, TO_HANDLE64(this), &DO_Select)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_SUBJECT, "Track", 0, TO_HANDLE64(this), &DO_Track)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_SUBJECT, "Follow", 0, TO_HANDLE64(this), &DO_Follow)) != TITAN_RESULT_OK) return result;

   // Set camera concept instantiator; return failure if unsuccessful
   if ((result = Cognitive->SetInstantiator(TO_INSTANTIATOR(this))) != TITAN_RESULT_OK) return result;

   // Register format label; return failure if unsuccessful
   if ((m_camera_interface_label = Label->GetHandle("Camera Interface")) == 0) return TITAN_RESULT_INSERTION_FAIL;

   // Get simulation object attribute interfaces
   if ((Location = _S(AtlasAttributeLocationAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_LOCATION))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
   if ((Size = _S(AtlasAttributeSizeAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_SIZE))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
   if ((Color = _S(AtlasAttributeColorAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_COLOR))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Attach camera axiom to the simulation
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyCamera::AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept_handle)
{
   // Create a pure abstract camera instance; return failure if unsuccessful
   if ((m_default.reference = Reference->Add(caller_id, "MyCameraInterface", m_concept, 0)) == 0) return TITAN_RESULT_INSERTION_FAIL;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Link camera axiom to other axioms in the simulation
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyCamera::AtlasSim_Link(TITAN_HANDLE64 concept_handle)
{
   atlas_value_t                                      value;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     label_handle;

   // Get the first label matching 'Construct'
   if ((label_handle = Label->GetFirst(m_default.reference, "ApplicationWindow", reference_info, ATLAS_GET_REFERENCE)) != NULL)
      {
      // If the reference is valid, get the video interface from the label reference; otherwise, return a failure
      if (reference_info.reference == 0 || (Label->GetValue(reference_info.reference, "Video Interface", value) != TITAN_RESULT_OK)) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      // Get GL interface; return failure if unsuccessful
      if (value.amount.format_handle == TITAN_UINT_ERROR || (m_GL = _SP(MyGLAPI, value.amount.raw_value)) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      }

   // Get console interface
   if ((label_handle = Label->GetFirst(m_default.reference, "User Console", reference_info, ATLAS_GET_REFERENCE)) != NULL)
      {
      // If the reference is valid, get the console interface from the label reference; otherwise, return a failure
      if (reference_info.reference == 0 || (Label->GetValue(reference_info.reference, "Console Interface", value) != TITAN_RESULT_OK)) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      // If the console is available
      if (value.amount.handle_value)
         {
         // Register keywords; return failure if unsuccessful
         // if (Ledger->RegisterKeyword(value.amount.handle_value, "LoadObject", TO_HANDLE64(this), &Key_LoadObject) == 0) return TITAN_RESULT_INIT_FAIL;
         }
      }

   // Grab the scene interface
   if ((label_handle = Label->GetFirst(m_default.reference, "Kitchen", reference_info, ATLAS_GET_REFERENCE)) != NULL)
      {
      // If the reference is valid, get the video interface from reference; otherwise, return a failure
      if (reference_info.reference == 0 || (Label->GetValue(reference_info.reference, "Scene Interface", value) != TITAN_RESULT_OK)) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      // Get scene interface; return failure if unsuccessful
      if (value.amount.format_handle == TITAN_UINT_ERROR || (m_scene = _SP(scene_data_t, value.amount.raw_value)) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      }

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Get the camera instance value, specified by format <value.format_handle> from supplied <instance> and <element> pair, where:
      <value> is the value retrieved
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyCamera::AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake)
{
   // If the camera interface is requested
   if (value.amount.format_handle == m_camera_interface_label)
      {
      // Set the window camera interface value
      value.amount.format_handle = 0;
      value.amount.raw_value = &m_active_camera;
      // Return success
      return TITAN_RESULT_OK;
      }

   // Return failure otherwise
   return TITAN_RESULT_ERROR;
}

/*
   End of file
*/
