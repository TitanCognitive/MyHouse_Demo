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

#ifndef MY_CONCEPT_CONSOLE_H
#define MY_CONCEPT_CONSOLE_H

   // Dependencies
   #include "Atlas/TitanAtlasAxiom.h"
   #include "../MyGLAPI/MyGLAPI.h"

   // Forward declarations 
   class MyConsole;
   
   // Console input line size in bytes
   #define CONSOLE_INPUT_SIZE                         128
   #define CONSOLE_INPUT_MAX                          126

   // Console simulation data structure definition
   struct console_simulation_data_t
      {
      TITAN_HANDLE64                                  reference;                             // Reference control handle
      atlas_coordinate_t                              coordinates;                           // Instance coordinates
      titan_mat4_t                                    transform;                             // Transform
      titan_vec4_t                                    color;                                 // Color (RGBA)
      };

   // Console message data structure definition
   struct console_message_data_t
      {
      titan_vec3_t                                    position;                              // Object position
      TITAN_FLOAT                                     scale;                                 // Object scale
      titan_vec4_t                                    color;                                 // Object color
      titan_mat4_t                                    transform;                             // Object transform
      };

   // Console message structure
   struct console_message_t
      {
      TITAN_HANDLE64                                  handle;                                // Console message handle
      TITAN_UINT                                      index;                                 // Console message index
      TITAN_SIZE32                                    data_size;                             // Console message data size
      TITAN_NAME                                      text[CONSOLE_INPUT_SIZE];              // Console message text
      TITAN_NAME                                      temp_text[CONSOLE_INPUT_SIZE];         // Console message temporary text (used for text manipulation)
      };

   // Console data structure definition
   struct console_data_t
      {
      TITAN_BOOL                                      rendering;                             // True when rendering information
      TITAN_BOOL                                      show_help;                             // True when showing help menu
      TITAN_BOOL                                      overwrite;                             // True when the text is in overwrite mode (not insert)
      TITAN_UINT                                      cursor_index;                          // Text input cursor index
      TITAN_UINT                                      selection_index;                       // Text selection cursor index to mark a region [cursor, selection] or [selection, cursor]
      TITAN_HANDLE64                                  target;                                // Console target reference
      TITAN_HANDLE64                                  ledger;                                // Titan ledger used by console
      TITAN_HANDLE64                                  sim_object;                            // Simulated object of console
      console_simulation_data_t                       console_sim;                           // Console instance simulation data
      console_simulation_data_t                       help_sim;                              // Help instance simulation data
      TITAN_HANDLE64                                  selected_line_handle;                  // Titan console current line handle (0 == no line)
      titan_vec4_t                                    input_line_color;                      // Input line color (RGBA)
      console_message_t                               message;                               // Current console message
      TITAN_BYTE                                      keyboard_translation_table[256 * 2];   // Key values for regular and shifted key presses (e.g. 'a' or 'A' key for A key depending on shift and caps lock)
      TITAN_STRING                                    keyboard_macros[256];                  // Keyboard macros
      };

   // Console intercept structure definition
   struct my_console_intercept_t
      {
      MyConsole                                       *self;                                 // Console axiom
      console_data_t                                  *console;                              // Console data
      };

   // Console text editor class used by MyConsole
   class MyConsoleEditor
      {
      public:
         // Framework methods:
         titan_result_t Initialize(MyConsole *in_console, AtlasLedgerAPI in_ledger, AtlasAttributeLocationAPI in_location);

      public:
         // Public methods:
         void Key_Text(TITAN_DOUBLE system_time, console_data_t *console, my_window_haptic_t *haptics, TITAN_INT key_shift);
         void Key_Left(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_Right(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_Up(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_Down(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_Home(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_End(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_Backspace(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_Delete(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_Copy(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_Paste(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_F1(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);
         void Key_ESCAPE(TITAN_DOUBLE system_time, console_data_t *console, TITAN_BITMAP64 &key_flags, TITAN_DOUBLE &key_time, TITAN_BOOL control_key, TITAN_BOOL shift_key);

      private:
         // Message methods
         void SelectPreviousMessage(console_data_t *console, TITAN_BOOL update_message, TITAN_HANDLE64 ledger_handshake = 0);
         void SelectNextMessage(console_data_t *console, TITAN_BOOL update_message, TITAN_HANDLE64 ledger_handshake = 0);
         void SelectEarliestMessage(console_data_t *console, TITAN_BOOL update_message, TITAN_HANDLE64 ledger_handshake = 0);
         void SelectLatestMessage(console_data_t *console, TITAN_BOOL update_message, TITAN_HANDLE64 ledger_handshake = 0);

         // Keyboard methods
         void GetWordHeadLeft(TITAN_STRING text_string, TITAN_UINT &cursor_index, TITAN_UINT max_string_length);
         void GetWordTailLeft(TITAN_STRING text_string, TITAN_UINT &cursor_index, TITAN_UINT max_string_length);
         void GetWordHeadRight(TITAN_STRING text_string, TITAN_UINT &cursor_index, TITAN_UINT max_string_length);
         void GetWordTailRight(TITAN_STRING text_string, TITAN_UINT &cursor_index, TITAN_UINT max_string_length);
         void InsertText(TITAN_STRING text_string, TITAN_STRING insert_string, TITAN_UINT &cursor_index, TITAN_UINT max_string_length, TITAN_BOOL overwrite, TITAN_STRING temp_string);
         void InsertText(TITAN_STRING text_string, TITAN_CHAR character, TITAN_UINT &cursor_index, TITAN_UINT max_string_length, TITAN_BOOL overwrite);
         void DeleteText(TITAN_STRING text_string, TITAN_UINT start_cursor, TITAN_UINT end_cursor, TITAN_UINT max_string_length);
         void MoveSubString(TITAN_STRING text_string, TITAN_UINT source_cursor, TITAN_UINT destination_cursor, TITAN_UINT transfer_size, TITAN_UINT max_string_size);

      private:
         // Private variables:
         AtlasLedgerAPI                               Ledger;                                // Atlas ledge interface
         AtlasAttributeLocationAPI                    Location;                              // Location attribute interface
         MyConsole                                    *m_Console;                            // Hosting console class
         TITAN_BOOL                                   m_word_chars[255];                     // Valid word characters (for text editing)
      };

   // Console axiom class
   class MyConsole : public TitanAxiom
      {
      public:
         // Framework methods:
         MyConsole(TITAN_HANDLE64 handshake);
         ~MyConsole();

      public:
         // Public methods:
         TITAN_HANDLE64 Print(console_data_t *console, titan_vec3_t *position, TITAN_FLOAT scale, titan_vec4_t *color, const TITAN_STRING message, ...);
         void RegisterPerspective(console_data_t *console, TITAN_HANDLE64 perspective_handle);

      public:
         // Video intercept
         static void MyRender(MyGLAPI *GL, TITAN_POINTER user_data);
         static void T_Think(TITAN_FLOAT sim_time, atlas_coordinate_t *coordinate, TITAN_BITMAP32 trigger_flags);
         static void T_EnterHelp(TITAN_FLOAT sim_time, atlas_coordinate_t *coordinate, TITAN_BITMAP32 trigger_flags);
         static void T_LeaveHelp(TITAN_FLOAT sim_time, atlas_coordinate_t *coordinate, TITAN_BITMAP32 trigger_flags);
         static void MyWindowEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics);
         static void MyKeyboardEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics);

      protected:
         // Domain methods:
         // T1: Attach concept to the knowledge base
         virtual titan_result_t AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, const TITAN_STRING attach_params);
         // T3: Attach concept to the simulation
         virtual titan_result_t AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept);
         // T5: Link concept to other concepts in the simulation
         virtual titan_result_t AtlasSim_Link(TITAN_HANDLE64 concept);
         // Instantiator methods:
         // I1: Create an instance for supplied reference
         virtual TITAN_HANDLE64 AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake = 0);
         // I2: Destroy a supplied instance
         virtual titan_result_t AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0);
         // I5: Return the simulation object of supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0);
         // I8: Set supplied instance value
         virtual titan_result_t AtlasSim_SetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0);
         // I9: Get supplied instance value 
         virtual titan_result_t AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0);

      protected:
         // Interpretation methods: BE
         TITAN_NLP_QUERY BE_Online(ATLAS_SIMULATE_PARAMS);

      protected:
         // Interpretation methods: DO
         TITAN_NLP_QUERY DO_Clear(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Activate(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Deactivate(ATLAS_SIMULATE_PARAMS);
      
      public:
         // Public methods (shared with editor):
         titan_result_t CopyToClipboard(TITAN_POINTER window_handle, const TITAN_STRING data) 
                        { return m_GL->CopyToClipboard(window_handle, data); }
         titan_result_t CopyFromClipboard(TITAN_POINTER window_handle, const TITAN_STRING data, TITAN_SIZE64 max_data_size = 0) 
                        { return m_GL->CopyFromClipboard(window_handle, data, max_data_size); }

      private:
         // Private methods:
         TITAN_HANDLE64 GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, console_data_t **camera_data,
                                   TITAN_BITMAP64 query_flags = ATLAS_GET_INSTANCE);
         void SetConsoleKeyboardTranslationTable(console_data_t *console);
         titan_result_t Tell(console_data_t *console, TITAN_HANDLE64 source_private_reference, TITAN_HANDLE64 destination_reference, const TITAN_STRING text);

      protected:
         // Protected variables:
         TITAN_HANDLE64                               m_console_interface_label;             // Console interface label
         TITAN_HANDLE64                               m_console_instance_data_label;         // Instance data label
         TITAN_HANDLE64                               m_v_camera;                            // Console camera value label
         TITAN_INT                                    m_window_width;                        // Width of the client window
         TITAN_INT                                    m_window_height;                       // Height of the client window
         TITAN_INT                                    m_client_center_x;                     // Window center X position (in client coordinates)
         TITAN_INT                                    m_client_center_y;                     // Window center Y position (in client coordinates)
         TITAN_INT                                    m_window_center_x;                     // Window center X position
         TITAN_INT                                    m_window_center_y;                     // Window center Y position
         TITAN_FLOAT                                  m_window_aspect_ratio;                 // Window aspect ratio
         TITAN_BOOL                                   m_wait;                                // If this bit is on, then class should wait

      private:
         // Private variables
         TITAN_HANDLE64                               m_user_clone;                          // User agent Atlas clone
         TITAN_HANDLE64                               m_agent;                               // User agent handle
         TITAN_HANDLE64                               m_concept;                             // Concept handle
         TITAN_HANDLE64                               m_agent_reference;                     // Reference handle
         my_console_intercept_t                       m_console_intercept;                   // Console intercept structure
         MyGLAPI                                      *m_GL;                                 // GL interface
         MyConsoleEditor                              m_Editor;                              // Console editor class (for text editing)

         // Attribute interfaces:
         AtlasAttributeLocationAPI                    Location;                              // Location attribute interface
         AtlasAttributeSizeAPI                        Size;                                  // Size attribute interface
         AtlasAttributeColorAPI                       Color;                                 // Color attribute interface
      };

#endif

/*
   End of file
*/
