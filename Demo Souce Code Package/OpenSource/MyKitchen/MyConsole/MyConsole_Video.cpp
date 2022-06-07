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
   Load a model specified by model index
   Add model name as a label for supplied camera instance
*/
void MyConsole::MyRender(MyGLAPI *GL, TITAN_POINTER user_data)
{
   my_console_intercept_t                             *console_intercept = _RP(my_console_intercept_t, user_data);
   console_data_t                                     *console = console_intercept->console;
   MyConsole                                          *self = console_intercept->self;
   console_message_data_t                             *message_data;
   titan_vec3_t                                       origin;
   TITAN_INT                                          width, height;
   TITAN_HANDLE64                                     console_line_handle;
   TITAN_UINT                                         line_size;
   TITAN_NAME                                         output_text[512];
   TITAN_UINT                                         line_index, num_lines;
   TITAN_USHORT                                       line_data_size;
   TITAN_FLOAT                                        cursor_scale;
   const TITAN_STRING                                 cursor_text = "_";
   const TITAN_STRING                                 insert_cursor_text = "^";
 
   // Enable 2D rendering
   GL->Enable2D(true);
   GL->GetWindowSize(width, height);

   // Render console background
   glDisable(GL_TEXTURE_2D);
   glLoadMatrixf(console->console_sim.transform.f.value);

   // Render background
   glBegin(GL_TRIANGLE_STRIP);
   glColor4f(0.1f, 0.1f, 0.1f, 0.3f); 
      glVertex2i(20, 40);
      glVertex2i(width - 20, 40);
      glVertex2i(20, 300);
      glVertex2i(width - 20, 300);
   glEnd();

   // Render horizontal line above text input
   glLineWidth(2.0f);
   glBegin(GL_LINES);
      glColor4f(0.8f, 0.75f, 0.5f, 0.3f);
      glVertex2i(30, 80);
      glVertex2i(width - 30, 80);
   glEnd();
   glLineWidth(1.0f);

  
   // Turn on texture mapping
   glEnable(GL_TEXTURE_2D);

   // Set text color
   glColor4fv(console->input_line_color.f.value);
   // Print input line
   glRasterPos2i(30, 50);
   GL->Font->FontPrint(NULL, 12, console->message.text);

   // Compute cursor position based on cursor index
   cursor_scale = 8.0f + (12.0f * sinf(TO_FLOAT(Tools->GetTime()) * 0.01f));
   if (cursor_scale < 0) cursor_scale = 0;
   // Print the cursor  
   if (console->overwrite) glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
   glRasterPos2i(30 + 19 * console->cursor_index, 50);
   GL->Font->FontPrint(NULL, TO_32(cursor_scale), cursor_text);

   // Render previous messages
   glColor4fv(console->console_sim.color.f.value);
   if (console->message.index == 0) console->message.handle = self->Ledger->GetLatestMessage(console->ledger, line_index, line_size, output_text, 512);
   else self->Ledger->GetMessageLine(console->message.handle, line_index, line_size, output_text, 512);
   for (num_lines = 0, console_line_handle = console->message.handle;
        console_line_handle && num_lines < 10;
        console_line_handle = self->Ledger->GetPreviousMessage(console_line_handle, line_index, line_size, output_text, 512), num_lines++)
      {
      // Report text
      if (output_text[0])
         {
         // If message data is available
         if ((message_data = _RP(console_message_data_t , self->Ledger->GetMessageData(console_line_handle, line_data_size))) != NULL)
            {
            // Set message data info
            glColor4fv(message_data->color.f.value);
            // Print the colored, scaled, text
            glRasterPos2i(30, 90 + 20 * num_lines);
            GL->Font->FontPrint(NULL, 8, output_text);
            }
         // Otherwise, print message normally
         else
            {
            // Print the colored, scaled, text
            glRasterPos2i(30, 90 + 20 * num_lines);
            GL->Font->FontPrint(NULL, 8, output_text);
            }
         // Clear text
         output_text[0] = 0;
         }
      }

   // Disable 2D rendering
   GL->Enable2D(false);

   // Report help is needed
   if (console->show_help)
      {
      // Load 3D matrix
      glLoadIdentity();
      glTranslatef(0.0f, 0.0f, -25.0f);

      // Clear the origin
      VectorSet3(origin.f.value, -20.0f, 10.0f, 0.0f);
      // Read console help transform
      // glLoadMatrixf(console->help_reference_info.transform.f.value);
      GL->Font->FontPrintv(&origin, 8, "F1               Display Help menu"); origin.xyz_f.y -= 1.6f; origin.xyz_f.x = -20.0f;

      glColor3f(238.0f / 255.0f, 207.0f / 255.0f, 161.0f / 255.0f);
      GL->Font->FontPrintv(&origin, 8, "Window:"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      glColor3f(1.0f, 1.0f, 1.0f);
      GL->Font->FontPrintv(&origin, 8, "   ENTER         Enter/Leave Console"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "   CTRL-UP       Move console window up"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "   CTRL-DOWN     Move console window down"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;

      glColor3f(238.0f / 255.0f, 207.0f / 255.0f, 161.0f / 255.0f);
      GL->Font->FontPrintv(&origin, 8, "Log:"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      glColor3f(1.0f, 1.0f, 1.0f);
      GL->Font->FontPrintv(&origin, 8, "   SHIFT-UP      Scroll log up"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "   SHIFT-DOWN    Scroll log down"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;

      glColor3f(238.0f / 255.0f, 207.0f / 255.0f, 161.0f / 255.0f);
      GL->Font->FontPrintv(&origin, 8, "Command line:"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      glColor3f(1.0f, 1.0f, 1.0f);
      GL->Font->FontPrintv(&origin, 8, "   ESC           Clear console command line"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "   UP            Get previous command line"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      GL->Font->FontPrintv(&origin, 8, "   DOWN          Get next command line"); origin.xyz_f.y -= .8f; origin.xyz_f.x = -20.0f;
      }  
}

/*
   End of file
*/
