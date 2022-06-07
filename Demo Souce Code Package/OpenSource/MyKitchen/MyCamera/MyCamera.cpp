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
   Class constructor
*/
MyCamera::MyCamera(TITAN_HANDLE64 handshake) : TitanAxiom(handshake)
{
   // Initialize framework parameters
   Location = NULL;
   Size = NULL;
   Color = NULL;

   // Initialize local variables
   m_concept = 0;
   m_camera_interface_label = 0;

   // Initialize local pointers
   m_GL = NULL;
   m_scene = NULL;
   m_active_camera = NULL;

   // Clear default (pure) instance data
   memset(&m_default, 0, sizeof(camera_data_t));
}

/*
   Class destructor
*/
MyCamera::~MyCamera()
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
TITAN_HANDLE64 MyCamera::GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, 
                                    camera_data_t **instance_data, TITAN_BITMAP64 query_flags)
{
   TITAN_HANDLE64                                     component;

   // Get the directive first object (transitive); return failure if no object is found
   if ((component = Directive->Get(directive, component_id, reference_info, query_flags | ATLAS_GET_INSTANCE | ATLAS_GET_REFERENCE)) == 0) return 0;

   // If instance data is requested
   if (instance_data)
      {
      // Make sure the returned reference can be handled by this concept; return failure otherwise
      if (Cognitive->IsCompatible(reference_info.reference) != true) return 0;

      // Get reference instance and/or element
      if (reference_info.element) *instance_data = _RP(camera_data_t, reference_info.element);
      else if ((*instance_data = _RP(camera_data_t, reference_info.instance)) == NULL) return 0;
      }

   // Return component handle
   return component;
}

/*
   Align the input angle to the snap angle (degrees)
   Returns: The quantized angle in radians of the input angle <angle> based on the snap angle <snap_angle> in degrees           
*/
void MyCamera::AlignAngle(camera_data_t *camera, TITAN_UINT angle_index, TITAN_FLOAT snap_angle)
{
   TITAN_FLOAT                                        *camera_angles = camera->internal_angles.f.value;
   TITAN_FLOAT                                        angle = camera_angles[angle_index];
   TITAN_FLOAT                                        cos_pitch;

   // Ignore invalid angle indices
   if (angle_index > TITAN_ROLL) return;

   // Compute angle in degrees based on snap angle which was in radians
   if (angle < 0) angle = TO_32s((angle * (180.0f / M_PI_F) / snap_angle) - 0.5f) * snap_angle;
   else angle = TO_32s((angle * (180.0f / M_PI_F) / snap_angle) + 0.5f) * snap_angle;

   // Set angle limits for the pitch (disabled)
   if (0 && angle_index == 0)
      {
      if (angle < -90.0f / M_PI_F) angle = -90.f / M_PI_F;
      if (angle > 90.0f / M_PI_F) angle = 90.f / M_PI_F;
      }

   // Convert newly computed angle back from degrees to radians
   angle *= (M_PI_F / 180.0f);

   // Store new camera angle
   camera_angles[angle_index] = angle;

   // Compute camera directions:
   // Compute cosine of pitch temporarily since we'll used it several times in this method
   cos_pitch = cosf(camera_angles[TITAN_PITCH]);
   // Compute the forward, up, right vectors
   camera->forward = glm::vec3(cos_pitch * sinf(camera_angles[TITAN_YAW]), sinf(camera_angles[TITAN_PITCH]), cos_pitch * cosf(camera_angles[TITAN_YAW]));
   camera->right = glm::vec3(sinf(camera_angles[TITAN_YAW] - TO_FLOAT(M_PI_2)), 0.0f, cosf(camera_angles[TITAN_YAW] - TO_FLOAT(M_PI_2)));
   camera->up = glm::cross(camera->right, camera->forward);

   // Point the camera towards its newly computed forward vector (look forward)
   camera->video_transform = glm::lookAt(camera->position, camera->position + camera->forward, camera->up);
}

/*
   End of file
*/
