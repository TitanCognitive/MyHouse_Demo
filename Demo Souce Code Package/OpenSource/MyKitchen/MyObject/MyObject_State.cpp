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
#include "MyObject/MyObject.h"

/*
   Attach system to the context
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/ 
titan_result_t MyObject::AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept_handle, const TITAN_STRING attach_params)
{  
   titan_result_t                                     result;

   // Get system dependencies   
   // If the memory interface is not initialized; request a new interace
   if (Memory == NULL) return TITAN_RESULT_NOT_INITIALIZED;
   // If the tools interface is not initialized; request a new interace
   if (Tools == NULL) return TITAN_RESULT_NOT_INITIALIZED;

   // Remember concept
   m_concept = concept_handle;

   // Initialize the trail coordinates; return failure if unsuccessful
   if (TITAN_ALLOCATE_FLUSH_ARRAY(m_trail_coordinates, atlas_coordinate_t, m_max_trail_coordinates) == false) return TITAN_RESULT_NO_MEMORY;
   m_num_trail_coordinates = 0;

   // Set simulation methods for the lamp
   // BE block setup   

   // Be: Color
   // Be: Size
   // Be: Abstract
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Last", 0, TO_HANDLE64(this), &BE_Last)) != TITAN_RESULT_OK) return result;
   // DO: Motion
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Select", 0, TO_HANDLE64(this), &DO_Select)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Clear", 0, TO_HANDLE64(this), &DO_Clear)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Reset", 0, TO_HANDLE64(this), &DO_Reset)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Undo", 0, TO_HANDLE64(this), &DO_Undo)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Redo", 0, TO_HANDLE64(this), &DO_Redo)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Move", 0, TO_HANDLE64(this), &DO_Move)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Rotate", 0, TO_HANDLE64(this), &DO_Rotate)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Slide", 0, TO_HANDLE64(this), &DO_Move)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Stop", 0, TO_HANDLE64(this), &DO_Stop)) != TITAN_RESULT_OK) return result;
   // DO: Rest
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Sit", 0, TO_HANDLE64(this), &DO_Stand)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Rest", 0, TO_HANDLE64(this), &DO_Stand)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Stand", 0, TO_HANDLE64(this), &DO_Stand)) != TITAN_RESULT_OK) return result;
   // DO: System commands
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "/Destroy", 0, TO_HANDLE64(this), &DO_SYS_Destroy)) != TITAN_RESULT_OK) return result;

   // Set instantiator; return failure if unsuccessful
   if ((result = Cognitive->SetInstantiator(TO_INSTANTIATOR(this))) != TITAN_RESULT_OK) return result;
   
   // Create or retrieve interface labels; return failure if unsuccessful
   if (m_coordinate_interface_label == 0 && (m_coordinate_interface_label = Label->GetHandle("Coordinate Interface")) == 0) return TITAN_RESULT_INSERTION_FAIL;
   if (m_coordinate_label == 0 && (m_coordinate_label = Label->GetHandle("Coordinate")) == 0) return TITAN_RESULT_INSERTION_FAIL;
   if (m_transform_label == 0 && (m_transform_label = Label->GetHandle("Transform")) == 0) return TITAN_RESULT_INSERTION_FAIL;
   if (m_color_label == 0 && (m_color_label = Label->GetHandle("Color")) == 0) return TITAN_RESULT_INSERTION_FAIL;

   // Get simulation object attribute interfaces
   if ((Location = _S(AtlasAttributeLocationAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_LOCATION))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
   if ((Size = _S(AtlasAttributeSizeAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_SIZE))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
   if ((Color = _S(AtlasAttributeColorAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_COLOR))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
   if ((Shape = _S(AtlasAttributeShapeAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_SHAPE))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
   if ((Texture = _S(AtlasAttributeTextureAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_TEXTURE))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Attach object axiom to the simulation
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyObject::AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept_handle)
{
   // Create a pure abstract object instance; return failure if unsuccessful
   if ((m_reference = Reference->Add(caller_id, NULL, m_concept, 0)) == 0) return TITAN_RESULT_INSERTION_FAIL;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Link object axiom to other axioms in the simulation
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure*/
titan_result_t MyObject::AtlasSim_Link(TITAN_HANDLE64 concept_handle)
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

   // Get console interface
   if ((label_handle = Label->GetFirst(m_reference, "User Console", reference_info, ATLAS_GET_REFERENCE)) != NULL)
      {
      // If the reference is valid, get the video interface from reference; otherwise, return a failure
      if (reference_info.reference == 0 || (Label->GetValue(reference_info.reference, "Console Interface", value) != TITAN_RESULT_OK)) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      // If the console is available
      if (value.amount.handle_value)
         {
         // TODO: Register keywords 
         }
      }

   // Grab the camera interface
   if ((label_handle = Label->GetFirst(m_reference, "MyCameraInterface", reference_info, ATLAS_GET_REFERENCE)) != NULL)
      {
      // If the reference is valid, get the video interface from reference; otherwise, return a failure
      if (reference_info.reference == 0 || (Label->GetValue(reference_info.reference, "Camera Interface", value) != TITAN_RESULT_OK)) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      // Get GL interface; return failure if unsuccessful
      if (value.amount.format_handle == TITAN_UINT_ERROR || (m_camera = _SP(camera_data_t *, value.amount.raw_value)) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      }

   // Grab the scene interface
   if ((label_handle = Label->GetFirst(m_reference, "Kitchen", reference_info, ATLAS_GET_REFERENCE)) != NULL)
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
   End of file
*/
