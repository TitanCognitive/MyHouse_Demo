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
#include "MyScene/MyScene.h"

/*
   Create a camera instance
   Returns: Handle to the camera instance created upon successful operation
            0 upon failure to create camera instance
*/
TITAN_HANDLE64 MyCamera::AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake)
{
   camera_data_t                                      *camera;
   TITAN_HANDLE64                                     sim_object;
   TITAN_FLOAT                                        cos_pitch;
   TITAN_FLOAT                                        *camera_angles;

   // Create a simulation object; return failure if unsuccessful
   if ((sim_object = Simulation->CreateSimulationObject(caller_id, m_concept)) == NULL) return 0;
   
   // Allocate concept data structure
   if (TITAN_ALLOCATE_FLUSH_ARRAY(camera, camera_data_t, num_elements) == NULL) return 0;

   // Set camera to happy initially
   camera->reference = caller_id;
   camera->sim_object = sim_object;

   // Set direction vector
   camera_angles = camera->internal_angles.f.value;
   cos_pitch = cosf(camera_angles[TITAN_PITCH]);
   camera->forward = glm::vec3(cos_pitch * sinf(camera_angles[TITAN_YAW]), sinf(camera_angles[TITAN_PITCH]), cos_pitch * cosf(camera_angles[TITAN_YAW]));
   camera->right = glm::vec3(sinf(camera_angles[TITAN_YAW] - TO_FLOAT(M_PI_2)), 0.0f, cosf(camera_angles[TITAN_YAW] - TO_FLOAT(M_PI_2)));
   camera->up = glm::cross(camera->right, camera->forward);

   // If the parameters field has been set
   if (params_handle && m_GL)
      {
      // Precache model; return failure if unsuccessful
      if (m_GL->PrecacheResource(MY_GL_PRECACHE_FLAG_MODEL, TO_STRING(params_handle), &LoadModel, this, camera) != TITAN_RESULT_OK)
         {
         // FIXME: Should we fail?
         }
      }

   // Set camera simulation transform only (no coordinate)
   Location->SetCoordinate(camera->sim_object, NULL, _RP(titan_mat4_t, glm::value_ptr(camera->video_transform)), true);

   // Add object to the scene
   if (m_scene) m_scene->Scene->Register(m_scene, caller_id, glm::value_ptr(camera->video_transform));

   // Return camera data structure
   return TO_HANDLE64(camera);
}

/*
   Destroy camera instance concept data
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyCamera::AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake)
{
   camera_data_t                                      *camera;

   // Validate input; return failure if unscucessful
   if (instance == 0) return TITAN_RESULT_INVALID_INPUT;

   // Get camera instance data
   camera = _RP(camera_data_t, instance);

   // Free camera data
   TITAN_FREE(camera);

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Get camera simulation object based on supplied <instance> and <element> pair, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle to the simulation object of the supplied camera instance/element upon successful operation
            0 upon failure to retrieved the requested simulation object
*/
TITAN_HANDLE64 MyCamera::AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   camera_data_t                                      *camera;

   // Get instance data from element if it is available; otherwise, instance handle has instance data 
   if (element) camera = _RP(camera_data_t, element);
   else if (instance) camera = _RP(camera_data_t, instance);
   else return 0;

   // If the camera instance is valid, return its corresponding simulated object
   if (camera) return camera->sim_object;

   // Return failure otherwise
   return 0;
}

/*
   Load camera instance data from supplied file <file> into output handles <instance> and <element>, where:
      <caller_id> is the private reference ID of the reference loaded
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle of the instance loaded upon successful operation
            0 upon failure load the instance
*/
titan_result_t MyCamera::AtlasSim_Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 &instance, TITAN_HANDLE64 &element, TITAN_HANDLE64 handshake)
{
   titan_result_t                                     result;
   camera_data_t                                      *camera;
   TITAN_HANDLE64                                     sim_object;
   TITAN_UINT                                         element_index, num_elements;
   TITAN_FLOAT                                        cos_pitch;
   TITAN_FLOAT                                        *camera_angles;
   TITAN_SIZE64                                       num_bytes_read;

   // If the file was not specified, return failure
   if (file == 0) return TITAN_RESULT_INVALID_INPUT;

   // Read camera instance simulation object; return failure if unsuccessful
   if ((result = Simulation->LoadSimulationObject(caller_id, file, m_concept, sim_object)) != TITAN_RESULT_OK) return result;

   // Read number of elements in the camera instance; return failure if unsuccessful
   if ((result = File->Read(file, &num_elements, sizeof(TITAN_UINT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;
   // Allocate camera; return failure if unsuccessful
   if (TITAN_ALLOCATE_FLUSH_ARRAY(camera, camera_data_t, num_elements) == NULL) return TITAN_RESULT_NO_MEMORY;
   camera->reference = caller_id;  
   // Set camera simulation object
   camera->sim_object = sim_object;

   // Read element data for all elements
   for (element_index = 0; element_index < num_elements; element_index++)
      {
      // Read the camera coordinate; return failure if unsuccessful
      if ((result = File->Read(file, glm::value_ptr(camera->position), sizeof(TITAN_FLOAT), 3, num_bytes_read)) != TITAN_RESULT_OK) return result;
      if ((result = File->Read(file, camera->internal_angles.f.value, sizeof(TITAN_FLOAT), 3, num_bytes_read)) != TITAN_RESULT_OK) return result;
      // Set direction vector
      camera_angles = camera->internal_angles.f.value;
      cos_pitch = cosf(camera_angles[TITAN_PITCH]);
      camera->forward = glm::vec3(cos_pitch * sinf(camera_angles[TITAN_YAW]), sinf(camera_angles[TITAN_PITCH]), cos_pitch * cosf(camera_angles[TITAN_YAW]));
      camera->right = glm::vec3(sinf(camera_angles[TITAN_YAW] - TO_FLOAT(M_PI_2)), 0.0f, cosf(camera_angles[TITAN_YAW] - TO_FLOAT(M_PI_2)));
      camera->up = glm::cross(camera->right, camera->forward);
      camera->video_transform = glm::lookAt(camera->position, camera->position + camera->forward, camera->up);
      }

   // Set output handles
   instance = TO_HANDLE64(camera);
   element= 0;

   // Set camera simulation transform only (no coordinate)
   Location->SetCoordinate(camera->sim_object, NULL, _RP(titan_mat4_t, glm::value_ptr(camera->video_transform)), true);

   // Add object to the scene
   if (m_scene) m_scene->Scene->Register(m_scene, caller_id, glm::value_ptr(camera->video_transform));

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Save camera instance data to supplied file <file> from supplied <instance> and <element>, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyCamera::AtlasSim_Save(TITAN_HANDLE64 file, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   titan_result_t                                     result;
   camera_data_t                                      *camera;
   TITAN_UINT                                         element_index, num_elements;
   TITAN_SIZE64                                       num_bytes_written;

   // If the file was not specified, return failure
   if (file == 0) return TITAN_RESULT_INVALID_INPUT;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if (element) camera = _RP(camera_data_t, element);
   else if (instance) camera = _RP(camera_data_t, instance);
   else return TITAN_RESULT_INVALID_INPUT;

   // Save camera simulation object; return failure if unsuccessful
   if ((result = Simulation->SaveSimulationObject(file, camera->sim_object)) != TITAN_RESULT_OK) return result;

   // Set number of elements
   num_elements = 1;
   // Save number of elements in the camera instance; return failure if unsuccessful
   if ((result = File->Write(file, &num_elements, sizeof(TITAN_UINT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;

   // Save element data for all elements
   for (element_index = 0; element_index < num_elements; element_index++)
      {
      // Save the camera coordinate; return failure if unsuccessful
      if ((result = File->Write(file, glm::value_ptr(camera->position), sizeof(TITAN_FLOAT), 3, num_bytes_written)) != TITAN_RESULT_OK) return result;
      if ((result = File->Write(file, camera->internal_angles.f.value, sizeof(TITAN_FLOAT), 3, num_bytes_written)) != TITAN_RESULT_OK) return result;
      }

   // Return success
   return TITAN_RESULT_OK;
}

/*
   End of file
*/
