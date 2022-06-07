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

// Local dependencies
#include "MyCamera/MyCamera.h"
#include "MyConsole/MyConsole.h"

/*
   Do: Select
   Select directive object camera to be the current rendering perspective
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyCamera::DO_Select(ATLAS_SIMULATE_PARAMS) 
{ 
   MyCamera                                           *self = _RP(MyCamera, user_data);
   camera_data_t                                      *camera;
   atlas_value_t                                      value;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // If class pointer is not found, return NoRes
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact a camera; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &camera)) == 0) return NO_RESPONSE();

   // If this is a question
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the object is the selected camera, return true
      if (self->m_active_camera == camera) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }
    
   // Set camera intercept structure
   self->m_camera_intercept.camera = camera;
   self->m_camera_intercept.self = self;
   self->m_active_camera = camera;

   // Set haptic intercept - future input will be directed to the supplied intercept methods below
   if (self->m_GL->SetMouseIntercept(&MyMouseEvent, &self->m_camera_intercept) != TITAN_RESULT_OK) return NO_RESPONSE();
   if (self->m_GL->SetKeyboardIntercept(&MyKeyboardEvent, &self->m_camera_intercept) != TITAN_RESULT_OK) return NO_RESPONSE();
   if (self->m_GL->SetPostFrameCallback(0, &DisplayData, self) != TITAN_RESULT_OK) return NO_RESPONSE();

   // Get camera transform - position, orient, and return computed transform of camera 
   camera->video_transform = glm::lookAt(camera->position, camera->position + camera->forward, camera->up);

   // Remember this camera as the active camera
   self->m_active_camera = camera;

   // Tell the renderer to render from this camera's perspective
   self->m_GL->SetCameraTransform(glm::value_ptr(camera->video_transform));

   // If the console (user) is controlling this camera
   if (camera->console)
      {
      // Set console perspective to be this camera's
      camera->console->target = camera->reference;
      }
   else
      {
      // Get the directive subject and validate that it is in fact a camera; return failure if unsuccessful
      if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_SUBJECT, reference_info, NULL)) == 0) return NO_RESPONSE();
      // If the reference is valid, get the video interface from reference; otherwise, return a failure
      if (reference_info.reference == 0 || (self->Label->GetValue(reference_info.reference, "Instance Data", value) != TITAN_RESULT_OK)) return NO_RESPONSE();

      // If the console is available
      camera->console = _RP(console_data_t, value.amount.handle_value);
      
      // Set console perspective to be this camera's
      camera->console->target = camera->reference;
      }

   // Return true
   return TO_RESPONSE(100);
}

/*
   Do: Track
   Make subject camera track (constantly look at) object reference 
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyCamera::DO_Track(ATLAS_SIMULATE_PARAMS) 
{ 
   MyCamera                                           *self = _RP(MyCamera, user_data);
   camera_data_t                                      *camera;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive subject and validate that it is in fact a camera; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_SUBJECT, reference_info, &camera)) == 0) return NO_RESPONSE();
   // Get the directive first object (transitive); return failure if no object is found
   if ((component = self->Directive->Get(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, ATLAS_GET_REFERENCE)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Return true if the object is a tracked target
      if (camera->target_reference == reference_info.reference && camera->follow_target == false) return TO_RESPONSE(100);
      // Return false otherwise
      return TO_RESPONSE(-100);
      }

   // Set camera target 
   camera->target_reference = reference_info.reference;
   camera->follow_target = false;

   // Return true
   return TO_RESPONSE(100);
}

/*
   Do: Follow
   Make subject camera follow (constantly look at while staying nearby) object reference 
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyCamera::DO_Follow(ATLAS_SIMULATE_PARAMS) 
{ 
   MyCamera                                           *self = _RP(MyCamera, user_data);
   camera_data_t                                      *camera;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive subject and validate that it is in fact a camera; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_SUBJECT, reference_info, &camera)) == 0) return NO_RESPONSE();
   // Get the directive first object (transitive); return failure if no object is found
   if ((component = self->Directive->Get(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, ATLAS_GET_REFERENCE)) ==  0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Return true if the object is a followed target
      if (camera->target_reference == reference_info.reference && camera->follow_target == true) return TO_RESPONSE(100);
      // Return false otherwise
      return TO_RESPONSE(-100);
      }

   // Set camera target 
   camera->target_reference = reference_info.reference;
   camera->follow_target = true;

   // Return true
   return TO_RESPONSE(100);
}

/*
   End of file
*/
