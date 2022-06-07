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
#include "MyScene/MyScene.h"

/*
   DO: Select
   Select directive object for editing. Light will take keyboard and mouse focus.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_Select(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *object;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact an object; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &object)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the object is the selected object, return true
      if (self->m_selected_object == object) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // self the console
   // self->Agent->Tell(object->reference, 0, "Deactivate console");

   // Set object intercept structure
   self->m_object_intercept.object = object;
   self->m_object_intercept.self = self;
   self->m_last_object = self->m_selected_object = object;

   // Set haptic intercept
   if (self->m_GL->SetMouseIntercept(&MyMouseEvent, &self->m_object_intercept) != TITAN_RESULT_OK) return NO_RESPONSE();
   if (self->m_GL->SetKeyboardIntercept(&MyKeyboardEvent, &self->m_object_intercept) != TITAN_RESULT_OK) return NO_RESPONSE();
   if (self->m_GL->SetPostFrameCallback(0, &DisplayData, &self->m_object_intercept) != TITAN_RESULT_OK) return NO_RESPONSE();

   // Update coordinate for this reference
   self->Location->UpdateCoordinate(object->sim_object, TITAN_COORDINATE_TRS, 0, false);

   // Get latest coordinates of this object
   self->m_current_coordinate_index = 0;
   self->m_num_trail_coordinates = self->m_max_trail_coordinates;
   if (self->Location->GetCoordinates(object->sim_object, self->m_current_coordinate_index, self->m_trail_coordinates, self->m_num_trail_coordinates) == TITAN_RESULT_OK)
      {

      }

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Clear
   Clear directive object color visualization.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_Clear(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *object;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact an object; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &object)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the object is the selected object, return true
      if (self->m_selected_object == object) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Reset object color
   VectorClear4(object->color.f.value, 1.0f);

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Reset
   Reset directive object parameters.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_Reset(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *object;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact an object; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &object)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the object is the selected object, return true
      if (self->m_selected_object == object) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Reset object color
   VectorClear4(object->color.f.value, 1.0f);

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Undo
   Undo last directive object action.

   ** NOTE: This method is a placeholder - No action is currently performed **

   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_Undo(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *object;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact an object; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &object)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the object is the selected object, return true
      if (self->m_selected_object == object) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Redo
   Redo last directive object action.

   ** NOTE: This method is a placeholder - No action is currently performed **

   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_Redo(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *object;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact an object; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &object)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the object is the selected object, return true
      if (self->m_selected_object == object) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Remove
   Remove directive object from its hosting scene.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_Remove(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *object;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact an object; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &object)) == 0) return NO_RESPONSE();
   // Remove the object from the scene
   if (self->m_scene) self->m_scene->Scene->Cancel(self->m_scene, object->reference);

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Move
   Move directive object forward. Move parameters can be added to move object in different directions and relative to other references.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_Move(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *object;
   TITAN_FLOAT                                        *forward;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact an object; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &object)) == 0)
      {
      // Return failure
      return NO_RESPONSE();
      }

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Return 'No answer'
      return NO_RESPONSE();
      }

   // Assign new coordinates
   // object->coordinates.angle.rot.yaw = random();
   forward = &object->transform.f.value[8];
   VectorMA4(forward, 0.1f, object->coordinates.position.f.value, object->coordinates.position.f.value);
   object->coordinates.position.xyzt_f.t = 2.0f;
   object->coordinates.trigger.Trigger = NULL;
   // Update coordinate for this reference
   self->Location->UpdateCoordinate(object->sim_object, TITAN_COORDINATE_TRS, 0, true);

   // Return success
   return TO_RESPONSE(100);
}

/*
   DO: Rotate
   Rotate directive subject or object to an arbitrary angle. Rotate parameters should be expanded to handle degrees, etc.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_Rotate(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *object;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive subject and validate that it is in fact an object; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_SUBJECT, reference_info, &object)) == 0)
      {
      // Get the directive first object (transitive) and validate that it is in fact an object; return failure if unsuccessful
      if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &object)) == 0)
         {
         // Return failure
         return NO_RESPONSE();
         }
      }

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Return 'No answer'
      return NO_RESPONSE();
      }
   // Assign new coordinates
   if (object->coordinates.angle.rot.yaw < 0.523599f) object->coordinates.angle.rot.yaw = 0.523599f + random() * (M_PI_2_F - 0.523599f);
   else object->coordinates.angle.rot.yaw = 0.0f + random() * 0.1f;
   object->coordinates.angle.xyzt_f.t = .5f + random() * 1.0f;
   object->coordinates.trigger.Trigger = NULL;
   // Update coordinate for this reference
   self->Location->UpdateCoordinate(object->sim_object, TITAN_COORDINATE_ROTATE, 0, true);
   // Return 'No answer'
   return NO_RESPONSE();
}

/*
   DO: Stop
   Stop directive object motion.

   ** NOTE: This method is a placeholder - No action is currently performed **

   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_Stop(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *object;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact an object; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &object)) == 0) return NO_RESPONSE();

   // Process verb
   if (object)
      {
      // Return success
      return TO_RESPONSE(100);
      }

   // Return success
   return TO_RESPONSE(100);
}

/*
   DO: Stand
   Stop and rest directive object on top of whatever is below it.

   ** NOTE: This method is a placeholder - No action is currently performed **

   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_Stand(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *data;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Get subject group from directive; return failure if unsuccessful
   if ((component = self->Directive->Get(directive, ATLAS_DIRECTIVE_SUBJECT, reference_info)) == 0) return NO_RESPONSE();

   // Get data from element if it exists; otherwise, use instance data
   if (reference_info.element) data = _RP(object_data_t, reference_info.element);
   else data = _RP(object_data_t, reference_info.instance);

   // Process verb
   if (data)
      {
      // Return success
      return TO_RESPONSE(100);
      }

   // Return success
   return TO_RESPONSE(100);
}

/*
   Do: Destroy
   Destroy directive object reference and its associated instance
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyObject::DO_SYS_Destroy(ATLAS_SIMULATE_PARAMS) 
{ 
   MyObject                                           *self = _RP(MyObject, user_data);
   object_data_t                                      *object;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact an object; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &object)) == 0) return NO_RESPONSE();

   // Destroy the system object and its reference

   self->Reference->Destroy(object->reference);

   // Return true
   return NO_RESPONSE();
}


/*
   End of file
*/
