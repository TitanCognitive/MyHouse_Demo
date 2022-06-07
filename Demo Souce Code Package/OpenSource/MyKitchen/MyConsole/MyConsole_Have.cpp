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
#include "MyConsole/MyConsole.h"

// Define dependencies
#define TITAN_SYSTEM_MEMORY
#define TITAN_SYSTEM_TOOLS

// Include dependencies
#include TITAN_DEPENDENCIES

/*
   Attach system to the context
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/ 
titan_result_t MyConsole::TitanKB_Attach(TITAN_HANDLE64 presence_handle, TITAN_HANDLE64 concept_handle, const TITAN_STRING attach_params)
{  
   titan_result_t                                     result;

   // Get system dependencies   
   // If the memory interface is not initialized; request a new interace
   if (Memory == NULL) return TITAN_RESULT_NOT_INITIALIZED;
   // If the tools interface is not initialized; request a new interace
   if (Tools == NULL) return TITAN_RESULT_NOT_INITIALIZED;

   // Remember concept
   m_concept = concept_handle;

   // Set simulation methods for the lamp
   // BE block setup   
   if ((result = Cognitive->Be(TITAN_PERSPECTIVE_SUBJECT, "Online", 0, TO_HANDLE64(this), &MyConsole::BE_Online)) != TITAN_RESULT_OK) return result;
   // DO block setup
   if ((result = Cognitive->Do(TITAN_PERSPECTIVE_OBJECT, "Clear", 0, TO_HANDLE64(this), &MyConsole::DO_Clear)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(TITAN_PERSPECTIVE_OBJECT, "Activate", 0, TO_HANDLE64(this), &MyConsole::DO_Activate)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(TITAN_PERSPECTIVE_OBJECT, "Deactivate", 0, TO_HANDLE64(this), &MyConsole::DO_Deactivate)) != TITAN_RESULT_OK) return result;
   // HAVE block setup
   // if ((result = Cognitive->TitanKB_Have(TITAN_PERSPECTIVE_SUBJECT, "Freeze", NULL, 0, TO_HANDLE64(this), &MyConsole::DO_Freeze)) != TITAN_RESULT_OK) return result;

   // Set instantiator; return failure if unsuccessful
   if ((result = Cognitive->SetInstantiator(NULL, 0, TO_INSTANTIATOR(this))) != TITAN_RESULT_OK) return result;

   // Create or retrieve console interface label; return failure if unsuccessful
   if ((m_console_interface_label = Label->GetHandle("Console Interface")) == 0) return TITAN_RESULT_INSERTION_FAIL;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Attach axiom to simulation
*/
titan_result_t MyConsole::TitanSim_Attach(TITAN_HANDLE64 presence_handle, TITAN_HANDLE64 concept_handle)
{
   TITAN_HANDLE64                                     reference_handle;

   // Create a presence; return failure if unsuccessful
   if ((m_presence_private = Presence->Create("My Room", "Public", TITAN_PRESENCE_C1_M1)) == 0) return TITAN_RESULT_INSERTION_FAIL;
   // Get the presence public handle
   m_presence = Presence->GetPublicHandle(m_presence_private);

   // Create references
   if ((reference_handle = Presence->CreateInstanceReference(m_presence, "Console", concept_handle, 1, TITAN_AGENCY_EXPERT)) == 0) return TITAN_RESULT_INIT_FAIL;
   Presence->Resume(reference_handle);

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Link axiom
*/
titan_result_t MyConsole::TitanSim_Link(TITAN_HANDLE64 concept_handle)
{
   TITAN_HANDLE64                                     label_handle;
   titan_value_t                                      value;
   titan_concept_reference_t                          reference_data = { 0 };
   TITAN_BITMAP64                                     *key_states;

   // Grab the construct interface
   if ((label_handle = Presence->GetFirstElement(m_presence, "Construct", reference_data, TITAN_GET_REFERENCE | TITAN_GET_CONCEPT)) != NULL)
      {
      // If the reference is valid, get the video interface from reference; otherwise, return a failure
      if (reference_data.reference) value = Label->GetValue(m_presence, reference_data.reference, "Video Interface");
      else return TITAN_RESULT_RESOURCE_NOT_FOUND;
      // Get GL interface; return failure if unsuccessful
      if (value.format_handle == TITAN_UINT_ERROR || (m_GL = _SP(MyGL, value.raw_value)) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      }

   // If a construct exists
   if (m_GL)
      {
      // Get keyboard key states structure
      m_GL->GetKeyboardState(NULL, &key_states);

      // Set up special keys
      key_states[VK_RETURN] = KEY_STATE_SPECIAL;
      key_states[VK_ESCAPE] = KEY_STATE_SPECIAL;
      key_states[VK_BACK] = KEY_STATE_SPECIAL;
      key_states[VK_F1] = KEY_STATE_SPECIAL;
      key_states[VK_UP] = KEY_STATE_SPECIAL;
      key_states[VK_DOWN] = KEY_STATE_SPECIAL;
      key_states[VK_LEFT] = KEY_STATE_SPECIAL;
      key_states[VK_LSHIFT] = KEY_STATE_SPECIAL;
      key_states[VK_RSHIFT] = KEY_STATE_SPECIAL;
      key_states[VK_LMENU] = KEY_STATE_SPECIAL;
      key_states[VK_RMENU] = KEY_STATE_SPECIAL;
      key_states[VK_RIGHT] = KEY_STATE_SPECIAL;
      key_states[VK_HOME] = KEY_STATE_SPECIAL;
      key_states[VK_END] = KEY_STATE_SPECIAL;
      key_states[VK_SHIFT] = KEY_STATE_SPECIAL;
      key_states[VK_CONTROL] = KEY_STATE_SPECIAL;
      key_states[VK_MENU] = KEY_STATE_SPECIAL;
      }

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Allocate concept data
*/
TITAN_HANDLE64 MyConsole::TitanSim_Create(TITAN_HANDLE64 reference_control_handle, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake)
{
   console_data_t                                      *console;

   // Allocate concept data structure
   if (TITAN_ALLOCATE_FLUSH_ARRAY(console, console_data_t, _S(TITAN_SIZE32, num_elements)) == NULL) return 0;

   // Set console to happy initially
   console->console_reference.reference_control = reference_control_handle;
   console->coordinates.trigger.code_data = TO_HANDLE64(this);
   console->coordinates.trigger.user_data = TO_HANDLE64(console);
   console->coordinates.trigger.Trigger = &T_Think;

   // Set console simulation reference data (scale, angle, position, color)
   VectorClear4(console->coordinates.scale.f.value, 1.0f);
   VectorClear3(console->coordinates.angle.f.value, 0.0f);
   VectorSet3(console->coordinates.position.f.value, 0.0f, 0.0f, -20.0f);
   VectorSet4(console->console_reference.color.f.value, 0.8f + random() * 0.2f, 0.8f + random() * 0.2f, 0.8f + random() * 0.2f, 1.0f);
   // Set extra console data
   VectorSet4(console->input_line_color.f.value, 0.5f, 0.8f, 0.55f, 1.0f);
   // Set console simulation time
   console->coordinates.time = 1.0f;

   // Allocate a new console of size 64-KB; return failure if unsuccessful
   if ((console->console_handle = Console->Create(reference_control_handle, "My Console", 1024 * 64)) == 0) return 0;
   Console->InsertMessage(console->console_handle, 0, "(c) 2020 Titan Virtual Corporation. All rights reserved");
   Console->InsertMessage(console->console_handle, 0, "Press ESC to return to the navigation mode");

   // Set current scroll line in the log (0 = no line selected and scroll all the way to the latest message)
   console->current_line_handle = 0; 

   // Initialize keyboard translation table
   SetConsoleKeyboardTranslationTable(console);
   // Start console simulation
   Presence->SetCoordinate(reference_control_handle, &console->coordinates, &console->transform, TITAN_COORDINATE_TRS);

   // Create a help reference
   if ((console->help_reference.reference_control = Presence->CreateReference(m_presence, "Console Help", m_concept, NULL)) == 0) return 0;
   // Start help simulation
   Presence->SetCoordinate(console->help_reference.reference_control, &console->help_reference.coordinates, &console->help_reference.transform, TITAN_COORDINATE_TRS);
   Presence->Resume(console->help_reference.reference_control);

   // Return object data structure
   return TO_HANDLE64(console);
}


/*
   Get the console instance values based on <format_handle>
*/
titan_value_t MyConsole::TitanSim_GetValue(TITAN_HANDLE64 instance_handle, TITAN_HANDLE64 element_handle, TITAN_HANDLE64 format_handle, TITAN_HANDLE64 handshake)
{
   console_data_t                                     *console;
   titan_value_t                                      value = { TITAN_UINT_ERROR, TITAN_UINT_ERROR };

   // Get data from element if it exists; otherwise, use instance data
   if (element_handle) console = _RP(console_data_t, element_handle);
   else console = _RP(console_data_t, instance_handle);

   // If instance is invalid, return failure
   if (console == NULL) return value;

   // If the video interface is requested
   if (format_handle == m_console_interface_label)
      {
      // Return the window video interface
      value.format_handle = 0;
      value.handle_value = console->console_handle;
      }

   // Return value
   return value;
}

/*
   BE: Online
   Returns: True/False value
   (TITAN_HANDLE64 code_handle, TITAN_HANDLE64 handshake_handle, TITAN_HANDLE64 directive, TITAN_BITMAP32 directive_header)
*/
titan_probe_result_t MyConsole::BE_Online(TITAN_HANDLE64 code_handle, TITAN_HANDLE64 handshake_handle, TITAN_HANDLE64 directive, titan_auxiliary_parameters_t directive_header)
{
   MyConsole                                           *self = _RP(MyConsole, code_handle);
   TITAN_HANDLE64                                     subject_group;
   TITAN_HANDLE64                                     group_element;
   console_data_t                                      *data;
   titan_concept_reference_t                          reference = { 0 };

   // Get subject group from directive; return failure if unsuccessful
   if ((subject_group = self->Directive->Get(directive, TITAN_DIRECTIVE_SUBJECT)) == 0) return NO_RESPONSE();

   // Get subject from directive; return failure if unsuccessful
   if ((group_element = self->Group->Get(subject_group, reference, TITAN_GET_INSTANCE)) == 0) return NO_RESPONSE();

   // Get data from element if it exists; otherwise, use instance data
   if (reference.element) data = _RP(console_data_t, reference.element);
   else data = _RP(console_data_t, reference.instance);

   // If this expression is a query
   if (directive_header.expression_type & TITAN_EXP_TYPE_QUERY)
      {
      // If this console is happy, return true
      if (data->rendering == true) return TO_RESPONSE(100, 0, 0, 0);
      return NO_RESPONSE();
      }
   else
      {
      // If this is not a query, set value
      data->rendering = true;
      }
 
   // Return true
   return TO_RESPONSE(100, 0, 0, 0);
}

/*
   DO: Melt
   Returns: Relevance
*/
titan_probe_result_t MyConsole::DO_Clear(TITAN_HANDLE64 code_handle, TITAN_HANDLE64 handshake_handle, TITAN_HANDLE64 directive, titan_auxiliary_parameters_t directive_header) 
{ 
   MyConsole                                           *self = _RP(MyConsole, code_handle);
   console_data_t                                      *console;

   // Get the subject; return failure if unsuccessful
   if (self->GetElement(directive, TITAN_DIRECTIVE_SUBJECT, &console) == 0 || console == NULL)
      {
      // Return failure
      return NO_RESPONSE();
      }

   // If this expression is a query
   if (directive_header.expression_type & TITAN_EXP_TYPE_QUERY)
      {
      // If this console is sleeping, return true
      if (console->rendering == true) return TO_RESPONSE(100, 0, 0, 0);
      return NO_RESPONSE();
      }
   else
      {
      // Only make take if not sleeping already
      if (console->rendering == false)
         {
         // If this is not a query, set value
         console->rendering = true;
         }
      }

   // Return true
   return TO_RESPONSE(100, 0, 0, 0);
}

/*
   DO: Activate console
   Returns: Relevance
*/
titan_probe_result_t MyConsole::DO_Activate(TITAN_HANDLE64 code_handle, TITAN_HANDLE64 handshake_handle, TITAN_HANDLE64 directive, titan_auxiliary_parameters_t directive_header) 
{ 
   MyConsole                                           *self = _RP(MyConsole, code_handle);
   console_data_t                                      *console;

   // Get the subject; return failure if unsuccessful
   if (self->GetElement(directive, TITAN_DIRECTIVE_SUBJECT, &console, self->m_concept) == 0 || console == NULL)
      {
      // Return failure
      return NO_RESPONSE();
      }

   // Get the object; return failure if unsuccessful
   if (my_camera->GetElement(directive, TITAN_DIRECTIVE_OBJECT1, &object, my_camera->m_concept) == 0 || object == NULL) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.expression_type & TITAN_EXP_TYPE_QUERY)
      {
      // If this console is sleeping, return true
      if (console->rendering == true) return TO_RESPONSE(100, 0, 0, 0);
      return NO_RESPONSE();
      }
   else
      {
      // If a construct exists
      if (self->m_GL)
         {
         // Set up console intercept table
         self->m_console_intercept.console = console;
         self->m_console_intercept.self = self;
         // Set mouse intercept
         if (self->m_GL->SetPostFrameCallback(0, &MyRender, &self->m_console_intercept) != TITAN_RESULT_OK) return NO_RESPONSE();
         if (self->m_GL->SetKeyboardIntercept(&MyKeyboardEvent, &self->m_console_intercept) != TITAN_RESULT_OK) return NO_RESPONSE();
         // Set rendering state
         if (console->rendering == false) console->rendering = true;
         }
      }

   // Return true
   return TO_RESPONSE(100, 0, 0, 0);
}

/*
   DO: Activate console
   Returns: Relevance
*/
titan_probe_result_t MyConsole::DO_Deactivate(TITAN_HANDLE64 code_handle, TITAN_HANDLE64 handshake_handle, TITAN_HANDLE64 directive, titan_auxiliary_parameters_t directive_header) 
{ 
   MyConsole                                           *self = _RP(MyConsole, code_handle);
   console_data_t                                      *console;

   // Get the subject; return failure if unsuccessful
   if (self->GetElement(directive, TITAN_DIRECTIVE_SUBJECT, &console) == 0 || console == NULL)
      {
      // Return failure
      return NO_RESPONSE();
      }

   // If this expression is a query
   if (directive_header.expression_type & TITAN_EXP_TYPE_QUERY)
      {
      // If this console is sleeping, return true
      if (console->rendering == true) return TO_RESPONSE(100, 0, 0, 0);
      return NO_RESPONSE();
      }
   else
      {     
      // If a construct exists
      if (self->m_GL)
         {
         // Release window overlay and keyboard intercept
         self->m_GL->SetPostFrameCallback(0, NULL, NULL);
         self->m_GL->SetKeyboardIntercept(NULL, NULL);
         // Reset rendering state
         if (console->rendering == true) console->rendering = false;
         }
      }

   // Return true
   return TO_RESPONSE(100, 0, 0, 0);
}
/*
   End of file
*/
