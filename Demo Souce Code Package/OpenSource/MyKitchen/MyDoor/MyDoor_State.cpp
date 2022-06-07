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
#include "MyDoor/MyDoor.h"

/*
   Attach door axiom to the knowledge base
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/ 
titan_result_t MyDoor::AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept_handle, const TITAN_STRING attach_params)
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
   // Be: state
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Open", 0, TO_HANDLE64(this), &BE_Opened)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Closed", 0, TO_HANDLE64(this), &BE_Closed)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Inverted", 0, TO_HANDLE64(this), &BE_Inverted)) != TITAN_RESULT_OK) return result;
   // DO: action
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Open", 0, TO_HANDLE64(this), &DO_Open)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Close", 0, TO_HANDLE64(this), &DO_Close)) != TITAN_RESULT_OK) return result;
   // DO: System commands
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "/Destroy", 0, TO_HANDLE64(this), &DO_SYS_Destroy)) != TITAN_RESULT_OK) return result;

   // Set instantiator; return failure if unsuccessful
   if ((result = Cognitive->SetInstantiator(TO_INSTANTIATOR(this))) != TITAN_RESULT_OK) return result;

   // Create or retrieve interface labels; return failure if unsuccessful
   if (m_coordinate_interface_label == 0 && (m_coordinate_interface_label = Label->GetHandle("Coordinate Interface")) == 0) return TITAN_RESULT_INSERTION_FAIL;
   if (m_coordinate_label == 0 && (m_coordinate_label = Label->GetHandle("Coordinate")) == 0) return TITAN_RESULT_INSERTION_FAIL;
   if (m_transform_label == 0 && (m_transform_label = Label->GetHandle("Transform")) == 0) return TITAN_RESULT_INSERTION_FAIL;
   if (m_color_label == 0 && (m_color_label = Label->GetHandle("Color")) == 0) return TITAN_RESULT_INSERTION_FAIL;

   // Get concept of the door frame (sub-concept); return failure if unsuccessful
   if ((m_sub_frame = Cognitive->GetSubconcept("Frame")) == 0) return TITAN_RESULT_RESOURCE_NOT_FOUND;
   // Set the instantiator of the door frame concept to be handled that door frame concept itself; return failure if unsuccessful
   if ((result = Cognitive->SetSubinstantiator("Frame", m_sub_frame)) != TITAN_RESULT_OK) return result;

   // Get concept of the door panel (sub-concept); return failure if unsuccessful
   if ((m_sub_panel = Cognitive->GetSubconcept("Panel")) == 0) return TITAN_RESULT_RESOURCE_NOT_FOUND;
   // Set the instantiator of the door panel concept to be handled that door panel concept itself; return failure if unsuccessful
   if ((result = Cognitive->SetSubinstantiator("Panel", m_sub_panel)) != TITAN_RESULT_OK) return result;
  
   // Return success
   return TITAN_RESULT_OK;
}

/*
   Attach door axiom to the simulation
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyDoor::AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept_handle)
{
   atlas_value_t                                      value;
   
   // Get door frame coordinate interface from its sub concept; return failure if unsuccessful
   value.amount.format_handle = m_coordinate_interface_label;
   if (Instance->GetValue(m_sub_frame, value, 0, 0) != TITAN_RESULT_OK) return TITAN_RESULT_INSERTION_FAIL;
   m_SubFrameCoordinate = _R(AtlasAttributeLocationAPI, value.amount.raw_value);

   // Get door panel coordinate interface from its sub concept; return failure if unsuccessful
   value.amount.format_handle = m_coordinate_interface_label;
   if (Instance->GetValue(m_sub_panel, value, 0, 0) != TITAN_RESULT_OK) return TITAN_RESULT_INSERTION_FAIL;
   m_SubPanelCoordinate = _R(AtlasAttributeLocationAPI, value.amount.raw_value);

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Link door axiom to other axioms in the simulation
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyDoor::AtlasSim_Link(TITAN_HANDLE64 concept_handle)
{
   atlas_value_t                                      value;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     label_handle;

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
