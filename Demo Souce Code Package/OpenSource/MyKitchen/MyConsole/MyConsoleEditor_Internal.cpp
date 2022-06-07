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
   Select previous message in supplied console
*/
void MyConsoleEditor::SelectPreviousMessage(console_data_t *console, TITAN_BOOL update_message, TITAN_HANDLE64 ledger_handshake)
{
   console_message_t                                  *message = &console->message;
   TITAN_STRING                                       text_test;

   // Reset destination message text
   memset(console->message.text, 0, 128);

   // If a current message ID is specified
   if (console->message.handle)
      {
      // If we are updating the command line
      if (update_message)
         {
         // Get the previous message
         if ((message->handle = Ledger->GetPreviousMessage(message->handle, message->index, message->data_size, message->temp_text, CONSOLE_INPUT_SIZE, ledger_handshake)) != 0)
            {
            // If the previous message exists and has valid printable characters; copy it into current text
            for (text_test = console->message.temp_text; *text_test; *text_test++) 
               if (*text_test > ' ') { strcpy(console->message.text, console->message.temp_text); break; }
            }
         }
      // Otherwise
      else
         {
         // Get the previous message
         message->handle = Ledger->GetPreviousMessage(message->handle, message->index, message->data_size, NULL, 0, ledger_handshake);
         }
      }
   // Otherwise
   else
      {
      // Get the latest message
      SelectLatestMessage(console, update_message, ledger_handshake);
      }

   // Set cursor of console to end of message text
   console->cursor_index = TO_32(strlen(console->message.text));
}

/*
   Select next message in supplied console
*/
void MyConsoleEditor::SelectNextMessage(console_data_t *console, TITAN_BOOL update_message, TITAN_HANDLE64 ledger_handshake)
{
   console_message_t                                  *message = &console->message;
   TITAN_STRING                                       text_test;

   // Reset destination message text
   memset(console->message.temp_text, 0, 128);

   // If a current message ID is specified
   if (console->message.handle)
      {
      // If we are updating the command line
      if (update_message)
         {
         // Get the next message
         if ((message->handle = Ledger->GetNextMessage(message->handle, message->index, message->data_size, message->temp_text, CONSOLE_INPUT_SIZE, ledger_handshake)) != 0)
            {
            // If the next message exists and has valid printable characters; copy it into current text
            for (text_test = console->message.temp_text; *text_test; *text_test++)
               if (*text_test > ' ') { strcpy(console->message.text, console->message.temp_text); break; }
            }
         }
      // Otherwise
      else
         {
         // Get the next message
         message->handle = Ledger->GetNextMessage(message->handle, message->index, message->data_size, NULL, 0, ledger_handshake);
         }
      }
   // Otherwise
   else
      {
      // Get the latest message
      SelectLatestMessage(console, update_message, ledger_handshake);
      }

   // Set cursor of console to end of message text
   console->cursor_index = TO_32(strlen(console->message.text));
}

/*
   Select earliest message in supplied console
*/
void MyConsoleEditor::SelectEarliestMessage(console_data_t *console, TITAN_BOOL update_message, TITAN_HANDLE64 ledger_handshake)
{
   console_message_t                                  *message = &console->message;

   // Reset destination message text
   memset(console->message.text, 0, 128);

   // If we are updating the command line
   if (update_message)
      {
      // Get the earliest message
      message->handle = Ledger->GetEarliestMessage(console->ledger, message->index, message->data_size, message->text, CONSOLE_INPUT_SIZE, ledger_handshake);
      // Set cursor of console to end of message text
      console->cursor_index = TO_32(strlen(console->message.text));
      }
   // Otherwise
   else
      {
      // Get the earliest message
      message->handle = Ledger->GetEarliestMessage(console->ledger, message->index, message->data_size, NULL, 0, ledger_handshake);
      }
}

/*
   Select latest message in supplied console
*/
void MyConsoleEditor::SelectLatestMessage(console_data_t *console, TITAN_BOOL update_message, TITAN_HANDLE64 ledger_handshake)
{
   console_message_t                                  *message = &console->message;

   // Reset destination message text
   memset(console->message.text, 0, 128);

   // If we are updating the command line
   if (update_message)
      {
      // Get the latest message
      message->handle = Ledger->GetLatestMessage(console->ledger, message->index, message->data_size, message->text, CONSOLE_INPUT_SIZE, ledger_handshake);
      // Set cursor of console to end of message text
      console->cursor_index = TO_32(strlen(console->message.text));
      }
   // Otherwise
   else
      {
      // Get the latest message
      message->handle = Ledger->GetLatestMessage(console->ledger, message->index, message->data_size, NULL, 0, ledger_handshake);
      }
}


/*
   Get head position of the word on the left of the cursor
*/
void MyConsoleEditor::GetWordHeadLeft(TITAN_STRING text_string, TITAN_UINT &cursor_index, TITAN_UINT max_string_length)
{
   // If this cursor is out of range, return
   if (cursor_index == 0 || cursor_index >= max_string_length) return;
   // Move to the left
   cursor_index--;

   // Keep going left
   for ( ; cursor_index > 0 && text_string[cursor_index]; cursor_index--)
      {
      // Skip to a valid word character preceded by an invalid word character
      if (m_word_chars[text_string[cursor_index]] == true && m_word_chars[text_string[cursor_index - 1]] == false)  return;
      }
}

/*
   Get tail position of the word on the left of the cursor
*/
void MyConsoleEditor::GetWordTailLeft(TITAN_STRING text_string, TITAN_UINT &cursor_index, TITAN_UINT max_string_length)
{
   // If this cursor is out of range, return
   if (cursor_index == 0 || cursor_index >= max_string_length) return;
   // Move to the left
   cursor_index--;

   // Keep going left
   for (; cursor_index > 0 && text_string[cursor_index]; cursor_index--)
      {
      // Skip to an invalid word character preceded by a valid word character
      if (m_word_chars[text_string[cursor_index]] == false && m_word_chars[text_string[cursor_index - 1]] == true)  return;
      }
}

/*
   Get head position of the word on the right of the cursor
*/
void MyConsoleEditor::GetWordHeadRight(TITAN_STRING text_string, TITAN_UINT &cursor_index, TITAN_UINT max_string_length)
{
   // If this cursor is out of range, return
   if (cursor_index >= max_string_length - 1) return;
   // Move to the right
   if (text_string[cursor_index + 1] != 0) cursor_index++;

   // Keep going right
   for (; cursor_index < max_string_length && text_string[cursor_index]; cursor_index++)
      {
      // Skip to an invalid word character succeeded by a valid word character
      if (m_word_chars[text_string[cursor_index]] == false && m_word_chars[text_string[cursor_index + 1]] == true)
         {
         // Return next character
         cursor_index++;
         return;
         }
      }
}

/*
   Get tail position of the word on the right of the cursor
*/
void MyConsoleEditor::GetWordTailRight(TITAN_STRING text_string, TITAN_UINT &cursor_index, TITAN_UINT max_string_length)
{
   // If this cursor is out of range, return
   if (cursor_index >= max_string_length - 1) return;
   // Move to the right
   if (text_string[cursor_index + 1] != 0) cursor_index++;

   // Keep going right
   for (; cursor_index < max_string_length && text_string[cursor_index]; cursor_index++)
      {
      // Skip to a valid word character succeeded by an invalid word character
      if (m_word_chars[text_string[cursor_index]] == true && m_word_chars[text_string[cursor_index + 1]] == false)
         {
         // Return next character
         cursor_index++;
         return;
         }
      }
}

/*
   Insert text <insert_string> into <text_string> and move the cursor to the new position after insertion
*/
void MyConsoleEditor::InsertText(TITAN_STRING text_string, TITAN_STRING insert_string, TITAN_UINT &cursor_index, TITAN_UINT max_string_length, TITAN_BOOL overwrite, TITAN_STRING temp_string)
{
   TITAN_UINT                                         text_size = TO_32(strlen(text_string));
   TITAN_UINT                                         insert_size = TO_32(strlen(insert_string));
   TITAN_UINT                                         copy_size, transfer_size;

   // If this is an overwrite
   if (overwrite)
      {
      // Make sure the new string can fit in the current string
      if (insert_size + cursor_index < max_string_length) copy_size = insert_size;
      else copy_size = max_string_length - cursor_index - 1;
      // Copy inserted string if there is some space left
      if (copy_size && cursor_index + copy_size < max_string_length)
         {
         // Copy string over
         strncpy(&text_string[cursor_index], insert_string, copy_size);
         text_string[cursor_index + copy_size] = 0;
         }
      }
   // Otherwise
   else
      {
      // Make sure the new string can fit in the current string
      if (insert_size + cursor_index < max_string_length) copy_size = insert_size;
      else copy_size = max_string_length - cursor_index - 1;
      transfer_size = (text_size - cursor_index);
      if (transfer_size + cursor_index + copy_size > max_string_length) transfer_size = max_string_length - cursor_index - copy_size - 1;
      // Copy left side of the string: 0 - cursor
      strncpy(temp_string, text_string, cursor_index);
      temp_string[cursor_index] = 0;

      // Copy inserted string if there is some space left
      if (copy_size && cursor_index + copy_size < max_string_length)
         {
         // Copy inserted text: cursor - cursor + copy_size
         strncat(&temp_string[cursor_index], insert_string, copy_size);
         temp_string[cursor_index + copy_size] = 0;
         }

      // Copy remainder of the string if there is some space left
      if (transfer_size && cursor_index + copy_size + transfer_size < max_string_length)
         {
         // Copy right side of the string: cursor + copy_size - text_size - cursor
         strncat(&temp_string[cursor_index + copy_size], &text_string[cursor_index], transfer_size);
         }
      temp_string[cursor_index + copy_size + transfer_size] = 0;
      // Copy back into input string
      strcpy(text_string, temp_string);
      }
}

/*
   Insert text character <character> into <text_string> and move the cursor to the new position after insertion
*/
void MyConsoleEditor::InsertText(TITAN_STRING text_string, TITAN_CHAR character, TITAN_UINT &cursor_index, TITAN_UINT max_string_length, TITAN_BOOL overwrite)
{
   TITAN_UINT                                         text_size = TO_32(strlen(text_string));
   TITAN_UINT                                         copy_index;

   // If this is an overwrite
   if (overwrite)
      {
      // Overwrite character at current index
      text_string[cursor_index] = character;
      }
   // Otherwise
   else
      {
      // Make sure the new string can fit in the current string
      if (text_size < max_string_length) text_size++;
      copy_index = text_size;
      for (; copy_index >= cursor_index + 1; copy_index--) text_string[copy_index] = text_string[copy_index - 1];
      text_string[text_size] = 0;
      // Overwrite character at current index
      text_string[cursor_index] = character;
      cursor_index++;
      }
}

/*
   Delete string data from <start_cursor> to <end_cursor> inclusive
*/
void MyConsoleEditor::DeleteText(TITAN_STRING text_string, TITAN_UINT start_cursor, TITAN_UINT end_cursor, TITAN_UINT max_string_length)
{
   TITAN_UINT                                         text_size = TO_32(strlen(text_string));
   TITAN_UINT                                         index;
   TITAN_UINT                                         copy_size;

   // If the starting cursor is greater than the ending cursor
   if (start_cursor > end_cursor)
      {
      // Flip indices
      index = end_cursor;
      end_cursor = start_cursor;
      start_cursor = index;
      }
   // If the same cursor is both the starting position and the ending position of the clip, return
   if (start_cursor == end_cursor) return;

   // Make sure the end cursor is valid; otherwise return
   if (text_size < end_cursor) return;

   // Compute tail of the string starting at end cursor
   copy_size = text_size - end_cursor;

   // Shift content of the string to left by one
   for (index = 0; index < copy_size; index++) text_string[start_cursor + index] = text_string[end_cursor + index];
   text_string[start_cursor + index] = 0;
}

/*
   Transfer sub-string of <transfer_size> from <source_cursor> position to <destination_cursor> position of the supplied string <text_string>
*/
void MyConsoleEditor::MoveSubString(TITAN_STRING text_string, TITAN_UINT source_cursor, TITAN_UINT destination_cursor, TITAN_UINT transfer_size, TITAN_UINT max_string_size)
{
   TITAN_UINT                                         index;

   // Validate string; return failure if unsuccessful
   if (text_string == NULL) return;

   // Make sure we do not leave the string space (stay < string_size>)
   if (transfer_size + destination_cursor > max_string_size) transfer_size = max_string_size - destination_cursor;
   if (transfer_size + source_cursor > max_string_size) transfer_size = max_string_size - source_cursor;
   // If there is nothing to copy, return
   if (transfer_size == 0) return;

   // If the destination cursor is on the left of the source
   if (destination_cursor < source_cursor)
      {
      // Copy left to right
      for (index = 0; index < transfer_size; index++) text_string[destination_cursor + index] = text_string[source_cursor + index];
      }
   // Otherwise
   else if (destination_cursor > source_cursor)
      {
      // Copy right to left
      for (index = transfer_size - 1; index >= 0; index--) text_string[destination_cursor + index] = text_string[source_cursor + index];
      }
}

/*
   End of file
*/
