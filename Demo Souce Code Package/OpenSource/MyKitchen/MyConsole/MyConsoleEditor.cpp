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

/*
   Initialize the editor
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure to initialize            
*/
titan_result_t MyConsoleEditor::Initialize(MyConsole *in_console, AtlasLedgerAPI in_ledger, AtlasAttributeLocationAPI in_location)
{
   TITAN_UINT                                         index;

   // If the ledger is not specified, return failure
   if ((m_Console= in_console) == NULL) return TITAN_RESULT_INVALID_INPUT;
   if ((Ledger = in_ledger) == NULL) return TITAN_RESULT_INVALID_INPUT;
   if ((Location = in_location) == NULL) return TITAN_RESULT_INVALID_INPUT;

   // Initialize valid word characters
   memset(m_word_chars, 0, sizeof(TITAN_CHAR) * 256);
   for (index = '0'; index <= '9'; index++) m_word_chars[index] = true;
   for (index = 'A'; index <= 'Z'; index++) m_word_chars[index] = true;
   for (index = 'a'; index <= 'z'; index++) m_word_chars[index] = true;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Handle text key presses
*/
void MyConsoleEditor::Key_Text(TITAN_DOUBLE system_time, console_data_t *console, my_window_haptic_t *haptics, TITAN_INT key_shift)
{
   TITAN_INT                                          key_index;
   TITAN_BITMAP64                                     *key_states = haptics->key_states;
   TITAN_DOUBLE                                       *key_times = haptics->key_times;
   TITAN_BYTE                                         *key_values;
   TITAN_STRING                                       *key_macros;

   // Get key values and macros from the console
   if ((key_values = console->keyboard_translation_table) == NULL) return;
   if ((key_macros = console->keyboard_macros) == NULL) return;

   // Handle alphanumeric keys
   for (key_index = ' '; key_index < 256; key_index++)
      {
      // If this is not a special key, then print it
      if (key_values[key_shift + key_index])
         {
         // Handle first toggle; slower response
         if (key_states[key_index] & KEY_STATE_ON)
            {
            // Handle first toggle; slower response
            if (key_states[key_index] & KEY_STATE_TOGGLE_ON)
               {
               // Reset selection
               console->selection_index = TITAN_UINT_MAX;
               // If the console is in overwrite mode
               if (console->overwrite)
                  {
                  // Add key value (character) to the text line
                  console->message.text[console->cursor_index] = key_values[key_shift + key_index];
                  // Move cursor forward, but do not go beyond input buffer size (keep overwriting last character instead)
                  if (console->cursor_index < CONSOLE_INPUT_MAX) console->cursor_index++;
                  }
               else
                  {
                  // Insert the character and shift the text
                  InsertText(console->message.text, key_values[key_shift + key_index], console->cursor_index, 128, false);
                  }
               // Set next trigger
               key_times[key_index] = system_time + 300.0f;
               }
            // Trigger 10 times per second
            else if (key_times[key_index] < system_time)
               {
               // If the console is in overwrite mode
               if (console->overwrite)
                  {
                  // Add key value (character) to the text line
                  console->message.text[console->cursor_index] = key_values[key_shift + key_index];
                  // Move cursor forward, but do not go beyond input buffer size (keep overwriting last character instead)
                  if (console->cursor_index < CONSOLE_INPUT_MAX) console->cursor_index++;
                  }
               else
                  {
                  // Insert the character and shift the text
                  InsertText(console->message.text, key_values[key_shift + key_index], console->cursor_index, 128, false);
                  }
               // Reset selection
               console->selection_index = TITAN_UINT_MAX;
               // Set next trigger
               key_times[key_index] = system_time + 100.0f;
               }
            }
         }
      }
}

/*
   Handle left arrow key being pressed
*/
void MyConsoleEditor::Key_Left(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
   // If the key is toggled
   if (key_flags & KEY_STATE_TOGGLE_ON)
      {
      // If the control key is pressed; skip to beginning of previous word
      if (control_key)
         {
         GetWordHeadLeft(console->message.text, console->cursor_index, 128);
         }
      // Otherwise, move cursor to the left
      else if (console->cursor_index > 0) console->cursor_index--;
      // Set next trigger
      key_time = system_time + 200.0f;
      }
   // Trigger 20 times per second
   else if (key_time < system_time)
      {
      // If the control key is pressed; skip to beginning of previous word
      if (control_key) GetWordHeadLeft(console->message.text, console->cursor_index, 128);
      // Otherwise, move cursor to the left
      else if (console->cursor_index > 0) console->cursor_index--;
      // Set next trigger
      key_time = system_time + 50.0f;
      }
}

/*
   Handle right arrow key being pressed
*/
void MyConsoleEditor::Key_Right(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
   // If the key is toggled
   if (key_flags & KEY_STATE_TOGGLE_ON)
      {
      // If the control key is pressed; skip to beginning of next word
      if (control_key) GetWordHeadRight(console->message.text, console->cursor_index, 128);
      // Otherwise, move cursor to the right
      else if (console->cursor_index < strlen(console->message.text)) console->cursor_index++;
      // Set next trigger
      key_time = system_time + 200.0f;
      }
   // Trigger 20 times per second
   else if (key_time < system_time)
      {
      // If the control key is pressed; skip to beginning of next word
      if (control_key) GetWordHeadRight(console->message.text, console->cursor_index, 128);
      // Otherwise, move cursor to the right
      else if (console->cursor_index < strlen(console->message.text)) console->cursor_index++;
      // Set next trigger
      key_time = system_time + 50.0f;
      }
}

/*
   Handle backspace key being pressed
*/
void MyConsoleEditor::Key_Backspace(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
   // If the key is toggled
   if (key_flags & KEY_STATE_TOGGLE_ON)
      {
      // If this is a word delete (ctrl-backspace)
      if (control_key)
         {
         // Select to tail of previous word
         console->selection_index = console->cursor_index;
         GetWordTailLeft(console->message.text, console->cursor_index, 128);
         DeleteText(console->message.text, console->selection_index, console->cursor_index, 128);
         }
      // Otherwise, this is a single character delete
      else
         {
         // Go back one character; delete text at that character and shift everything to the left
         if (console->cursor_index > 0) console->cursor_index--;
         DeleteText(console->message.text, console->cursor_index, console->cursor_index + 1, 128);
         }
      // Trigger 5 times a second
      key_time = system_time + 200.0f;
      }
   // Trigger 20 times per second for normal character delete
   else if (key_time < system_time && control_key == false)
      {
      // Go back one character; delete text at that character and shift everything to the left
      if (console->cursor_index > 0) console->cursor_index--;
      DeleteText(console->message.text, console->cursor_index, console->cursor_index + 1, 128);
      // Set next trigger
      key_time = system_time + 50.0f;
      }
}

/*
   Handle delete key being pressed
*/
void MyConsoleEditor::Key_Delete(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
   // If the key is toggled
   if (key_flags & KEY_STATE_TOGGLE_ON)
      {
      // If this is a word delete (ctrl-delete)
      if (control_key)
         {
         // Select to tail of previous word
         console->selection_index = console->cursor_index;
         GetWordHeadRight(console->message.text, console->selection_index, 128);
         DeleteText(console->message.text, console->cursor_index, console->selection_index, 128);
         }
      // Otherwise, this is a single character delete
      else
         {
         // Delete text at cursor and shift everything to the left
         DeleteText(console->message.text, console->cursor_index, console->cursor_index + 1, 128);
         // Trigger 5 times a second
         key_time = system_time + 200.0f;
         }
      }
   // Trigger 20 times per second for normal character delete
   else if (key_time < system_time && control_key == false)
      {
      // Delete text at cursor and shift everything to the left
      DeleteText(console->message.text, console->cursor_index, console->cursor_index + 1, 128);
      // Set next trigger
      key_time = system_time + 50.0f;
      }
}

/*
   Handle up arrow key being pressed
*/
void MyConsoleEditor::Key_Up(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
   // If the control key is pressed, move console window up
   if (control_key)
      {
      // Increase Y coordinate of the console
      console->console_sim.coordinates.position.xyzt_f.y += 10.0f;
      // Set time it takes to move console up
      console->console_sim.coordinates.position.xyzt_f.t = .1f;
      console->console_sim.coordinates.update_flags |= TITAN_COORDINATE_TRANSLATE;
      // Update console coordinate
      Location->UpdateCoordinate(console->sim_object);
      }
   // If the shift key is pressed
   else if (shift_key)
      {
      // Scroll text up
      SelectPreviousMessage(console, false);
      }
   // Otherwise, get the previous command
   else
      {
      // Select the previous text in the console history
      SelectPreviousMessage(console, true);
      }
}

/*
   Handle down arrow key being pressed
*/
void MyConsoleEditor::Key_Down(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
   // If the control key is pressed, move console window down
   if (control_key)
      {
      // Increase Y coordinate of the console
      console->console_sim.coordinates.position.xyzt_f.y -= 10.0f;
      // Set time it takes to move console down
      console->console_sim.coordinates.position.xyzt_f.t = .1f;
      console->console_sim.coordinates.update_flags |= TITAN_COORDINATE_TRANSLATE;
      // Update console coordinate
      Location->UpdateCoordinate(console->sim_object);
      }
   // If the shift key is pressed
   else if (shift_key)
      {
      // Scroll text down
      SelectNextMessage(console, false);
      }
   // Otherwise, get the next command
   else
      {
      // Select the next text in the console history
      SelectNextMessage(console, true);
      }
}

/*
   Handle the home key being pressed
*/
void MyConsoleEditor::Key_Home(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
   // Go to the beginning of the line
   console->cursor_index = 0;
}

/*
   Handle the end key being pressed
*/
void MyConsoleEditor::Key_End(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
   // Go to the end of the line
   console->cursor_index = TO_32(strlen(console->message.text));
}

/*
   If the escape key is pressed
*/
void MyConsoleEditor::Key_ESCAPE(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
   // Clear current text line and reset cursor to first character
   console->cursor_index = 0;
   memset(console->message.text, 0, 128);
}

/*
   Handle F1 key being pressed
*/
void MyConsoleEditor::Key_F1(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{  
   // If the help menu is not already showing
   if (console->show_help == false)
      {
      // Set help text coordinates to be middle of the screen
      VectorSet4(console->help_sim.coordinates.position.f.value, -23.0f, 12.0f, -30.0f, 0.1f);
      // Set help text scale to be normal, and get there in 100ms
      VectorSet4(console->help_sim.coordinates.scale.f.value, 1.0f, 1.0f, 1.0f, 0.1f);
      // Request call-back from simulation when destination is reached
      console->help_sim.coordinates.trigger.code_handle = TO_HANDLE64(m_Console);
      console->help_sim.coordinates.trigger.data_handle = TO_HANDLE64(console);
      console->help_sim.coordinates.trigger.Trigger = &MyConsole::T_EnterHelp;
      // Turn on rendering of help text
      console->show_help = true;
      }
   // If the help menu is already showing
   else
      {
      // Set help text Y coordinate to be above the screen
      console->help_sim.coordinates.position.xyzt_f.y = 30.0f;
      // Set help text scale to be be tiny, and get there in 100ms
      VectorSet4(console->help_sim.coordinates.scale.f.value, 0.1f, 0.1f, 0.1f, 0.1f);
      // Request call-back from simulation when destination is reached
      console->help_sim.coordinates.trigger.Trigger = &MyConsole::T_LeaveHelp;
      // Turn off rendering of help text
      console->show_help = false;
      }

   // Update coordinate / animation
   Location->UpdateCoordinate(console->sim_object, TITAN_COORDINATE_TRANSLATE | TITAN_COORDINATE_SCALE);
}

/*
   If the copy key is (CTRL-C) pressed
*/
void MyConsoleEditor::Key_Copy(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
}

/*
   If the paste key (CTRL-V or SHIFT-INSERT) is pressed
*/
void MyConsoleEditor::Key_Paste(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key)
{
   // Copy data from clipboard to memory (paste)
   m_Console->CopyFromClipboard(NULL, &console->message.text[console->cursor_index], CONSOLE_INPUT_MAX - console->cursor_index);
   // Set cursor of console to the end of the pasted text
   console->cursor_index = TO_32(strlen(console->message.text));
}

/*
   End of file
*/
