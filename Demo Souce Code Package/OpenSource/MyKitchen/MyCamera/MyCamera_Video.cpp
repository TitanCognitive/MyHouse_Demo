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
#include "../MyGLAPI/MyGLAPI_OS.h"

/*
   Load a model specified by model index
   Add model name as a label for supplied camera instance
*/
void MyCamera::LoadModel(TITAN_INT model_index, MyCamera *self, camera_data_t *camera)
{
   TITAN_STRING                                       model_name;
   TITAN_UINT                                         model_name_index;
   TITAN_UINT                                         num_model_names;

   // Make sure the class data is available as a payload
   if (self == NULL) { return; }

   // Set camera model handle
   camera->model_handle = model_index;
   camera->model_instance = -1;

   // If the model was successfully allocated
   if (model_index != -1)
      {
      // Get the number of model names
      num_model_names = self->m_GL->Model->GetNumModelNames(model_index);
      // Add all model names as labels for this instance
      for (model_name_index = 0; model_name_index < num_model_names; model_name_index++)
         {
         // If the model name is valid
         if ((model_name = self->m_GL->Model->GetModelName(model_index, model_name_index)) != NULL)
            {
            // Add model name as a label for this instance 
            self->Label->AddLabel(camera->reference, model_name, camera->reference);
            }
         }
      }
}

/*
   Update transform of the active camera in the MyCamera class
*/
void MyCamera::UpdateCamera(MyGLAPI *GL, TITAN_POINTER user_data)
{
   MyCamera                                           *self = _SP(MyCamera, user_data);
   camera_data_t                                      *camera = self->m_active_camera;
   titan_mat4_t                                       target_transform;  

   // Return if no active camera is found
   if (camera == NULL) return;

   // If the active camera has no target
   if (camera->target_reference == 0)
      {
      // Disable camera updates
      self->m_GL->SetPreFrameCallback(0, NULL, NULL);
      // Return
      return;
      }

   // Get the transform of the active camera target; return if unsuccessful
   if (self->Reference->GetTransform(camera->target_reference, target_transform) !=  TITAN_RESULT_OK) return;
   
   // Point camera towards the target transform
   camera->video_transform = glm::lookAt(camera->position, 
                                        glm::vec3(target_transform.f.value[12], target_transform.f.value[13], target_transform.f.value[14]), 
                                        glm::vec3(0.0f, 1.0f, 0.0f));
}

/*
   Display camera data at the end of a rendering frame (post-render)
*/
void MyCamera::DisplayData(MyGLAPI *GL, TITAN_POINTER user_data)
{
   MyCamera                                           *self = _SP(MyCamera, user_data);
   camera_data_t                                      *camera = self->m_active_camera;
   TITAN_FLOAT                                        *vector_3 = NULL;
   TITAN_FLOAT                                        *vector_4 = NULL;
   titan_vec3_t                                       origin;

   // Reset OpenGL transformation matrix
   glLoadIdentity();

   // Enable 2D texture rendering
   glEnable(GL_TEXTURE_2D);

   // Set text color to transparent yellow
   glColor4f(1.0f, 1.0f, 0.0f, 0.2f);

   // Display current frame number
   if (camera->rendering_info == true)
      {
      // Set font rendering coordinates
      VectorSet3(origin.f.value, -44.0f, -22.0f, -30.0f);
      GL->Font->FontPrintv(&origin, 8, "Frame: %llu", GL->GetFrame());
      }
      
   // Set font rendering coordinates
   VectorSet3(origin.f.value, -44.0f, -24.0f, -30.0f);
   GL->Font->FontPrintv(&origin, 15, "Press 'F1' for help; 'Enter' to activate console.");

   // Do not render info if info rendering is not enabled
   if (camera->rendering_info == false) return;
      
   // Display help text:
   // Set text raster position
   VectorSet3(origin.f.value, -20.0f, 10.0f, -20.0f);
   // Set text color to white
   glColor3f(1.0f, 1.0f, 1.0f);
   // Display help text header
   GL->Font->FontPrintv(&origin, 8, "F1            Display help menu"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "F2            Display world grid"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "F3            Show instance names"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "CTRL-Q        Terminate Atlas"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "CTRL-F10      Remember the scene (overwrite)"); origin.xyz_f.y -= 1.6f; origin.xyz_f.x = -20.0f;

   // Set text color to gray
   glColor3f(0.8f, 0.8f, 0.8f);
   // Display help text body
   GL->Font->FontPrintv(&origin, 8, "ENTER         Enter/Leave console"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "W / UP        Move camera forward"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "S / DOWN      Move camera backward"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "A / LEFT      Move camera left"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "D / RIGHT     Move camera right"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "ALT           Move camera down"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "Space         Move camera up"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "F5            Round camera pitch to nearest 45 degrees"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "F6            Round camera yaw to nearest 45 degrees"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
   GL->Font->FontPrintv(&origin, 8, "F7            Round camera roll to nearest 45 degrees"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;

   // Set text raster position
   VectorSet3(origin.f.value, -44.0f, -18.0f, -30.0f);
   
   // If an active camera exists
   if (camera)
      {
      glColor3f(0.0f, 0.8f, 0.0f);
      // Display camera angles in degrees
      GL->Font->FontPrintv(&origin, 4, "Angles: % 6.2f (F5) % 6.2f (F6) % 6.2f (F7)",
                     camera->internal_angles.rot.pitch * 180.0f / M_PI_F,
                     camera->internal_angles.rot.yaw * 180.0f / M_PI_F,
                     camera->internal_angles.rot.roll * 180.0f / M_PI_F);
      // Display camera angles in radians
      origin.xyz_f.y -= 0.8f;
      GL->Font->FontPrintv(&origin, 4, "Angles (rad): % 6.2f % 6.2f % 6.2f",
                     camera->internal_angles.rot.pitch,
                     camera->internal_angles.rot.yaw,
                     camera->internal_angles.rot.roll);
      }
}

/*
   End of file
*/
