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

/*
   BE: Active
   Set subject camera of directive to be active
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyCamera::BE_Active(ATLAS_SIMULATE_PARAMS)
{
   MyCamera                                           *self = _RP(MyCamera, user_data);
   camera_data_t                                      *camera;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();  

   // Get the directive subject and validate that it is in fact a camera; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_SUBJECT, reference_info, &camera)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Check camera speed and return true if it's slow
      if (camera == self->m_active_camera) return TO_RESPONSE(100);
      // Return false otherwise
      return TO_RESPONSE(-100);
      }

   // Set camera intercept structure
   self->m_camera_intercept.camera = NULL;
   self->m_camera_intercept.self = self;

   // Set haptic intercept
   if (self->m_GL->SetMouseIntercept(&MyMouseEvent, &self->m_camera_intercept) != TITAN_RESULT_OK) return NO_RESPONSE();
   if (self->m_GL->SetKeyboardIntercept(&MyKeyboardEvent, &self->m_camera_intercept) != TITAN_RESULT_OK) return NO_RESPONSE();

   // Select current camera as the main perspective for the renderer
   self->m_GL->SetCameraTransform(glm::value_ptr(camera->video_transform));

   // Return true
   return TO_RESPONSE(100);
}

/*
   BE: Slow
   Slow down directive subject camera speed by 20%
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyCamera::BE_Slow(ATLAS_SIMULATE_PARAMS)
{
   MyCamera                                           *self = _RP(MyCamera, user_data);
   camera_data_t                                      *camera;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();  

   // Get the directive subject and validate that it is in fact a camera; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_SUBJECT, reference_info, &camera)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Check camera speed and return true if it's slow
      if (camera->speed < 1.0f) return TO_RESPONSE(100);
      // Return 'no answer'
      return NO_RESPONSE();
      }

   // Lower camera speed if it's above threshold
   if (camera->speed > 0.2f) camera->speed *= 0.8f;

   // Return true
   return TO_RESPONSE(100);
}

/*
   BE: Fast
   Increase directive subject camera speed by 20%
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyCamera::BE_Fast(ATLAS_SIMULATE_PARAMS)
{
   MyCamera                                           *self = _RP(MyCamera, user_data);
   camera_data_t                                      *camera;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive subject and validate that it is in fact a camera; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_SUBJECT, reference_info, &camera)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Check camera speed and return true if it's slow
      if (camera->speed > 1.0f) return TO_RESPONSE(100);
      // Return 'no answer'
      return NO_RESPONSE();
      }

   // Increase camera speed if it's below threshold
   if (camera->speed < 10.0f) camera->speed *= 1.2f;

   // Return true
   return TO_RESPONSE(100);
}

/*
   BE: Fast
   Set directive subject camera to be the current rendering perspective
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyCamera::BE_Current(ATLAS_SIMULATE_PARAMS)
{
   MyCamera                                           *self = _RP(MyCamera, user_data);
   camera_data_t                                      *camera;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive subject and validate that it is in fact a camera; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_SUBJECT, reference_info, &camera)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Check if the subject is selected camera
      if (self->m_active_camera == camera) return TO_RESPONSE(100);
      // Return 'no'
      return TO_RESPONSE(-100);
      }

   // Set selected camera to subject
   self->m_active_camera = camera;

   // Return true
   return TO_RESPONSE(100);
}

/*
   End of file
*/
