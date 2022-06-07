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

// Initialize the static variables
TITAN_HANDLE64                                        MyDoor::m_coordinate_interface_label = NULL;
TITAN_HANDLE64                                        MyDoor::m_coordinate_label = NULL;
TITAN_HANDLE64                                        MyDoor::m_transform_label = NULL;
TITAN_HANDLE64                                        MyDoor::m_color_label = NULL;
/*
   Class constructor
*/
MyDoor::MyDoor(TITAN_HANDLE64 handshake) : TitanAxiom(handshake)
{
   // Initialize framework
   // Initialize variables
   m_reference = 0;
   m_sub_frame = 0;
   m_sub_panel = 0;

   // Initialize pointers
   m_scene = NULL;
   m_SubFrameCoordinate = NULL;
   m_SubPanelCoordinate = NULL;
}

/*
   Class destructor
*/
MyDoor::~MyDoor()
{
}

/*
   Get element specified by component id <component_id> of supplied directive <directive>, where:
      <reference_info> is the structure used to retrieve reference data of the first component matching <component_id> in the supplied directive 
      <instance_data> is the output pointer to the instance of the directive component
      <query_flags> is the parameter passed to the directive interface Get (See Directive::Get in Atlas SDK)

   Returns: Handle of the directive component retrieved upon success
            0 upon failure to retrieve the requested directive component
*/
TITAN_HANDLE64 MyDoor::GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, 
                                  door_data_t **instance_data, TITAN_BITMAP64 query_flags)
{
   TITAN_HANDLE64                                     component;

   // Get the directive component; return failure if unsuccessful
   if ((component = Directive->Get(directive, component_id, reference_info, query_flags | ATLAS_GET_INSTANCE | ATLAS_GET_REFERENCE)) == 0) return 0;
   // Make sure the returned reference can be handled by this concept; return failure otherwise
   if (Cognitive->IsCompatible(reference_info.reference) != true) return 0;

   // If instance data is requested
   if (instance_data)
      {
      // Get reference instance and/or element
      if (reference_info.element) *instance_data = _RP(door_data_t, reference_info.element);
      else if ((*instance_data = _RP(door_data_t, reference_info.instance)) == NULL) return 0;
      }

   // Return component handle
   return component;
}

/*
   Open the door
*/
atlas_probe_result_t MyDoor::OpenDoor(door_data_t *door, TITAN_UBYTE query_type)
{
   glm::vec3                                          forward, up, right;
   TITAN_FLOAT                                        angle;
   TITAN_FLOAT                                        *mat;

   // Get the transform matrix
   mat = door->panel.transform->f.value;

   // Get the local direction vectors
   right = glm::normalize(glm::vec3(mat[0], mat[4], mat[8]));
   up = glm::normalize(glm::vec3(mat[1], mat[5], mat[9]));
   forward = glm::normalize(glm::vec3(mat[2], mat[6], mat[10]));
   angle = acosf(glm::dot(right, glm::vec3(1.0f, 0.0f, 0.0f)));

   // If this expression is a query
   if (query_type == ATLAS_EXP_QUERY)
      {
      // If the door angle is greater than 30 degrees
      if (angle > .523599f)
         {
         // Return true (the door is indeed open)
         return TO_RESPONSE(100);
         }
      // Otherwise, the door is closed
      return TO_RESPONSE(-100);
      }

   // Update the object coordinate
   if (door->inverted) door->panel.coordinate->angle.rot.yaw = -M_PI_2_F;
   else door->panel.coordinate->angle.rot.yaw = M_PI_2_F;
   door->panel.coordinate->angle.xyzt_f.t = .2f + random() * .5f;
   m_SubPanelCoordinate->UpdateCoordinate(door->panel.sim_object, TITAN_COORDINATE_ROTATE, 0, true);
   // FIXME: self->Agent->Tell(door->reference, 0, "Rotate the door");

   // Return true
   return TO_RESPONSE(100);
}

/*
   Close the door
*/
atlas_probe_result_t MyDoor::CloseDoor(door_data_t *door, TITAN_UBYTE query_type)
{
   glm::vec3                                          forward, up, right;
   TITAN_FLOAT                                        angle;
   TITAN_FLOAT                                        *mat;

   // Get the transform matrix
   mat = door->panel.transform->f.value;

   // Get the local direction vectors
   right = glm::normalize(glm::vec3(mat[0], mat[4], mat[8]));
   up = glm::normalize(glm::vec3(mat[1], mat[5], mat[9]));
   forward = glm::normalize(glm::vec3(mat[2], mat[6], mat[10]));
   angle = acosf(glm::dot(right, glm::vec3(1.0f, 0.0f, 0.0f)));

   // If this expression is a query
   if (query_type == ATLAS_EXP_QUERY)
      {
      // If the door angle is greater than 30 degrees
      if (angle > .523599f)
         {
         // Return true (the door is indeed open)
         return TO_RESPONSE(100);
         }
      // Otherwise, the door is closed
      return TO_RESPONSE(-100);
      }

   // Update the object coordinate
   door->panel.coordinate->angle.rot.yaw = 0.0f;
   door->panel.coordinate->angle.xyzt_f.t = .2f + random() * .5f;
   m_SubPanelCoordinate->UpdateCoordinate(door->panel.sim_object, TITAN_COORDINATE_ROTATE, 0, true);

   // Return true
   return TO_RESPONSE(100);
}

/*
   Create an instance of sub concept <sub_concept> and store its results in supplied sub-object structure <sub_object>
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyDoor::CreateSubObjectInstance(TITAN_HANDLE64 reference, TITAN_HANDLE64 sub_concept, door_object_t *sub_object, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake)
{
   atlas_value_t                                      value;

   // Create a sub-concept instance; return failure if unsuccessful
   if (Instance->Create(reference, sub_concept, sub_object->instance, num_elements, params_handle, handshake) != TITAN_RESULT_OK) return TITAN_RESULT_INSERTION_FAIL;

   // Get door position from its sub concept; return failure if unsuccessful
   value.amount.format_handle = m_transform_label;
   if (Instance->GetValue(sub_concept, value, sub_object->instance, 0, handshake) != TITAN_RESULT_OK) return TITAN_RESULT_NO_DATA;
   sub_object->transform = _RP(titan_mat4_t, value.amount.raw_value);

   // Get door color from its sub concept; return failure if unsuccessful
   value.amount.format_handle = m_color_label;
   if (Instance->GetValue(sub_concept, value, sub_object->instance, 0, handshake) != TITAN_RESULT_OK) return TITAN_RESULT_NO_DATA;
   sub_object->color = _RP(titan_vec4_t, value.amount.raw_value);

   // Get light position from its sub concept; return failure if unsuccessful
   value.amount.format_handle = m_coordinate_label;
   if (Instance->GetValue(sub_concept, value, sub_object->instance, 0, handshake) != TITAN_RESULT_OK) return TITAN_RESULT_NO_DATA;
   sub_object->coordinate = _RP(atlas_coordinate_t, value.amount.raw_value);

   // Get the instance objects of the door; return failure if unsuccessful
   if ((Instance->GetSimulationObject(sub_concept, sub_object->sim_object, sub_object->instance, 0, handshake)) != TITAN_RESULT_OK) return TITAN_RESULT_NO_DATA;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Create an instance of sub concept <sub_concept> and store its results in supplied sub-object structure <sub_object>
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyDoor::GetSubObjectInstance(TITAN_HANDLE64 sub_concept, door_object_t *object, TITAN_HANDLE64 handshake)
{
   atlas_value_t                                      value;

   // Get the simulation object the specified door sub-concept; return failure if unsuccessful
   if ((Instance->GetSimulationObject(sub_concept, object->sim_object, object->instance, 0, handshake)) != TITAN_RESULT_OK) return TITAN_RESULT_NO_DATA;

   // Get door position from this sub concept simulation object; return failure if unsuccessful
   value.amount.format_handle = m_transform_label;
   if (Instance->GetValue(sub_concept, value, object->instance, 0, handshake) != TITAN_RESULT_OK) return TITAN_RESULT_NO_DATA;
   object->transform = _RP(titan_mat4_t, value.amount.raw_value);

   // Get door color from this sub concept simulation object; return failure if unsuccessful
   value.amount.format_handle = m_color_label;
   if (Instance->GetValue(sub_concept, value, object->instance, 0, handshake) != TITAN_RESULT_OK) return TITAN_RESULT_NO_DATA;
   object->color = _RP(titan_vec4_t, value.amount.raw_value);

   // Get door coordinate structure from this sub concept simulation object; return failure if unsuccessful
   value.amount.format_handle = m_coordinate_label;
   if (Instance->GetValue(sub_concept, value, object->instance, 0, handshake) != TITAN_RESULT_OK) return TITAN_RESULT_NO_DATA;
   object->coordinate = _RP(atlas_coordinate_t, value.amount.raw_value);

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Destroy door instance data
*/
void MyDoor::Destroy(door_data_t *door)
{
   // Make sure the input data is valid; return otherwise
   if (door == NULL) return;

   // Destroy sub objects
   if (door->frame.instance) Instance->Destroy(m_sub_frame, door->frame.instance);
   if (door->panel.instance) Instance->Destroy(m_sub_frame, door->panel.instance);

   // Destroy door data
   TITAN_FREE(door);
}

/*
   End of file
*/
