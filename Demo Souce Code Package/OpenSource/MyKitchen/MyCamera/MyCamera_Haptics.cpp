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
#include "MyScene/MyScene.h"
#include "../MyGLAPI/MyGLAPI_OS.h"

/*
   Intercept an incoming mouse event from the hosting application, where:
      <sys_time> is the number of milliseconds that have passed since Atlas has started
      <user_data> is the camera intercept structure (See my_camera_intercept_t in MyCamera.h)
      <haptics> is the haptics structure (See my_window_haptic_t in MyWindow.h)
*/
void MyCamera::MyMouseEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics)
{
   #define MOUSE_SPEED                                0.08f                                  // Mouse movement speed multiplier
   my_camera_intercept_t                              *camera_intercept = _RP(my_camera_intercept_t, user_data);
   camera_data_t                                      *camera = camera_intercept->camera;
   MyCamera                                           *self = camera_intercept->self;
   MyGLAPI                                            *Window = self->m_GL;
   TITAN_FLOAT                                        *camera_angles;
   TITAN_DOUBLE                                       delta_time;
   TITAN_DOUBLE                                       *key_times = haptics->key_times;
   TITAN_BITMAP64                                     *key_states = haptics->key_states;
   TITAN_FLOAT                                        cos_pitch;

   // If right mouse button is clicked (state transition)
   if (key_states[VK_RBUTTON] & KEY_STATE_TOGGLE_ON)
      {
      // Remember old mouse coordinates
      haptics->last_mouse_toggle_x = haptics->mouse_x;
      haptics->last_mouse_toggle_y = haptics->mouse_y;
      // Turn off mouse cursor and reset its position to the center of the screen
      self->m_GL->SetMousePosition(haptics->client_center_x, haptics->client_center_y, MY_MOUSE_CURSOR_OFF);
      }
   // If the right mouse button was being held (state sustained)
   else if (key_states[VK_RBUTTON] & KEY_STATE_ON)
      {
      // Get mouse movement delta time
      delta_time = TO_FLOAT(haptics->mouse_delta_time) * .001;
      // Get camera angles
      camera_angles = camera->internal_angles.f.value;
      // Compute new orientation based on deviation of mouse cursor from the center of the screen
      camera_angles[TITAN_PITCH] += TO_FLOAT(MOUSE_SPEED * delta_time * TO_DOUBLE(haptics->client_center_y - haptics->mouse_y));
      camera_angles[TITAN_YAW] += TO_FLOAT(MOUSE_SPEED * delta_time * TO_DOUBLE(haptics->client_center_x - haptics->mouse_x));
      // Force the camera pitch to be at most straight up, or straight down
      if (camera_angles[TITAN_PITCH] < -90.0f / M_PI_F) camera_angles[TITAN_PITCH] = -90.f / M_PI_F;
      if (camera_angles[TITAN_PITCH] > 90.0f / M_PI_F) camera_angles[TITAN_PITCH]  = 90.f / M_PI_F;
      // Set direction vectors
      cos_pitch = cosf(camera_angles[TITAN_PITCH]);
      camera->forward = glm::vec3(cos_pitch * sinf(camera_angles[TITAN_YAW]), sinf(camera_angles[TITAN_PITCH]), cos_pitch * cosf(camera_angles[TITAN_YAW]));
      camera->right = glm::vec3(sinf(camera_angles[TITAN_YAW] - TO_FLOAT(M_PI_2)), 0.0f, cosf(camera_angles[TITAN_YAW] - TO_FLOAT(M_PI_2)));
      camera->up = glm::cross(camera->right, camera->forward);
      // Force camera to look down its forward vector
      camera->video_transform = glm::lookAt(camera->position, camera->position + camera->forward, camera->up);     
      // Reset mouse cursor position back to middle of the screen
      self->m_GL->SetMousePosition(haptics->client_center_x, haptics->client_center_y);
      }
   // If the right-mouse button is depressed
   else if (key_states[VK_RBUTTON] & KEY_STATE_TOGGLE_OFF)
      {
      // Recall old mouse cursor position and turn it back on
      self->m_GL->SetMousePosition(haptics->last_mouse_toggle_x, haptics->last_mouse_toggle_y, MY_MOUSE_CURSOR_ON);
      }
}

/*
   Intercept an incoming keyboard event from the hosting application, where:
      <sys_time> is the number of milliseconds that have passed since Atlas has started
      <user_data> is the camera intercept structure (See my_camera_intercept_t in MyCamera.h)
      <haptics> is the haptics structure (See my_window_haptic_t in MyWindow.h)
*/
void MyCamera::MyKeyboardEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics)
{
   #define KEYBOARD_SPEED                             0.01f                                  // Keyboard movement speed multiplier
   my_camera_intercept_t                              *camera_intercept = _RP(my_camera_intercept_t, user_data);
   camera_data_t                                      *camera = camera_intercept->camera;
   MyCamera                                           *self = camera_intercept->self;
   MyGLAPI                                            *Window = self->m_GL;
   TITAN_BITMAP64                                     *key_states = haptics->key_states;
   TITAN_DOUBLE                                       *key_times = haptics->key_times;
   TITAN_DOUBLE                                       frame_time;
   TITAN_DOUBLE                                       delta_time;

   // Validate input; return if unsuccessful
   if ((Window = self->m_GL) == NULL) return;

   // Get keyboard sampling time
   frame_time = system_time - haptics->last_key_sample_time;
   delta_time = TO_FLOAT(frame_time);
   if (delta_time > 1.0) delta_time = 1.0;

   if ((key_states[VK_LSHIFT] & KEY_STATE_ON) || (key_states[VK_RSHIFT] & KEY_STATE_ON)) delta_time *= 10.0f;
   if ((key_states[VK_LCONTROL] & KEY_STATE_ON) || (key_states[VK_RCONTROL] & KEY_STATE_ON)) delta_time *= 0.10f;

   // If the Escape key is pressed; quit
   if (key_states['A'] & KEY_STATE_ON || key_states[VK_LEFT] & KEY_STATE_ON)
      {
      // Get system time
      camera->position -= camera->right * TO_FLOAT(delta_time) * KEYBOARD_SPEED;
      camera->video_transform = glm::lookAt(camera->position, camera->position + camera->forward, camera->up);
      }
   if (key_states['D'] & KEY_STATE_ON || key_states[VK_RIGHT] & KEY_STATE_ON)
      {
      camera->position += camera->right * TO_FLOAT(delta_time) * KEYBOARD_SPEED;
      camera->video_transform = glm::lookAt(camera->position, camera->position + camera->forward, camera->up);
      }
   if (key_states['W'] & KEY_STATE_ON || key_states[VK_UP] & KEY_STATE_ON)
      {
      camera->position += camera->forward * TO_FLOAT(delta_time) * KEYBOARD_SPEED;
      camera->video_transform = glm::lookAt(camera->position, camera->position + camera->forward, camera->up);
      }
   if (key_states['S'] & KEY_STATE_ON || key_states[VK_DOWN] & KEY_STATE_ON)
      {
      camera->position -= camera->forward * TO_FLOAT(delta_time) * KEYBOARD_SPEED;
      camera->video_transform = glm::lookAt(camera->position, camera->position + camera->forward, camera->up);
      }
   if (key_states[VK_SPACE] & KEY_STATE_ON)
      {
      camera->position += camera->up * TO_FLOAT(delta_time) * KEYBOARD_SPEED;
      camera->video_transform = glm::lookAt(camera->position, camera->position + camera->forward, camera->up);
      }
   // If the ALT key is pressed
   if (key_states[VK_MENU] & KEY_STATE_ON)
      {
      // If the Enter key is also press
      if (key_states[VK_RETURN] & KEY_STATE_TOGGLE_ON)
         {
         self->m_GL->ToggleFullScreen();
         }
      else
         {
         camera->position -= camera->up * TO_FLOAT(delta_time) * KEYBOARD_SPEED;
         camera->video_transform = glm::lookAt(camera->position, camera->position + camera->forward, camera->up);
         }
      }
   // If the ALT key is pressed
   if (key_states[VK_CONTROL] & KEY_STATE_ON)
      {
      if (key_states['Q'] & KEY_STATE_TOGGLE_ON)
         {
         // End the client
         exit(0);
         // Return
         return;
         }
      if (key_states[VK_F10] & KEY_STATE_TOGGLE_ON)
         {
         // End the client
         self->Agent->Tell(camera->reference, 0, "remember the scene");
         // Return
         return;
         }
      }
   else
      {
      // If the escape key is pressed
      if (key_states[VK_ESCAPE] & KEY_STATE_TOGGLE_ON)
         {
         // End the client
         // self->Agent->Tell(camera->reference, 0, "stop client");
         // Return
         return;
         }
      // If the Enter key is also press  
      if (key_states[VK_RETURN] & KEY_STATE_TOGGLE_ON)
         {
         // End the client
         self->Agent->Tell(camera->reference, 0, "activate console");
         }

      // If the frame annotation is toggled
      if (key_states[VK_F1] & KEY_STATE_TOGGLE_ON)
         {
         // Toggle information rendering
         if (camera->rendering_info)
            {
            // Do not show rendering information
            camera->rendering_info = false;
            // Turn off information rendering
            // self->m_GL->SetPostFrameCallback(0, NULL, NULL);
            }
         // Otherwise
         else
            {
            // Show rendering information
            camera->rendering_info = true;
            // Turn on information rendering
            // self->m_GL->SetPostFrameCallback(0, &DisplayData, self);
            }
         }
      // If the world grid is toggled
      if (key_states[VK_F2] & KEY_STATE_TOGGLE_ON)
         {
         // Toggle grid rendering
         self->m_GL->ShowGrid();
         }
      // If the scene rendering is toggled
      if (key_states[VK_F3] & KEY_STATE_TOGGLE_ON)
         {
         // Toggle scene rendering
         self->m_scene->Scene->ToggleRendering(self->m_scene);
         }
      // If the frame annotation is toggled
      if (key_states[VK_F5] & KEY_STATE_TOGGLE_ON)
         {
         // Align camera angle and compute new camera vectors
         self->AlignAngle(camera, 0, 45.0f);
         }
      // If the frame annotation is toggled
      if (key_states[VK_F6] & KEY_STATE_TOGGLE_ON)
         {
         // Align camera angle and compute new camera vectors
         self->AlignAngle(camera, 1, 45.0f);
         }
      // Quantize the roll angle
      if (key_states[VK_F7] & KEY_STATE_TOGGLE_ON)
         {
         // Align camera angle and compute new camera vectors
         self->AlignAngle(camera, 2, 45.0f);
         }
      }      
}

/*
   End of file
*/
