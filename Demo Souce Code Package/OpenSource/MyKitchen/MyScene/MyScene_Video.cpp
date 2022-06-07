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
#include "MyScene/MyScene.h"
#include "../MyGLAPI/MyGLAPI_OS.h"

/*
   Callback method for post-rendering scene data
*/
void MyScene::DisplayData(MyGLAPI *GL, TITAN_POINTER user_data)
{
   my_scene_intercept_t                               *scene_intercept = _RP(my_scene_intercept_t, user_data);
   scene_data_t                                       *scene = scene_intercept->scene;
   TITAN_FLOAT                                        *mat, *position;
   titan_vec3_t                                       origin;
   TITAN_UINT                                         reference_index;
   TITAN_HANDLE64                                     *references = scene->references;

   // Set the camera perspective
   GL->SetCameraPerspective();

   // Enable alpha-blended 2D rendering
   glEnable(GL_BLEND);
   glEnable(GL_TEXTURE_2D);
   
   // Set text color to white
   glColor3f(1.0f, 1.0f, 0.0f);

   // Go through all the references in the scene
   for (reference_index = 0; reference_index < scene->num_references; reference_index++)
      {
      // Skip invalid references
      if (references[reference_index] == 0) continue;
      if (scene->names[reference_index] == NULL) continue;
      if ((mat = scene->transforms[reference_index]) == NULL) continue;

      // Set font rendering raster position
      position = &scene->transforms[reference_index][0xC];
      VectorCopy3(position, origin.f.value);
      
      // Render reference name
      GL->Font->FontPrintv(&origin, 4, "%s", scene->names[reference_index]);
      }

   // Turn off texture rendering
   glDisable(GL_TEXTURE_2D);

   // Turn on line rendering
   glBegin(GL_LINES);
   
   // Go through all the references in the scene
   for (reference_index = 0; reference_index < scene->num_references; reference_index++)
      {
      // Skip invalid references
      if (references[reference_index] == 0) continue;
      if (scene->names[reference_index] == NULL) continue;
      if ((mat = scene->transforms[reference_index]) == NULL) continue;
      position = &scene->transforms[reference_index][0xC];

      // Render forward vector
      glColor3f(0.0f, 0.0f, 1.0f);
      glVertex3fv(position);
      glVertex3f(position[0] + mat[0] * 0.1f, position[1] + mat[1] * 0.1f, position[2] + mat[2] * 0.1f);

      // Render up vector
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3fv(position);
      glVertex3f(position[0] + mat[4] * 0.1f, position[1] + mat[5] * 0.1f, position[2] + mat[6] * 0.1f);

      // Render right vector
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex3fv(position);
      glVertex3f(position[0] + mat[8] * -0.1f, position[1] + mat[9] * -0.1f, position[2] + mat[10] * -0.1f);
      }
   // Stop rendering lines
   glEnd();
}

/*
   End of file
*/
