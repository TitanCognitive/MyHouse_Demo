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
   Intercept an incoming mouse event from the hosting application, where:
      <sys_time> is the number of milliseconds that have passed since Atlas has started
      <user_data> is the object intercept structure (See my_object_intercept_t in MyObject.h)
      <haptics> is the haptics structure (See my_window_haptic_t in MyWindow.h)
*/
void MyObject::MyMouseEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics)
{
   my_object_intercept_t                              *object_intercept = _RP(my_object_intercept_t, user_data);
   object_data_t                                      *object = object_intercept->object;
   MyObject                                           *self = object_intercept->self;
   MyGLAPI                                            *Window = self->m_GL;
   TITAN_FLOAT                                        *transform;
   TITAN_DOUBLE                                       delta_time;
   TITAN_DOUBLE                                       *key_times;
   TITAN_BITMAP64                                     *key_states;
   TITAN_FLOAT                                        *object_angles;
   
   #define MOUSE_SPEED                                0.1

   // Get object transform
   transform = object->transform.f.value;

   // Get keyboard state
   Window->GetKeyboardState(&key_times, &key_states);

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
      // Set object animation to reach its destination in 100ms regardless of how far it moves
      object->coordinates.angle.xyzt_f.t = 0.001f;
      // Get object angles
      object_angles = object->coordinates.angle.f.value;
      // Compute new object angles based on mouse deviation from center of screen
      object_angles[TITAN_PITCH] -= TO_FLOAT(MOUSE_SPEED * delta_time * TO_DOUBLE(haptics->client_center_y - haptics->mouse_y));
      object_angles[TITAN_YAW] -= TO_FLOAT(MOUSE_SPEED * delta_time * TO_DOUBLE(haptics->client_center_x - haptics->mouse_x));
      // If the control key is pressed
      if (key_states[VK_CONTROL] & KEY_STATE_ON)
         {
         // Allow 10-degree angle quantization
         object_angles[TITAN_PITCH] = TO_32s(object_angles[TITAN_PITCH] * 10) * 0.1f;
         object_angles[TITAN_YAW] = TO_32s(object_angles[TITAN_YAW] * 10) * 0.1f;
         }
      // Update object coordinate / animation
      self->Location->UpdateCoordinate(object->sim_object, TITAN_COORDINATE_ROTATE, 0, true);
      // Get latest coordinates of this object from the simulation trail
      self->m_current_coordinate_index = 0;
      self->m_num_trail_coordinates = self->m_max_trail_coordinates;
      if (self->Location->GetCoordinates(object->sim_object, self->m_current_coordinate_index, self->m_trail_coordinates, self->m_num_trail_coordinates) == TITAN_RESULT_OK)
         {
         // Do nothing
         }
      // Reset mouse cursor position to middle of the screen
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
   Intercept an incoming window event
*/
void MyObject::MyWindowEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics)
{
   MyObject                                           *self = _RP(MyObject, user_data);
   
   // Do nothing
   return;
}

/*
   Intercept an incoming keyboard event from the hosting application, where:
      <sys_time> is the number of milliseconds that have passed since Atlas has started
      <user_data> is the object intercept structure (See my_object_intercept_t in MyObject.h)
      <haptics> is the haptics structure (See my_window_haptic_t in MyWindow.h)
*/
void MyObject::MyKeyboardEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics)
{
   my_object_intercept_t                              *object_intercept = _RP(my_object_intercept_t, user_data);
   object_data_t                                      *object = object_intercept->object;
   MyObject                                           *self = object_intercept->self;
   MyGLAPI                                            *Window = self->m_GL;
   TITAN_FLOAT                                        movement_speed = .005f;
   TITAN_ULONG                                        coordinate_index;
   TITAN_BITMAP64                                     *key_states = haptics->key_states;
   TITAN_DOUBLE                                       *key_times = haptics->key_times;
   TITAN_DOUBLE                                       frame_time;
   TITAN_DOUBLE                                       delta_time;
   TITAN_FLOAT                                        *object_matrix;
   TITAN_BOOL                                         update_position = false;
   glm::vec3                                          forward, up, right, position;

   // Validate input; return if unsuccessful
   if ((Window = self->m_GL) == NULL) return;

   // Get keyboard sampling time
   frame_time = system_time - haptics->last_key_sample_time;
   delta_time = TO_FLOAT(frame_time) * .001;
   if (delta_time > 1.0) delta_time = 1.0;

   // Get selected object matrix
   object_matrix = object->transform.f.value;

   // If a camera is selected
   if (self->m_camera[0])
      {
      // Use camera perspective
      right = glm::normalize(self->m_camera[0]->right);
      up = glm::normalize(self->m_camera[0]->up);
      forward = glm::normalize(self->m_camera[0]->forward);
      }
   // Otherwise, use selected object perspective
   else
      {
      // Get the local direction vectors
      right = glm::normalize(glm::vec3(object_matrix[0], object_matrix[4], object_matrix[8]));
      up = glm::normalize(glm::vec3(object_matrix[1], object_matrix[5], object_matrix[9]));
      forward = glm::normalize(glm::vec3(object_matrix[2], object_matrix[6], object_matrix[10]));
      }
   
   // Get object position vector
   position = glm::vec3(object->coordinates.position.xyzt_f.x, object->coordinates.position.xyzt_f.y, object->coordinates.position.xyzt_f.z);

   // If the SHIFT key is pressed, move the object 10 times faster
   if ((key_states[VK_LSHIFT] & KEY_STATE_ON) || (key_states[VK_RSHIFT] & KEY_STATE_ON)) movement_speed *= 10.0f;
   // If the CTRL key is pressed, move the object 10 times slower
   if ((key_states[VK_LCONTROL] & KEY_STATE_ON) || (key_states[VK_RCONTROL] & KEY_STATE_ON)) movement_speed *= 0.10f;

   // If the W or UP key is pressed, move selected object forward
   if (key_states['W'] & KEY_STATE_ON || key_states[VK_UP] & KEY_STATE_ON)
      {
      position += forward * TO_FLOAT(delta_time) * movement_speed;      
      update_position = true;
      }
   // If the S or DOWN key is pressed, move selected object back
   if (key_states['S'] & KEY_STATE_ON || key_states[VK_DOWN] & KEY_STATE_ON)
      {     
      position -= forward * TO_FLOAT(delta_time) * movement_speed;
      update_position = true;
      }
   // If the A or LEFT key is pressed, move selected object left
   if (key_states['A'] & KEY_STATE_ON || key_states[VK_LEFT] & KEY_STATE_ON)
      {
      position -= right * TO_FLOAT(delta_time) * movement_speed;
      update_position = true;
      }
   // If the D or RIGHT key is pressed, move selected object right
   if (key_states['D'] & KEY_STATE_ON || key_states[VK_RIGHT] & KEY_STATE_ON)
      {
      position += right * TO_FLOAT(delta_time) * movement_speed;
      update_position = true;
      }
   // If the SPACE key is pressed, move selected object up
   if (key_states[VK_SPACE] & KEY_STATE_ON)
      {
      position += up * TO_FLOAT(delta_time) * movement_speed;
      update_position = true;
      }
   // If the ALT key is pressed
   if (key_states[VK_MENU] & KEY_STATE_ON)
      {
      // If the Enter key is also pressed
      if (key_states[VK_RETURN] & KEY_STATE_TOGGLE_ON)
         {
         // Toggle full-screen rendering
         self->m_GL->ToggleFullScreen();
         }
      // Otherwise, move selected object up
      else
         {
         position -= up * TO_FLOAT(delta_time) * movement_speed;
         update_position = true;
         }
      }

   // If the CTRL key is pressed
   if (key_states[VK_CONTROL] & KEY_STATE_ON)
      {
      // If the Q key is pressed, quit
      if (key_states['Q'] & KEY_STATE_TOGGLE_ON)
         {
         // End the client (must be in the same domain as MyClient)
         // self->Agent->Tell(object->reference, 0, "stop client");
         exit(0);
         // Return
         return;
         }
      // If the Z key is pressed, move selected object to its previous trail crumb position
      if (key_states['Z'] & KEY_STATE_ON)
         {
         // Get current crumb index
         coordinate_index = self->m_current_coordinate_index;
         // Get previous crumb index based on current crumb index
         self->Location->GetPreviousCoordinate(object->sim_object, coordinate_index, &object->coordinates);
         // Keep track of current crumb index
         if (coordinate_index) self->m_current_coordinate_index = coordinate_index;
         // Update/animate selected object
         self->Location->UpdateCoordinate(object->sim_object);
         // Return
         return;
         }
      // If the Y key is pressed, move selected object to its next trail crumb position
      if (key_states['Y'] & KEY_STATE_ON)
         {
         // Get current crumb index
         coordinate_index = self->m_current_coordinate_index;
         // Get next crumb index based on current crumb index
         self->Location->GetNextCoordinate(object->sim_object, coordinate_index, &object->coordinates);
         // Keep track of current crumb index
         if (coordinate_index) self->m_current_coordinate_index = coordinate_index;
         // Update/animate selected object
         self->Location->UpdateCoordinate(object->sim_object);
         // Return
         return;
         }
      // If CTRL-F10 is pressed, the save the scene
      if (key_states[VK_F10] & KEY_STATE_TOGGLE_ON)
         {
         // Request the scene to be saved
         self->Agent->Tell(object->reference, 0, "remember the scene");
         // Return
         return;
         }
      }
   else
      {
      // If the Q key is pressed
      if (key_states['Q'] & KEY_STATE_TOGGLE_ON)
         {
         // Release window overlay and select current camera
         self->Agent->Tell(object->reference, 0, "select camera");
         // Return
         return;
         }
      // If the RETURN key is pressed
      if (key_states[VK_RETURN] & KEY_STATE_TOGGLE_ON)
         {
         // Activate the console
         self->Agent->Tell(object->reference, 0, "activate console");
         // Return
         return;
         }

      // If the F1 key is pressed
      if (key_states[VK_F1] & KEY_STATE_TOGGLE_ON)
         {
         // If the frame annotation is toggled
         if (self->m_show_help == false) self->m_show_help = true;
         else self->m_show_help = false;
         }
      // If the F2 key is pressed
      if (key_states[VK_F2] & KEY_STATE_TOGGLE_ON)
         {
         // Toggle world grid rendering
         self->m_GL->ShowGrid();
         }
      // If the F5 key is pressed
      if (key_states[VK_F5] & KEY_STATE_TOGGLE_ON)
         {
         // Snap pitch of selected object to nearest 45 degrees
         if (object)
            {
            const TITAN_INT                           angle_snap = 45;
            object->coordinates.angle.f.value[TITAN_PITCH] = TO_32s((object->coordinates.angle.f.value[TITAN_PITCH] * (180.0f / M_PI_F) / angle_snap) + 0.5f) * angle_snap * (M_PI_F / 180.0f);
            update_position = true;
            }
         }
      // If the frame annotation is toggled
      if (key_states[VK_F6] & KEY_STATE_TOGGLE_ON)
         {
         // Snap yaw of selected object to nearest 45 degrees
         if (object)
            {
            const TITAN_INT                           angle_snap = 45;
            object->coordinates.angle.f.value[TITAN_YAW] = TO_32s((object->coordinates.angle.f.value[TITAN_YAW] * (180.0f / M_PI_F) / angle_snap) + 0.5f) * angle_snap * (M_PI_F / 180.0f);
            update_position = true;
            }
         }
      // Quantize the roll angle
      if (key_states[VK_F7] & KEY_STATE_TOGGLE_ON)
         {
         // Snap roll of selected object to nearest 45 degrees
         if (object)
            {
            const TITAN_INT                           angle_snap = 45;
            object->coordinates.angle.f.value[TITAN_ROLL] = TO_32s((object->coordinates.angle.f.value[TITAN_ROLL] * (180.0f / M_PI_F) / angle_snap) + 0.5f) * angle_snap * (M_PI_F / 180.0f);
            update_position = true;
            }
         }
      }

   // If the position needs to be updated because of recent changes in this method
   if (update_position)
      {
      // Set destination position to requested position
      VectorCopy3(glm::value_ptr(position), object->coordinates.position.f.value);
      // Set time to get to destination to 1 millisecond
      object->coordinates.position.xyzt_f.t = 0.001f;
      // Set time to reach scale and angle to instantaneous
      object->coordinates.angle.xyzt_f.t = 0.001f;
      object->coordinates.scale.xyzt_f.t = 0.001f;
      // Update/animate the coordinate
      self->Location->UpdateCoordinate(object->sim_object, TITAN_COORDINATE_TRS, 0, true);
      // Get latest trail coordinates of this object
      self->m_current_coordinate_index = 0;
      self->m_num_trail_coordinates = self->m_max_trail_coordinates;
      if (self->Location->GetCoordinates(object->sim_object, self->m_current_coordinate_index, self->m_trail_coordinates, self->m_num_trail_coordinates) == TITAN_RESULT_OK)
         {

         }
      }
}

/*
   End of file
*/
