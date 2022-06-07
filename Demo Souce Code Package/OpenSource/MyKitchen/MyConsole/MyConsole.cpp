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
   Class constructor
*/
MyConsole::MyConsole(TITAN_HANDLE64 handshake) : TitanAxiom(handshake)
{
   // Initialize variables
   m_console_interface_label = 0;
   m_console_instance_data_label = 0;
   m_v_camera = 0;
   m_agent = 0;
   m_user_clone = 0;
   m_agent_reference = 0;
   m_concept = 0;
   m_window_width = 0;
   m_window_height = 0;
   m_client_center_x = 0;
   m_client_center_y = 0;
   m_window_center_x = 0;
   m_window_center_y = 0;
   m_window_aspect_ratio = 1.0f;
   m_wait = false;

   // Initialize pointers
   m_GL = NULL;
   Location = NULL;
   Size = NULL;
   Color = NULL;
}

/*
   Class destructor
*/
MyConsole::~MyConsole()
{
}

/*
   Get element specified by component id <component_id> of supplied directive <directive>, where:
      <reference_info> is the structure used to retrieve reference data of the first component matching <component_id> in the supplied directive 
      <instance_data> is the output pointer to the instance of the directive component
      <query_flags> is the parameter passed to the directive interface Get (See Directive::Get in Atlas SDK)

   Returns: Handle of the directive component retrieved upon success
            0 upon failure to retrieve the requested directive component
*/
TITAN_HANDLE64 MyConsole::GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info,
                                     console_data_t **instance_data, TITAN_BITMAP64 query_flags)
{
   TITAN_HANDLE64                                     component;

   // Get the directive object; return failure if unsuccessful
   if ((component = Directive->Get(directive, component_id, reference_info, query_flags | ATLAS_GET_INSTANCE | ATLAS_GET_REFERENCE)) == 0) return 0;

   // If instance data is requested
   if (instance_data)
      {
      // Make sure the returned reference can be handled by this concept; return failure otherwise
      if (Cognitive->IsCompatible(reference_info.reference) != true) return 0;

      // Get reference instance and/or element
      if (reference_info.element) *instance_data = _RP(console_data_t, reference_info.element);
      else if ((*instance_data = _RP(console_data_t, reference_info.instance)) == NULL) return 0;
      }

   // Return component handle
   return component;
}

/*
   Set console keyboard translation table for supplied console <console>
   The table is a list of translation characters for each one of the virtual keys. There are
   256 possible keys, and there are two states for each key - standard, and shifted.
*/
void MyConsole::SetConsoleKeyboardTranslationTable(console_data_t *console)
{
   TITAN_BYTE                                         *value;
   TITAN_STRING                                       *key_macros;
   TITAN_UINT                                         key_index;

   // Validate input; return failure if unsuccessful
   if (console == NULL) return;

   // Get the translation table of the console; return if unsuccessful
   if ((value = console->keyboard_translation_table) == NULL) return;
   if ((key_macros = console->keyboard_macros) == NULL) return;

   // Clear keyboard key values and macros
   memset(value, 0, sizeof(TITAN_BYTE) * 256 * 2);
   memset(key_macros, 0, sizeof(TITAN_STRING) * 256);

   // Initialize the table
   value[0x30] = '0'; value[0x130] = ')'; key_macros[0x30] = "0";
   value[0x31] = '1'; value[0x131] = '!'; key_macros[0x31] = "1";
   value[0x32] = '2'; value[0x132] = '@'; key_macros[0x32] = "2";
   value[0x33] = '3'; value[0x133] = '#'; key_macros[0x33] = "3";
   value[0x34] = '4'; value[0x134] = '$'; key_macros[0x34] = "4";
   value[0x35] = '5'; value[0x135] = '%'; key_macros[0x35] = "5";
   value[0x36] = '6'; value[0x136] = '^'; key_macros[0x36] = "6";
   value[0x37] = '7'; value[0x137] = '&'; key_macros[0x37] = "7";
   value[0x38] = '8'; value[0x138] = '*'; key_macros[0x38] = "8";
   value[0x39] = '9'; value[0x139] = '('; key_macros[0x39] = "9";

   // Space key:
   value[' '] = ' '; value[0x100 + ' '] = ' ';

   // Handle alpha keys: 'A'-'Z'
   for (key_index = 'A'; key_index <= 'Z'; key_index++)
      {
      // Set normal key value to lower case (assuming caps lock is off)
      value[key_index] = TO_08s(key_index) | 32;
      // Set shifted key value to upper case (assuming caps lock is off)
      value[0x100 + key_index] = TO_08s(key_index);
      }

   // Set numeric pad key
   value[VK_NUMPAD0] = '0';
   value[VK_NUMPAD1] = '1';
   value[VK_NUMPAD2] = '2';
   value[VK_NUMPAD3] = '3';
   value[VK_NUMPAD4] = '4';
   value[VK_NUMPAD5] = '5';
   value[VK_NUMPAD6] = '6';
   value[VK_NUMPAD7] = '7';
   value[VK_NUMPAD8] = '8';
   value[VK_NUMPAD9] = '9';
   value[VK_MULTIPLY] = '*';
   value[VK_ADD] = '+';
   value[VK_SUBTRACT] = '-';
   value[VK_DECIMAL] = '.';
   value[VK_DIVIDE] = '/';

   value[VK_LBUTTON] = '\0'; value[0x100 + VK_LBUTTON] = '\0';
   value[VK_RBUTTON] = '\0'; value[0x100 + VK_RBUTTON] = '\0';
   value[VK_CANCEL] = '\0'; value[0x100 + VK_CANCEL] = '\0';
   value[VK_MBUTTON] = '\0'; value[0x100 + VK_MBUTTON] = '\0';
   value[VK_XBUTTON1] = '\0'; value[0x100 + VK_XBUTTON1] = '\0';
   value[VK_XBUTTON2] = '\0'; value[0x100 + VK_XBUTTON2] = '\0';
   value[VK_BACK] = '\0'; value[0x100 + VK_BACK] = '\0';
   value[VK_TAB] = '\0'; value[0x100 + VK_TAB] = '\0';
   value[VK_CLEAR] = '\0'; value[0x100 + VK_CLEAR] = '\0';
   value[VK_RETURN] = '\0'; value[0x100 + VK_RETURN] = '\0';
   value[VK_SHIFT] = '\0'; value[0x100 + VK_SHIFT] = '\0';
   value[VK_CONTROL] = '\0'; value[0x100 + VK_CONTROL] = '\0';
   value[VK_MENU] = '\0'; value[0x100 + VK_MENU] = '\0';
   value[VK_PAUSE] = '\0'; value[0x100 + VK_PAUSE] = '\0';
   value[VK_CAPITAL] = '\0'; value[0x100 + VK_CAPITAL] = '\0';
   value[VK_KANA] = '\0'; value[0x100 + VK_KANA] = '\0';
   value[VK_HANGUL] = '\0'; value[0x100 + VK_HANGUL] = '\0';
   value[VK_JUNJA] = '\0'; value[0x100 + VK_JUNJA] = '\0';
   value[VK_FINAL] = '\0'; value[0x100 + VK_FINAL] = '\0';
   value[VK_HANJA] = '\0'; value[0x100 + VK_HANJA] = '\0';
   value[VK_KANJI] = '\0'; value[0x100 + VK_KANJI] = '\0';
   value[VK_ESCAPE] = '\0'; value[0x100 + VK_ESCAPE] = '\0';
   value[VK_CONVERT] = '\0'; value[0x100 + VK_CONVERT] = '\0';
   value[VK_NONCONVERT] = '\0'; value[0x100 + VK_NONCONVERT] = '\0';
   value[VK_ACCEPT] = '\0'; value[0x100 + VK_ACCEPT] = '\0';
   value[VK_MODECHANGE] = '\0'; value[0x100 + VK_MODECHANGE] = '\0';
   value[VK_SPACE] = ' '; value[0x100 + VK_SPACE] = ' ';
   value[VK_PRIOR] = '\0'; value[0x100 + VK_PRIOR] = '\0';
   value[VK_NEXT] = '\0'; value[0x100 + VK_NEXT] = '\0';
   value[VK_END] = '\0'; value[0x100 + VK_END] = '\0';
   value[VK_HOME] = '\0'; value[0x100 + VK_HOME] = '\0';
   value[VK_LEFT] = '\0'; value[0x100 + VK_LEFT] = '\0';
   value[VK_UP] = '\0'; value[0x100 + VK_UP] = '\0';
   value[VK_RIGHT] = '\0'; value[0x100 + VK_RIGHT] = '\0';
   value[VK_DOWN] = '\0'; value[0x100 + VK_DOWN] = '\0';
   value[VK_SELECT] = '\0'; value[0x100 + VK_SELECT] = '\0';
   value[VK_SELECT] = '\0'; value[0x100 + VK_SELECT] = '\0';
   value[VK_PRINT] = '\0'; value[0x100 + VK_PRINT] = '\0';
   value[VK_EXECUTE] = '\0'; value[0x100 + VK_EXECUTE] = '\0';
   value[VK_SNAPSHOT] = '\0'; value[0x100 + VK_SNAPSHOT] = '\0';
   value[VK_INSERT] = '\0'; value[0x100 + VK_INSERT] = '\0';
   value[VK_DELETE] = '\0'; value[0x100 + VK_DELETE] = '\0';
   value[VK_HELP] = '\0'; value[0x100 + VK_HELP] = '\0';
   value[VK_LWIN] = '\0'; value[0x100 + VK_LWIN] = '\0';
   value[VK_RWIN] = '\0'; value[0x100 + VK_RWIN] = '\0';
   value[VK_APPS] = '\0'; value[0x100 + VK_APPS] = '\0';
   value[VK_SLEEP] = '\0'; value[0x100 + VK_SLEEP] = '\0';
   value[VK_NUMPAD0] = '\0'; value[0x100 + VK_NUMPAD0] = '\0';
   value[VK_NUMPAD1] = '\0'; value[0x100 + VK_NUMPAD1] = '\0';
   value[VK_NUMPAD2] = '\0'; value[0x100 + VK_NUMPAD2] = '\0';
   value[VK_NUMPAD3] = '\0'; value[0x100 + VK_NUMPAD3] = '\0';
   value[VK_NUMPAD4] = '\0'; value[0x100 + VK_NUMPAD4] = '\0';
   value[VK_NUMPAD5] = '\0'; value[0x100 + VK_NUMPAD5] = '\0';
   value[VK_NUMPAD6] = '\0'; value[0x100 + VK_NUMPAD6] = '\0';
   value[VK_NUMPAD7] = '\0'; value[0x100 + VK_NUMPAD7] = '\0';
   value[VK_NUMPAD8] = '\0'; value[0x100 + VK_NUMPAD8] = '\0';
   value[VK_NUMPAD9] = '\0'; value[0x100 + VK_NUMPAD9] = '\0';
   value[VK_MULTIPLY] = '*'; value[0x100 + VK_MULTIPLY] = '*';
   value[VK_ADD] = '+'; value[0x100 + VK_ADD] = '+';
   value[VK_SEPARATOR] = '\0'; value[0x100 + VK_SEPARATOR] = '\0';
   value[VK_SUBTRACT] = '-'; value[0x100 + VK_SUBTRACT] = '-';
   value[VK_DECIMAL] = '.'; value[0x100 + VK_DECIMAL] = '.';
   value[VK_DIVIDE] = '/'; value[0x100 + VK_DIVIDE] = '/';
   value[VK_F1] = '\0'; value[0x100 + VK_F1] = '\0';
   value[VK_F2] = '\0'; value[0x100 + VK_F2] = '\0';
   value[VK_F3] = '\0'; value[0x100 + VK_F3] = '\0';
   value[VK_F4] = '\0'; value[0x100 + VK_F4] = '\0'; 
   value[VK_F5] = '\0'; value[0x100 + VK_F5] = '\0';
   value[VK_F6] = '\0'; value[0x100 + VK_F6] = '\0'; 
   value[VK_F7] = '\0'; value[0x100 + VK_F7] = '\0';
   value[VK_F8] = '\0'; value[0x100 + VK_F8] = '\0';
   value[VK_F9] = '\0'; value[0x100 + VK_F9] = '\0';
   value[VK_F10] = '\0'; value[0x100 + VK_F10] = '\0';
   value[VK_F11] = '\0'; value[0x100 + VK_F11] = '\0';
   value[VK_F12] = '\0'; value[0x100 + VK_F12] = '\0';
   value[VK_F13] = '\0'; value[0x100 + VK_F13] = '\0';
   value[VK_F14] = '\0'; value[0x100 + VK_F14] = '\0';
   value[VK_F15] = '\0'; value[0x100 + VK_F15] = '\0';
   value[VK_F16] = '\0'; value[0x100 + VK_F16] = '\0';
   value[VK_F17] = '\0'; value[0x100 + VK_F17] = '\0';
   value[VK_F18] = '\0'; value[0x100 + VK_F18] = '\0';
   value[VK_F19] = '\0'; value[0x100 + VK_F19] = '\0';
   value[VK_F20] = '\0'; value[0x100 + VK_F20] = '\0';
   value[VK_F21] = '\0'; value[0x100 + VK_F21] = '\0';
   value[VK_F22] = '\0'; value[0x100 + VK_F22] = '\0';
   value[VK_F23] = '\0'; value[0x100 + VK_F23] = '\0';
   value[VK_F24] = '\0'; value[0x100 + VK_F24] = '\0';
   value[VK_NUMLOCK] = '\0'; value[0x100 + VK_NUMLOCK] = '\0';
   value[VK_SCROLL] = '\0'; value[0x100 + VK_SCROLL] = '\0';
   value[VK_LSHIFT] = '\0'; value[0x100 + VK_LSHIFT] = '\0';
   value[VK_RSHIFT] = '\0'; value[0x100 + VK_RSHIFT] = '\0';
   value[VK_LCONTROL] = '\0'; value[0x100 + VK_LCONTROL] = '\0';
   value[VK_RCONTROL] = '\0'; value[0x100 + VK_RCONTROL] = '\0';
   value[VK_LMENU] = '\0'; value[0x100 + VK_LMENU] = '\0';
   value[VK_RMENU] = '\0'; value[0x100 + VK_RMENU] = '\0';
   value[VK_BROWSER_BACK] = '\0'; value[0x100 + VK_BROWSER_BACK] = '\0';
   value[VK_BROWSER_FORWARD] = '\0'; value[0x100 + VK_BROWSER_FORWARD] = '\0';
   value[VK_BROWSER_REFRESH] = '\0'; value[0x100 + VK_BROWSER_REFRESH] = '\0';
   value[VK_BROWSER_STOP] = '\0'; value[0x100 + VK_BROWSER_STOP] = '\0';
   value[VK_BROWSER_SEARCH] = '\0'; value[0x100 + VK_BROWSER_SEARCH] = '\0';
   value[VK_BROWSER_FAVORITES] = '\0'; value[0x100 + VK_BROWSER_FAVORITES] = '\0';
   value[VK_BROWSER_HOME] = '\0'; value[0x100 + VK_BROWSER_HOME] = '\0';
   value[VK_VOLUME_MUTE] = '\0'; value[0x100 + VK_VOLUME_MUTE] = '\0';
   value[VK_VOLUME_DOWN] = '\0'; value[0x100 + VK_VOLUME_DOWN] = '\0';
   value[VK_VOLUME_UP] = '\0'; value[0x100 + VK_VOLUME_UP] = '\0';
   value[VK_MEDIA_NEXT_TRACK] = '\0'; value[0x100 + VK_MEDIA_NEXT_TRACK] = '\0';
   value[VK_MEDIA_PREV_TRACK] = '\0'; value[0x100 + VK_MEDIA_PREV_TRACK] = '\0';
   value[VK_MEDIA_STOP] = '\0'; value[0x100 + VK_MEDIA_STOP] = '\0';
   value[VK_MEDIA_PLAY_PAUSE] = '\0'; value[0x100 + VK_MEDIA_PLAY_PAUSE] = '\0';
   value[VK_LAUNCH_MAIL] = '\0'; value[0x100 + VK_LAUNCH_MAIL] = '\0';
   value[VK_LAUNCH_MEDIA_SELECT] = '\0'; value[0x100 + VK_LAUNCH_MEDIA_SELECT] = '\0';
   value[VK_LAUNCH_APP1] = '\0'; value[0x100 + VK_LAUNCH_APP1] = '\0';
   value[VK_LAUNCH_APP2] = '\0'; value[0x100 + VK_LAUNCH_APP2] = '\0';
   value[VK_OEM_1] = ';'; value[0x100 + VK_OEM_1] = ':';
   value[VK_OEM_PLUS] = '='; value[0x100 + VK_OEM_PLUS] = '+';
   value[VK_OEM_COMMA] = ','; value[0x100 + VK_OEM_COMMA] = '<';
   value[VK_OEM_MINUS] = '-'; value[0x100 + VK_OEM_MINUS] = '+';
   value[VK_OEM_PERIOD] = '.'; value[0x100 + VK_OEM_PERIOD] = '>';
   value[VK_OEM_2] = '/'; value[0x100 + VK_OEM_2] = '?';
   value[VK_OEM_3] = '`'; value[0x100 + VK_OEM_3] = '~';
   value[VK_OEM_4] = '['; value[0x100 + VK_OEM_4] = '{';
   value[VK_OEM_5] = '\\'; value[0x100 + VK_OEM_5] = '|';
   value[VK_OEM_6] = ']'; value[0x100 + VK_OEM_6] = '}';
   value[VK_OEM_7] = '\''; value[0x100 + VK_OEM_7] = '\"';
   value[VK_OEM_8] = '\0'; value[0x100 + VK_OEM_8] = '\0';
   value[VK_OEM_8] = '\0'; value[0x100 + VK_OEM_8] = '\0';
   value[VK_OEM_102] = '\0'; value[0x100 + VK_OEM_102] = '\0';
   value[VK_PROCESSKEY] = '\0'; value[0x100 + VK_PROCESSKEY] = '\0';
   value[VK_PACKET] = '\0'; value[0x100 + VK_PACKET] = '\0';
   value[VK_ATTN] = '\0'; value[0x100 + VK_ATTN] = '\0';
   value[VK_CRSEL] = '\0'; value[0x100 + VK_CRSEL] = '\0';
   value[VK_EXSEL] = '\0'; value[0x100 + VK_EXSEL] = '\0';
   value[VK_EREOF] = '\0'; value[0x100 + VK_EREOF] = '\0';
   value[VK_PLAY] = '\0'; value[0x100 + VK_PLAY] = '\0';
   value[VK_ZOOM] = '\0'; value[0x100 + VK_ZOOM] = '\0';
   value[VK_NONAME] = '\0'; value[0x100 + VK_NONAME] = '\0';
   value[VK_PA1] = '\0'; value[0x100 + VK_PA1] = '\0';
   value[VK_OEM_CLEAR] = '\0'; value[0x100 + VK_OEM_CLEAR] = '\0';


   #ifdef DEBUG_KEYS
   key_macros[VK_LBUTTON] = "VK_LBUTTON";
   key_macros[VK_RBUTTON] = "VK_RBUTTON";
   key_macros[VK_CANCEL] = "VK_CANCEL";
   key_macros[VK_MBUTTON] = "VK_MBUTTON";
   key_macros[VK_XBUTTON1] = "VK_XBUTTON1";
   key_macros[VK_XBUTTON2] = "VK_XBUTTON2";
   key_macros[VK_BACK] = "VK_BACK";
   key_macros[VK_TAB] = "VK_TAB";
   key_macros[VK_CLEAR] = "VK_CLEAR";
   key_macros[VK_RETURN] = "VK_RETURN";
   key_macros[VK_SHIFT] = "VK_SHIFT";
   key_macros[VK_CONTROL] = "VK_CONTROL";
   key_macros[VK_MENU] = "VK_MENU";
   key_macros[VK_PAUSE] = "VK_PAUSE";
   key_macros[VK_CAPITAL] = "VK_CAPITAL";
   key_macros[VK_KANA] = "VK_KANA";
   key_macros[VK_HANGUL] = "VK_HANGUL";
   key_macros[VK_JUNJA] = "VK_JUNJA";
   key_macros[VK_FINAL] = "VK_FINAL";
   key_macros[VK_HANJA] = "VK_HANJA";
   key_macros[VK_KANJI] = "VK_KANJI";
   key_macros[VK_ESCAPE] = "VK_ESCAPE";
   key_macros[VK_CONVERT] = "VK_CONVERT";
   key_macros[VK_NONCONVERT] = "VK_NONCONVERT";
   key_macros[VK_ACCEPT] = "VK_ACCEPT";
   key_macros[VK_MODECHANGE] = "VK_MODECHANGE";
   key_macros[VK_SPACE] = "VK_SPACE";
   key_macros[VK_PRIOR] = "VK_PRIOR";
   key_macros[VK_NEXT] = "VK_NEXT";
   key_macros[VK_END] = "VK_END";
   key_macros[VK_HOME] = "VK_HOME";
   key_macros[VK_LEFT] = "VK_LEFT";
   key_macros[VK_UP] = "VK_UP";
   key_macros[VK_RIGHT] = "VK_RIGHT";
   key_macros[VK_DOWN] = "VK_DOWN";
   key_macros[VK_SELECT] = "VK_SELECT";
   key_macros[VK_SELECT] = "VK_SELECT";
   key_macros[VK_PRINT] = "VK_PRINT";
   key_macros[VK_EXECUTE] = "VK_EXECUTE";
   key_macros[VK_SNAPSHOT] = "VK_SNAPSHOT";
   key_macros[VK_INSERT] = "VK_INSERT";
   key_macros[VK_DELETE] = "VK_DELETE";
   key_macros[VK_HELP] = "VK_HELP";
   key_macros[VK_LWIN] = "VK_LWIN";
   key_macros[VK_RWIN] = "VK_RWIN";
   key_macros[VK_APPS] = "VK_APPS";
   key_macros[VK_SLEEP] = "VK_SLEEP";
   key_macros[VK_NUMPAD0] = "VK_NUMPAD0";
   key_macros[VK_NUMPAD1] = "VK_NUMPAD1";
   key_macros[VK_NUMPAD2] = "VK_NUMPAD2";
   key_macros[VK_NUMPAD3] = "VK_NUMPAD3";
   key_macros[VK_NUMPAD4] = "VK_NUMPAD4";
   key_macros[VK_NUMPAD5] = "VK_NUMPAD5";
   key_macros[VK_NUMPAD6] = "VK_NUMPAD6";
   key_macros[VK_NUMPAD7] = "VK_NUMPAD7";
   key_macros[VK_NUMPAD8] = "VK_NUMPAD8";
   key_macros[VK_NUMPAD9] = "VK_NUMPAD9";
   key_macros[VK_MULTIPLY] = "VK_MULTIPLY";
   key_macros[VK_ADD] = "VK_ADD";
   key_macros[VK_SEPARATOR] = "VK_SEPARATOR";
   key_macros[VK_SUBTRACT] = "VK_SUBTRACT";
   key_macros[VK_DECIMAL] = "VK_DECIMAL";
   key_macros[VK_DIVIDE] = "VK_DIVIDE";
   key_macros[VK_F1] = "VK_F1";
   key_macros[VK_F2] = "VK_F2";
   key_macros[VK_F3] = "VK_F3";
   key_macros[VK_F4] = "VK_F4";
   key_macros[VK_F5] = "VK_F5";
   key_macros[VK_F6] = "VK_F6";
   key_macros[VK_F7] = "VK_F7";
   key_macros[VK_F8] = "VK_F8";
   key_macros[VK_F9] = "VK_F9";
   key_macros[VK_F10] = "VK_F10";
   key_macros[VK_F11] = "VK_F11";
   key_macros[VK_F12] = "VK_F12";
   key_macros[VK_F13] = "VK_F13";
   key_macros[VK_F14] = "VK_F14";
   key_macros[VK_F15] = "VK_F15";
   key_macros[VK_F16] = "VK_F16";
   key_macros[VK_F17] = "VK_F17";
   key_macros[VK_F18] = "VK_F18";
   key_macros[VK_F19] = "VK_F19";
   key_macros[VK_F20] = "VK_F20";
   key_macros[VK_F21] = "VK_F21";
   key_macros[VK_F22] = "VK_F22";
   key_macros[VK_F23] = "VK_F23";
   key_macros[VK_F24] = "VK_F24";
   key_macros[VK_NUMLOCK] = "VK_NUMLOCK";
   key_macros[VK_SCROLL] = "VK_SCROLL";
   key_macros[VK_LSHIFT] = "VK_LSHIFT";
   key_macros[VK_RSHIFT] = "VK_RSHIFT";
   key_macros[VK_LCONTROL] = "VK_LCONTROL";
   key_macros[VK_RCONTROL] = "VK_RCONTROL";
   key_macros[VK_LMENU] = "VK_LMENU";
   key_macros[VK_RMENU] = "VK_RMENU";
   key_macros[VK_BROWSER_BACK] = "VK_BROWSER_BACK";
   key_macros[VK_BROWSER_FORWARD] = "VK_BROWSER_FORWARD";
   key_macros[VK_BROWSER_REFRESH] = "VK_BROWSER_REFRESH";
   key_macros[VK_BROWSER_STOP] = "VK_BROWSER_STOP";
   key_macros[VK_BROWSER_SEARCH] = "VK_BROWSER_SEARCH";
   key_macros[VK_BROWSER_FAVORITES] = "VK_BROWSER_FAVORITES";
   key_macros[VK_BROWSER_HOME] = "VK_BROWSER_HOME";
   key_macros[VK_VOLUME_MUTE] = "VK_VOLUME_MUTE";
   key_macros[VK_VOLUME_DOWN] = "VK_VOLUME_DOWN";
   key_macros[VK_VOLUME_UP] = "VK_VOLUME_UP";
   key_macros[VK_MEDIA_NEXT_TRACK] = "VK_MEDIA_NEXT_TRACK";
   key_macros[VK_MEDIA_PREV_TRACK] = "VK_MEDIA_PREV_TRACK";
   key_macros[VK_MEDIA_STOP] = "VK_MEDIA_STOP";
   key_macros[VK_MEDIA_PLAY_PAUSE] = "VK_MEDIA_PLAY_PAUSE";
   key_macros[VK_LAUNCH_MAIL] = "VK_LAUNCH_MAIL";
   key_macros[VK_LAUNCH_MEDIA_SELECT] = "VK_LAUNCH_MEDIA_SELECT";
   key_macros[VK_LAUNCH_APP1] = "VK_LAUNCH_APP1";
   key_macros[VK_LAUNCH_APP2] = "VK_LAUNCH_APP2";
   key_macros[VK_OEM_1] = "VK_OEM_1";
   key_macros[VK_OEM_PLUS] = "VK_OEM_PLUS";
   key_macros[VK_OEM_COMMA] = "VK_OEM_COMMA";
   key_macros[VK_OEM_MINUS] = "VK_OEM_MINUS";
   key_macros[VK_OEM_PERIOD] = "VK_OEM_PERIOD";
   key_macros[VK_OEM_2] = "VK_OEM_2";
   key_macros[VK_OEM_3] = "VK_OEM_3";
   key_macros[VK_OEM_4] = "VK_OEM_4";
   key_macros[VK_OEM_5] = "VK_OEM_5";
   key_macros[VK_OEM_6] = "VK_OEM_6";
   key_macros[VK_OEM_7] = "VK_OEM_7";
   key_macros[VK_OEM_8] = "VK_OEM_8";
   key_macros[VK_OEM_8] = "VK_OEM_8";
   key_macros[VK_OEM_102] = "VK_OEM_102";
   key_macros[VK_PROCESSKEY] = "VK_PROCESSKEY";
   key_macros[VK_PACKET] = "VK_PACKET";
   key_macros[VK_ATTN] = "VK_ATTN";
   key_macros[VK_CRSEL] = "VK_CRSEL";
   key_macros[VK_EXSEL] = "VK_EXSEL";
   key_macros[VK_EREOF] = "VK_EREOF";
   key_macros[VK_PLAY] = "VK_PLAY";
   key_macros[VK_ZOOM] = "VK_ZOOM";
   key_macros[VK_NONAME] = "VK_NONAME";
   key_macros[VK_PA1] = "VK_PA1";
   key_macros[VK_OEM_CLEAR] = "VK_OEM_CLEAR";
   #endif
   key_macros[VK_F12] = "This is a macro test!";
}

/*
   Execute the supplied expression
*/
titan_result_t MyConsole::Tell(console_data_t *console, TITAN_HANDLE64 source, TITAN_HANDLE64 destination, const TITAN_STRING text)
{
   TITAN_HANDLE64                                     group_handle, element;
   atlas_reference_info_t                             object = { 0 };
   TITAN_NAME                                         reference_name[128];
   titan_vec4_t                                       color;

   // If the console is not valid; return failure
   if (console == NULL) return TITAN_RESULT_NOT_INITIALIZED;

   // Validate source reference
   if (source == 0) return TITAN_RESULT_INVALID_INPUT;

   // Tell the destination agent
   Agent->Tell(source, destination, text);

   // Get the who attachment group
   if ((group_handle = Agent->_GetAttachment(source, ATLAS_ATTACHMENT_WHO)) != 0)
      {
      // Print message
      VectorSet4(color.f.value, 1.0f, 1.0f, 0.0f, 1.0f);
      Print(console, NULL, 2.0f, &color, "Results:");

      // Initialize reference data name buffer
      object.name = reference_name;
      object.name_max_size = 127;
      // Request first member of the attachment group 
      if (element = Group->GetFirst(group_handle, object, ATLAS_GET_NONE))
         {
         // Display first member
         VectorSet4(color.f.value, 0.4f, 1.0f, 0.5f, 1.0f);
         Print(console, NULL, 2.0f, &color, "    %s", reference_name);
         // Request all remaining members in the attachment group
         while (element = Group->GetNext(element, object, ATLAS_GET_NONE))
            {
            // Display first member
            VectorSet4(color.f.value, 0.4f, 1.0f, 0.5f, 1.0f);
            Print(console, NULL, 2.0f, &color, "    %s", reference_name);
            }
         }
      }

   // Return result
   return TITAN_RESULT_OK;
}

/*
   Register a new perspective reference for current console
   If the perspective handle is not specified, use the console reference as the perspective 
*/
void MyConsole::RegisterPerspective(console_data_t *console, TITAN_HANDLE64 perspective_handle)
{
   // Set perspective handle
   if (console == NULL) return;

   // If the perspective handle is s
   if (perspective_handle) console->target = perspective_handle;
   else console->target = console->console_sim.reference;
}

/*
   Print the message
*/
TITAN_HANDLE64 MyConsole::Print(console_data_t *console, titan_vec3_t *position, TITAN_FLOAT scale, titan_vec4_t *color, const TITAN_STRING message, ...)
{
   va_list                                            argptr;
   TITAN_ULONG                                        text_length;
   TITAN_HANDLE64                                     message_handle;
   console_message_data_t                             *message_data;
   TITAN_USHORT                                       message_data_size;

   // Format the message
   va_start(argptr, message);
   text_length = vsnprintf(console->message.text, CONSOLE_INPUT_MAX, message, argptr) + 1;
   va_end(argptr);

   // Insert message into the console
   if ((message_handle = Ledger->InsertMessage(console->ledger, sizeof(console_message_data_t), 0,  console->message.text)) != 0)
      {
      // If the message has data associated with it
      if ((message_data = _RP(console_message_data_t, Ledger->GetMessageData(message_handle, message_data_size))) != NULL && message_data_size == sizeof(console_message_data_t))
         {
         // If position is specified
         if (position)
            {
            // Copy position
            VectorCopy4(position->f.value, message_data->position.f.value);
            }
         
         // Copy scale
         message_data->scale = scale;
         
         // If color is specified
         if (color)
            {
            // Copy color value
            VectorCopy4(color->f.value, message_data->color.f.value);
            }
         }
      }

   // Return message handle
   return message_handle;
}

/*
   End of file
*/
