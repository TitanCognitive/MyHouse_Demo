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
#include "MyConsole/MyConsole.h"
#include "../MyGLAPI/MyGLAPI_OS.h"

/*
   Intercept an incoming mouse event from the hosting application, where:
      <sys_time> is the number of milliseconds that have passed since Atlas has started
      <user_data> is the console class (See MyConsole.h)
      <haptics> is the haptics structure (See my_window_haptic_t in MyWindow.h)
*/
void MyConsole::MyWindowEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics)
{
   MyConsole                                           *self = _RP(MyConsole, user_data);
 
   // Get mouse position and window parameters
   self->m_window_width = haptics->x_max - haptics->x_min;
   self->m_window_height = haptics->y_max - haptics->y_min;
   self->m_window_aspect_ratio = TO_FLOAT(self->m_window_width) / TO_FLOAT(self->m_window_height);  
   self->m_client_center_x = haptics->client_center_x;
   self->m_client_center_y = haptics->client_center_y;
   self->m_window_center_x = haptics->screen_center_x;
   self->m_window_center_y = haptics->screen_center_y;
}

/*
   Intercept an incoming keyboard event from the hosting application, where:
      <sys_time> is the number of milliseconds that have passed since Atlas has started
      <user_data> is the console intercept structure (See my_console_intercept_t in MyConsole.h)
      <haptics> is the haptics structure (See my_window_haptic_t in MyWindow.h)
*/
void MyConsole::MyKeyboardEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics)
{
   my_console_intercept_t                             *console_intercept = _RP(my_console_intercept_t, user_data);
   console_data_t                                     *console = console_intercept->console;
   MyConsole                                          *self = console_intercept->self;
   MyGLAPI                                            *Window;
   TITAN_BYTE                                         *key_values;
   TITAN_STRING                                       *key_macros;
   TITAN_INT                                          key_shift = 0x0;
   TITAN_BITMAP64                                     *key_states = haptics->key_states;
   TITAN_DOUBLE                                       *key_times = haptics->key_times;
   TITAN_BOOL                                         control_state, shift_state;

   // Validate input; return if unsuccessful
   if ((Window = self->m_GL) == NULL) return;
   if ((key_values = console->keyboard_translation_table) == NULL) return;
   if ((key_macros = console->keyboard_macros) == NULL) return;

   // Get control and shift key states
   (key_states[VK_CONTROL] & KEY_STATE_ON) ? control_state = true : control_state = false;
   (key_states[VK_SHIFT] & KEY_STATE_ON) ? shift_state = true : shift_state = false;

   // If the caps lock is on
   if (key_states[VK_CAPITAL] & KEY_STATE_ACTIVE) 
      {
      // If the shift key is pressed, cancel out the shift; otherwise, shift to capital
      (key_states[VK_SHIFT] & KEY_STATE_ON) ? key_shift = 0x0 : key_shift = 0x100;
      }
   else
      {
      // If the shift key is pressed, shift to capital; otherwise, cancel out the shift
      (key_states[VK_SHIFT] & KEY_STATE_ON) ? key_shift = 0x100 : key_shift = 0x0;
      }

   // Process text input keys 
   if (control_state == false) self->m_Editor.Key_Text(system_time, console, haptics, key_shift);

   // Handle arrow keys   
   if (key_states[VK_LEFT] & KEY_STATE_ON) self->m_Editor.Key_Left(system_time, console, key_states[VK_LEFT], key_times[VK_LEFT], control_state, shift_state);
   if (key_states[VK_RIGHT] & KEY_STATE_ON) self->m_Editor.Key_Right(system_time, console, key_states[VK_RIGHT], key_times[VK_RIGHT], control_state, shift_state);
   if (key_states[VK_UP] & KEY_STATE_TOGGLE_ON) self->m_Editor.Key_Up(system_time, console, key_states[VK_UP], key_times[VK_UP], control_state, shift_state);
   if (key_states[VK_DOWN] & KEY_STATE_TOGGLE_ON) self->m_Editor.Key_Down(system_time, console, key_states[VK_DOWN], key_times[VK_DOWN], control_state, shift_state);
   if (key_states[VK_HOME] & KEY_STATE_TOGGLE_ON) self->m_Editor.Key_Home(system_time, console, key_states[VK_HOME], key_times[VK_HOME], control_state, shift_state);
   if (key_states[VK_END] & KEY_STATE_TOGGLE_ON) self->m_Editor.Key_End(system_time, console, key_states[VK_END], key_times[VK_END], control_state, shift_state);
   // Handle backspace key; delete character before cursor
   if (key_states[VK_BACK] & KEY_STATE_ON) self->m_Editor.Key_Backspace(system_time, console, key_states[VK_BACK], key_times[VK_BACK], control_state, shift_state);
   // Handle delete key; delete character under cursor
   if (key_states[VK_DELETE] & KEY_STATE_ON) self->m_Editor.Key_Delete(system_time, console, key_states[VK_DELETE], key_times[VK_DELETE], control_state, shift_state);
   // Handle insert key; toggle text insert/overwrite mode
   if (key_states[VK_INSERT] & KEY_STATE_TOGGLE_ON) console->overwrite ^= true;
   // Handle function keys
   if (key_states[VK_F1] & KEY_STATE_TOGGLE_ON) self->m_Editor.Key_F1(system_time, console, key_states[VK_F1], key_times[VK_F1], control_state, shift_state);
   // Handle special keys
   if (key_states[VK_ESCAPE] & KEY_STATE_TOGGLE_ON) self->m_Editor.Key_ESCAPE(system_time, console, key_states[VK_ESCAPE], key_times[VK_ESCAPE], control_state, shift_state);

   // If the clipboard paste command is requested (Ctrl-V) 
   if (((key_states['V'] & KEY_STATE_TOGGLE_ON) && control_state) || ((key_states[VK_INSERT] & KEY_STATE_TOGGLE_ON) && shift_state))
      {
      self->m_Editor.Key_Paste(system_time, console, key_states['V'], key_times[VK_F1], control_state, shift_state);
      }

   // If the return key is pressed
   if (key_states[VK_RETURN] & KEY_STATE_TOGGLE_ON)
      {
      // If there is no message; deactivate the console
      if (console->cursor_index == 0)
         {
         // Release window overlay
         self->m_GL->SetPostFrameCallback(0, NULL, NULL);
         // Activate current camera
         self->Tell(console, console->console_sim.reference, 0, "select current camera");
         }
      // Otherwise, process the line
      else
         {
         titan_vec4_t                                 color = { 0.8f, 0.8f, 0.8f, 1.0f };
         
         // Scroll down to the latest message on console
         console->message.handle = 0;
         console->selected_line_handle = 0;
         
         // Display message on console
         self->Print(console, NULL, 2.0f, &color, console->message.text);
         // If this is a keyword message
         if (console->message.text[0] == '!')
            {
            // Process keyword string
            self->Ledger->ProcessKeyword(console->ledger, self->Reference->GetPublicHandle(console->console_sim.reference), &console->message.text[1]);
            }
         // Otherwise
         else
            {
            // Tell the expression to the target of the console (or the hosting Atlas clone, if the target reference is 0)
            self->Tell(console, console->target, 0, console->message.text);
            // Reset cursor position
            console->cursor_index = 0;
            // Clear the text message
            memset(console->message.text, 0, 128);
            }
         }
      }
}


/*
   End of file
*/
