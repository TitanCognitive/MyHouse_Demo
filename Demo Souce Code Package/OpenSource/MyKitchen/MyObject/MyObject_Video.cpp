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
#include "MyCamera/MyCamera.h"
#include "../MyGLAPI/MyGLAPI_OS.h"

/*
   Load a model specified by model index
   Add model name as a label for supplied camera instance
*/
void MyObject::LoadModel(TITAN_INT model_index, MyObject *self, object_data_t *object)
{
   TITAN_STRING                                       model_name;
   TITAN_UINT                                         model_name_index;
   TITAN_UINT                                         num_model_names;

   // Make sure the class data is available as a payload
   if (self == NULL) { return; }

   // Set object model handle
   object->model_handle = model_index;

   // If the model was successfully allocated
   if (model_index >= 0)
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
            self->Label->AddLabel(object->reference, model_name, object->reference);
            }
         }
      // Create object model instance
      object->model_instance = self->m_GL->Model->CreateModelInstance(model_index, object->transform.f.value, &object->color, true);
      }
   // The model doesn't exist, mark it with ERROR index 
   else
      {
      // Set to -2 (ERROR)
      model_index = -2;
      }
}

/*
   Callback method for post-rendering object data
*/
void MyObject::DisplayData(MyGLAPI *GL, TITAN_POINTER user_data)
{
   my_object_intercept_t                              *object_intercept = _RP(my_object_intercept_t, user_data);
   object_data_t                                      *object = object_intercept->object;
   TITAN_FLOAT                                        *mat = object->transform.f.value;
   MyObject                                           *self = object_intercept->self;
   titan_vec3_t                                       origin;
   TITAN_UINT                                         coordinate_index;

   // Reset OpenGL transformation matrix
   glLoadIdentity();

   // Enable 2D texture rendering
   glEnable(GL_TEXTURE_2D);

   // Set text color to transparent yellow
   glColor4f(1.0f, 1.0f, 0.0f, 0.2f);
   // Set font rendering coordinates
   VectorSet3(origin.f.value, -44.0f, -24.0f, -30.0f);
   GL->Font->FontPrintv(&origin, 15, "Press 'F1' for help; 'Enter' to activate console.");

   // Turn off texture rendering
   glDisable(GL_TEXTURE_2D);

   // Set the camera perspective
   GL->SetCameraPerspective();

   // Turn on thick line rendering
   glLineWidth(3.0f);
   glBegin(GL_LINE_STRIP);

   // Go through all the coordinates in the simulation trail
   for (coordinate_index = 0; coordinate_index < self->m_num_trail_coordinates; coordinate_index++)
      {
      // Color trail in green
      glColor3f(0.0f, 1.0f, 0.0f);
      // Render trail crumb as a segment from previous trail crumb
      glVertex3fv(self->m_trail_coordinates[coordinate_index].position.f.value);
      }
   // Stop rendering lines
   glEnd();
   glLineWidth(1.0f);

   // Reset camera position
   glLoadIdentity();

   // Turn on 2D texture rendering
   glEnable(GL_TEXTURE_2D);

   // If the help menu is shown
   if (self->m_show_help)
      {
      // Set font raster position
      VectorSet3(origin.f.value, -20.0f, 10.0f, -20.0f);
      // Set font color to white
      glColor3f(1.0f, 1.0f, 1.0f);
      // Display help text header
      GL->Font->FontPrintv(&origin, 8, "F1            Display Help menu"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "F2            Display world grid"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "CTRL-Q        Terminate Atlas"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "CTRL-F10      Remember the scene (overwrite)"); origin.xyz_f.y -= 1.6f; origin.xyz_f.x = -20.0f;
      // Set text color to gray
      glColor3f(0.8f, 0.8f, 0.8f);
      // Display help text body
      GL->Font->FontPrintv(&origin, 8, "ENTER         Enter/Leave console"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "Q             Leave object editing mode"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "W / UP        Move object forward"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "S / DOWN      Move object backward"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "A / LEFT      Move object Left"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "D / RIGHT     Move object right"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "ALT           Move object down"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "Space         Move object up"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "SHIFT         Increase movement speed"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "CONTROL       Decrease movement speed"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      }

   // Set font color to white
   glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
   // If camera is available
   if (self->m_camera[0])
      {
      TITAN_FLOAT                                     *cam_vector;
      // Set font raster position
      VectorSet3(origin.f.value, -44.0f, -10.0f, -30.0f);
      cam_vector = glm::value_ptr(self->m_camera[0]->forward);
      // Display object header
      GL->Font->FontPrintv(&origin, 4, "Camera vectors:"); origin.xyz_f.y -= 0.8f;
      // Print camera forward vector coordinates
      GL->Font->FontPrintv(&origin, 4, "   Forward: % 6.2f % 6.2f % 6.2f", cam_vector[0], cam_vector[1], cam_vector[2]); origin.xyz_f.y -= 0.8f;
      // Print camera up vector coordinates
      cam_vector = glm::value_ptr(self->m_camera[0]->up);
      GL->Font->FontPrintv(&origin, 4, "   Up: % 6.2f % 6.2f % 6.2f", cam_vector[0], cam_vector[1], cam_vector[2]); origin.xyz_f.y -= 0.8f;
      // Print camera right vector coordinates
      cam_vector = glm::value_ptr(self->m_camera[0]->right);
      GL->Font->FontPrintv(&origin, 4, "   Right: % 6.2f % 6.2f % 6.2f", cam_vector[0], cam_vector[1], cam_vector[2]); origin.xyz_f.y -= 0.8f;
      }

   // Set font color to yellow
   glColor4f(1.0f, 1.0f, 0.0f, 0.8f);
   // Set text raster position
   VectorSet3(origin.f.value, -44.0f, -14.0f, -30.0f);
   // Display object header
   GL->Font->FontPrintv(&origin, 4, "Object Angles:"); origin.xyz_f.y -= 0.8f;
   // Display camera angles in degrees
   GL->Font->FontPrintv(&origin, 4, "   Degrees: % 6.2f (F5) % 6.2f (F6) % 6.2f (F7)", 
                        object->coordinates.angle.rot.pitch * 180.0f / M_PI_F,
                        object->coordinates.angle.rot.yaw * 180.0f / M_PI_F,                  
                        object->coordinates.angle.rot.roll * 180.0f / M_PI_F);
   // Display camera angles in radians
   origin.xyz_f.y -= 0.8f;
   GL->Font->FontPrintv(&origin, 4, "   Radians: % 6.2f % 6.2f % 6.2f",
                        object->coordinates.angle.rot.pitch,
                        object->coordinates.angle.rot.yaw,                  
                        object->coordinates.angle.rot.roll); origin.xyz_f.y -= 0.8f;

   // If the object transform is to be rendered
   if (mat)
      {
      // Set matrix color
      glColor4f(0.4f, 1.0f, 0.2f, .4f);
      // Set text raster position
      VectorSet3(origin.f.value, -44.0f, -18.0f, -30.0f);
      // Render transform matrix [4x4] (right-handed)
      GL->Font->FontPrintv(&origin, 10, "Matrix: % 6.2f % 6.2f % 6.2f % 6.2f", mat[0x0], mat[0x1], mat[0x2], mat[0x3]); origin.xyz_f.y -= 1.2f;
      GL->Font->FontPrintv(&origin, 10, "        % 6.2f % 6.2f % 6.2f % 6.2f", mat[0x4], mat[0x5], mat[0x6], mat[0x7]); origin.xyz_f.y -= 1.2f;
      GL->Font->FontPrintv(&origin, 10, "        % 6.2f % 6.2f % 6.2f % 6.2f", mat[0x8], mat[0x9], mat[0xA], mat[0xB]); origin.xyz_f.y -= 1.2f;

      // Set font color to blue
      glColor4f(0.0f, 0.0f, 1.0f, .8f);
      // Set text raster position
      VectorSet3(origin.f.value, 15.0f, -22.0f, -30.0f);
      // Render X coordinate
      GL->Font->FontPrintv(&origin, 12, "% 6.2f", mat[0xC]); origin.xyz_f.x += 7.0f;

      // Set font color to green
      glColor4f(0.0f, 1.0f, 0.0f, .4f);
      // Render Y coordinate
      GL->Font->FontPrintv(&origin, 12, " % 6.2f", mat[0xD]); origin.xyz_f.x += 7.0f;

      // Set font color to red
      glColor4f(1.0f, 0.0f, 0.0f, .4f);
      // Render Z coordinate
      GL->Font->FontPrintv(&origin, 12, " % 6.2f", mat[0xE]); origin.xyz_f.x += 7.0f;

      // Set font color to blue
      glColor4f(1.0f, 1.0f, 1.0f, .2f);
      // Render W coordinate
      GL->Font->FontPrintv(&origin, 12, " % 6.2f", mat[0xF]); origin.xyz_f.y -= 0.8f;
      }
  
}

/*
   End of file
*/
